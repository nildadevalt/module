#!/usr/bin/env python3
"""
Automated sUNC runner for this workspace (Bloxstrap + emulator + module).
- Deploys latest Module.dll to Bloxstrap version dir
- Launches Roblox place 90441122676618
- Waits for module (port 6969), injects readiness + sUNC script
- Monitors player log for progress/results, detects disconnect/exit/timeout
- Tracks UI "Not Responding" periods (freeze) and restarts on failure
- Saves run artifacts under .maintenance/auto-sunc-*

Run: python auto_sunc.py [--runs N] [--timeout S] [--no-deploy] [--script PATH]
"""
from __future__ import annotations

import argparse
import ctypes
from ctypes import wintypes
import datetime
import json
import os
from pathlib import Path
import re
import socket
import struct
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parent
PLACE_ID = 95206881  # default: Baseplate (sUNC place 90441122676618 may be age-restricted)
VERSION_DIR = Path(os.environ.get("LOCALAPPDATA", "")) / "Bloxstrap" / "Versions" / "version-4ea8e85481e34f46"
PLAYER_LOGS = Path(os.environ.get("LOCALAPPDATA", "")) / "Roblox" / "logs"
MSBUILD = Path(r"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe")
MODULE_SRC = ROOT / "Module" / "x64" / "Release" / "Module.dll"
DECOMPILER_SRC = ROOT / "Module" / "Dependencies" / "Tovek" / "decompiler.exe"
OFFICIAL_SCRIPT = ROOT / "Module" / ".maintenance" / "sunc-official.lua"
OUT_DIR = ROOT / "Module" / ".maintenance"


def safe_print(msg: str) -> None:
    try:
        print(msg, flush=True)
    except UnicodeEncodeError:
        sys.stdout.buffer.write((msg + "\n").encode("utf-8", errors="replace"))
        sys.stdout.flush()


def log(msg: str) -> None:
    ts = datetime.datetime.now().strftime("%H:%M:%S")
    safe_print(f"[{ts}] {msg}")


def ps(script: str, timeout: int = 30) -> str:
    r = subprocess.run(
        ["powershell.exe", "-NoProfile", "-NonInteractive", "-Command", script],
        capture_output=True, text=True, encoding="utf-8", errors="replace", timeout=timeout,
    )
    return (r.stdout or "") + (r.stderr or "")


def kill_roblox() -> None:
    ps(
        "$ErrorActionPreference='SilentlyContinue'; "
        "Get-Process RobloxPlayerBeta -ErrorAction SilentlyContinue | Stop-Process -Force; "
        "Start-Sleep -Seconds 2"
    )
    for _ in range(20):
        out = ps("(Get-Process RobloxPlayerBeta -ErrorAction SilentlyContinue | Measure-Object).Count").strip()
        if out.endswith("0") or out == "0":
            return
        time.sleep(0.5)
    # last resort
    subprocess.run(["taskkill", "/F", "/IM", "RobloxPlayerBeta.exe"], capture_output=True)


def deploy() -> dict:
    if not MODULE_SRC.is_file():
        raise RuntimeError(f"Missing built module: {MODULE_SRC}")
    if not VERSION_DIR.is_dir():
        raise RuntimeError(f"Missing Bloxstrap version dir: {VERSION_DIR}")
    subprocess.run(
        ["powershell.exe", "-NoProfile", "-NonInteractive", "-Command",
         f"Copy-Item -LiteralPath '{MODULE_SRC}' -Destination '{VERSION_DIR / 'module.dll'}' -Force; "
         f"if (Test-Path '{DECOMPILER_SRC}') {{ Copy-Item -LiteralPath '{DECOMPILER_SRC}' -Destination '{VERSION_DIR / 'decompiler.exe'}' -Force -ErrorAction SilentlyContinue }}"],
        check=False, capture_output=True,
    )
    st = (VERSION_DIR / "module.dll").stat()
    return {"path": str(VERSION_DIR / "module.dll"), "bytes": st.st_size, "mtime": st.st_mtime}


def read_shared(path: Path) -> str:
    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel.CreateFileW.argtypes = [wintypes.LPCWSTR, wintypes.DWORD, wintypes.DWORD,
                                   ctypes.c_void_p, wintypes.DWORD, wintypes.DWORD, wintypes.HANDLE]
    kernel.CreateFileW.restype = wintypes.HANDLE
    kernel.ReadFile.argtypes = [wintypes.HANDLE, ctypes.c_void_p, wintypes.DWORD,
                                ctypes.POINTER(wintypes.DWORD), ctypes.c_void_p]
    kernel.CloseHandle.argtypes = [wintypes.HANDLE]
    handle = kernel.CreateFileW(str(path), 0x80000000, 7, None, 3, 0, None)
    if handle == ctypes.c_void_p(-1).value:
        if ctypes.get_last_error() in (2, 3):
            return ""
        raise ctypes.WinError(ctypes.get_last_error())
    chunks = []
    try:
        buf = ctypes.create_string_buffer(65536)
        count = wintypes.DWORD()
        while True:
            if not kernel.ReadFile(handle, buf, len(buf), ctypes.byref(count), None):
                raise ctypes.WinError(ctypes.get_last_error())
            if not count.value:
                break
            chunks.append(buf.raw[:count.value])
    finally:
        kernel.CloseHandle(handle)
    return b"".join(chunks).decode("utf-8", errors="replace")


def console_lines(output: str):
    lines = []
    for line in output.splitlines():
        m = re.search(r"\[FLog::Creator(?:Output|Warning|Error)\]\s*(.*)", line)
        line = m[1] if m else line
        lines.append(line.replace("\u200c", "").replace("\ufeff", "").strip())
    return lines


def parse_report(output: str):
    lines = console_lines(output)
    checks, summaries = [], []
    for index, line in enumerate(lines):
        check = re.fullmatch(r"(✅|❌|⛔) ([A-Za-z_]\w*(?:\.\w+)*)", line)
        if check:
            checks.append((index, check[1], check[2]))
        summary = re.search(r"with (\d+)% success rate \((\d+) out of (\d+)\)", line)
        if summary:
            summaries.append((index, *map(int, summary.groups())))
    return lines, checks, summaries


def process_alive(pid: int) -> bool:
    out = ps(f"$p=Get-Process -Id {pid} -ErrorAction SilentlyContinue; if($p){{'1'}}else{{'0'}}", timeout=10).strip()
    return out.endswith("1")


def process_responding(pid: int) -> bool | None:
    out = ps(f"$p=Get-Process -Id {pid} -ErrorAction SilentlyContinue; if($p){{$p.Responding}}else{{'NA'}}", timeout=10).strip().lower()
    if "true" in out:
        return True
    if "false" in out:
        return False
    return None


def port_ready(pid: int | None = None) -> bool:
    try:
        with socket.create_connection(("127.0.0.1", 6969), timeout=1.0):
            return True
    except Exception:
        return False


def send_script_bytes(data: bytes) -> None:
    with socket.create_connection(("127.0.0.1", 6969), timeout=5.0) as s:
        s.sendall(struct.pack("!I", len(data)) + data)


def launch_place(place_id: int) -> None:
    os.startfile(f"roblox://experiences/start?placeId={place_id}")


def find_player_log(start_time: float) -> Path | None:
    if not PLAYER_LOGS.is_dir():
        return None
    cands = [p for p in PLAYER_LOGS.glob("*Player*last.log")
             if "CrashHandler" not in p.name and p.stat().st_mtime >= start_time - 5]
    if not cands:
        return None
    return max(cands, key=lambda p: p.stat().st_mtime)


def run_once(script_path: Path, timeout: int, run_id: str) -> dict:
    start_time = time.time()
    kill_roblox()
    launch_place(PLACE_ID)
    log(f"launched place {PLACE_ID}")

    # wait for process
    pid = None
    deadline = time.monotonic() + 60
    while time.monotonic() < deadline:
        out = ps("(Get-Process RobloxPlayerBeta -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty Id)").strip()
        if out.isdigit():
            pid = int(out)
            break
        time.sleep(1)
    if not pid:
        raise RuntimeError("Roblox process did not start")

    # wait for port 6969
    deadline = time.monotonic() + 90
    while time.monotonic() < deadline:
        if not process_alive(pid):
            raise RuntimeError("Roblox exited before module ready (port 6969)")
        if port_ready(pid):
            break
        time.sleep(0.5)
    else:
        raise RuntimeError("Module port 6969 not ready within 90s")
    log(f"module ready pid={pid}")

    # wait a bit for in-game join/data model
    time.sleep(8)

    source = script_path.read_bytes()
    readiness = (
        b'local players = game:GetService("Players")\n'
        b'if not game:IsLoaded() then game.Loaded:Wait() end\n'
        b'local player = players.LocalPlayer\n'
        b'while not player do task.wait(); player = players.LocalPlayer end\n'
        b'pcall(function() player:WaitForChild("PlayerScripts", 60) end)\n'
        b'task.wait(3)\n'
    )
    payload = readiness + source

    # A single submit right after the port opens can land before the DataModel
    # finished loading; the module then queues it but it never executes and the
    # run looks like a hang with no output. Re-submit until the player log shows
    # the script actually produced output.
    deadline = time.monotonic() + 45
    attempt = 0
    while True:
        attempt += 1
        send_script_bytes(payload)
        log(f"submitted {len(payload)} bytes (attempt {attempt})")

        wait_until = time.monotonic() + 6
        while time.monotonic() < wait_until:
            candidate = find_player_log(start_time)
            if candidate:
                try:
                    text = read_shared(candidate)
                except Exception:
                    text = ""
                if "[RUN]" in text or "DIAG" in text or "[MD]" in text or "🟢" in text or "🟡" in text:
                    break
            time.sleep(0.5)
        else:
            if time.monotonic() >= deadline:
                break
            continue
        break

    player_log: Path | None = None
    output = ""
    checks_seen = 0
    printed = set()
    not_responding_since: float | None = None
    max_not_responding: float = 0.0
    freeze_events = 0

    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if not process_alive(pid):
            raise RuntimeError("Roblox process exited")

        responding = process_responding(pid)
        now = time.monotonic()
        if responding is False:
            if not_responding_since is None:
                not_responding_since = now
                freeze_events += 1
                log("WARN: window not responding (freeze detected)")
            max_not_responding = max(max_not_responding, now - not_responding_since)
        else:
            if not_responding_since is not None:
                dur = now - not_responding_since
                log(f"window responding again after {dur:.1f}s")
            not_responding_since = None

        if player_log is None:
            player_log = find_player_log(start_time)
            if player_log:
                log(f"player log: {player_log}")

        if player_log:
            try:
                output = read_shared(player_log)
            except Exception:
                output = ""
            if "Client has been disconnected with reason:" in output:
                raise RuntimeError("Roblox disconnected from server")
            lines, checks, summaries = parse_report(output)
            checks_seen = len(checks)
            for index, line in enumerate(lines):
                if index in printed:
                    continue
                if "SUNCIO" in line or "✅" in line or "❌" in line or "⛔" in line or "success rate" in line or "https://r.sunc.su/" in line or "UNC Summary" in line or "tests failed" in line or "globals are missing" in line or "Completed in" in line or "⏱️" in line:
                    safe_print(line)
                    printed.add(index)
            # completion heuristics: SUNCIO-DONE or summary line seen
            has_done = any(l.strip() == "SUNCIO-DONE" or l.strip().startswith("SUNCIO-DONE") for l in lines)
            has_summary = bool(summaries)
            if has_done or (has_summary and checks_seen >= 1):
                # give a moment for trailing lines
                time.sleep(2)
                try:
                    output = read_shared(player_log)
                except Exception:
                    pass
                lines, checks, summaries = parse_report(output)
                report = {
                    "status": "completed",
                    "run_id": run_id,
                    "place_id": PLACE_ID,
                    "checks_received": len(checks),
                    "failed_checks": [name for _, sym, name in checks if sym in ("❌", "⛔")],
                    "has_done_marker": any("SUNCIO-DONE" in l for l in lines),
                    "freeze_events": freeze_events,
                    "max_not_responding_sec": round(max_not_responding, 2),
                    "player_log": str(player_log) if player_log else None,
                }
                if summaries:
                    _, percent, passed, total = summaries[-1]
                    report.update({"percent": percent, "passed": passed, "total": total})
                return report

        time.sleep(1.0)

    raise RuntimeError(f"timeout after {timeout}s (checks_seen={checks_seen}, freeze_events={freeze_events}, max_nr={max_not_responding:.1f}s)")


def main() -> int:
    global PLACE_ID
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--runs", type=int, default=1)
    ap.add_argument("--timeout", type=int, default=420)
    ap.add_argument("--script", type=Path, default=OFFICIAL_SCRIPT)
    ap.add_argument("--no-deploy", action="store_true")
    ap.add_argument("--build", action="store_true")
    ap.add_argument("--place-id", type=int, default=PLACE_ID)
    args = ap.parse_args()
    PLACE_ID = args.place_id

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    if args.build:
        log("building module...")
        r = subprocess.run([str(MSBUILD), "Module/VynxVX.vcxproj", "/p:Configuration=Release", "/p:Platform=x64", "/m", "/nologo", "/v:minimal"], cwd=ROOT)
        if r.returncode != 0:
            log("build failed")
            return 2

    results = []
    try:
        for i in range(1, args.runs + 1):
            run_id = datetime.datetime.now().strftime(f"auto-sunc-%Y%m%dT%H%M%SZ-r{i}")
            log(f"=== run {i}/{args.runs} ({run_id}) ===")
            try:
                kill_roblox()
                if not args.no_deploy:
                    info = deploy()
                    log(f"deployed module: {info['bytes']} bytes")
                report = run_once(args.script, args.timeout, run_id)
                report["completed_utc"] = datetime.datetime.now(datetime.timezone.utc).isoformat()
                log(f"RESULT: {json.dumps(report, ensure_ascii=False)}")
            except Exception as e:
                report = {
                    "status": "error",
                    "run_id": run_id,
                    "place_id": PLACE_ID,
                    "error": str(e),
                    "completed_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
                }
                log(f"ERROR: {e}")
            try:
                (OUT_DIR / f"{run_id}.json").write_text(json.dumps(report, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
            except Exception:
                pass
            results.append(report)
            if i < args.runs:
                time.sleep(3)
    finally:
        log("stopping Roblox...")
        kill_roblox()

    ok = sum(1 for r in results if r.get("status") == "completed")
    log(f"done: {ok}/{len(results)} completed")
    return 0 if ok == len(results) and results else 1


if __name__ == "__main__":
    sys.exit(main())
