#!/usr/bin/env python3
"""Run the official sUNC suite against Module.dll on Windows."""

import argparse
import ctypes
from ctypes import wintypes
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import socket
import struct
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parent.parent
PLACE_ID = 90441122676618
EXPECTED_CHECKS = 86
VERSION = Path(r"C:\Users\LASSE\AppData\Local\Roblox\Versions\version-4ea8e85481e34f46")
PLAYER_LOGS = VERSION.parent.parent / "logs"
MSBUILD = Path(r"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe")
DEFAULT_SCRIPT = Path(r"D:\NERO\MacOS\tests\sunc_current.lua")


def console_lines(output):
    lines = []
    for line in output.splitlines():
        match = re.search(r"\[FLog::Creator(?:Output|Warning|Error)\]\s*(.*)", line)
        line = match[1] if match else line
        lines.append(line.replace("\u200c", "").replace("\ufeff", "").strip())
    return lines


def parse_report(output):
    lines = console_lines(output)
    checks, summaries = [], []
    for index, line in enumerate(lines):
        check = re.fullmatch(r"(\u2705|\u274c) ([A-Za-z_]\w*(?:\.\w+)*)", line)
        if check:
            checks.append((index, check[1], check[2]))
        summary = re.search(r"with (\d+)% success rate \((\d+) out of (\d+)\)", line)
        if summary:
            summaries.append((index, *map(int, summary.groups())))
    return lines, checks, summaries


def validate_report(output, *, require_success=True):
    def expect(condition, message):
        if not condition:
            raise ValueError(message)

    lines, checks, summaries = parse_report(output)
    marker = f"SUNCIO-PLACE {PLACE_ID}"
    expect(not any("Client has been disconnected with reason:" in line for line in lines),
           "Roblox disconnected from the server; this run is invalid")
    expect(lines.count(marker) == 1, "Missing or repeated place marker")
    expect(lines.count("SUNCIO-DONE") == 1, "Missing or repeated completion marker")
    expect(len(checks) == EXPECTED_CHECKS, f"Received {len(checks)}/{EXPECTED_CHECKS} checks")
    expect(len({name for _, _, name in checks}) == EXPECTED_CHECKS, "Duplicate check results")
    expect(len(summaries) == 1, "Missing or repeated suite summary")
    summary_index, percent, passed, total = summaries[0]
    expect(lines.index(marker) < checks[0][0], "Results precede the place marker")
    expect(checks[-1][0] < summary_index < lines.index("SUNCIO-DONE"), "Report is out of order")
    failures = [name for _, status, name in checks if status == "\u274c"]
    expect(total == EXPECTED_CHECKS and passed == total - len(failures), "Summary disagrees with checks")
    expect(percent == passed * 100 // total, "Percentage disagrees with checks")
    if require_success:
        expect(percent == 100 and not failures, f"{passed}/{total} passed; failures: {', '.join(failures)}")
    return {"place_id": PLACE_ID, "ran": total, "passed": passed, "failed": len(failures), "percent": percent}


def failure_report(output, error):
    lines, checks, summaries = parse_report(output)
    try:
        validated = validate_report(output, require_success=False)
    except ValueError:
        validated = None
    disconnected = any("Client has been disconnected with reason:" in line for line in lines)
    report = {
        "status": "failed" if validated and validated["failed"] else "invalid",
        "place_id": PLACE_ID,
        "error": str(error),
        "checks_received": len(checks),
        "failed_checks": [name for _, symbol, name in checks if symbol == "\u274c"],
        "completion_marker_received": "SUNCIO-DONE" in lines,
        "disconnected": disconnected,
    }
    if validated:
        report.update(validated)
    return report


def powershell(source):
    result = subprocess.run(
        ["powershell.exe", "-NoProfile", "-NonInteractive", "-Command", source],
        cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace",
        creationflags=subprocess.CREATE_NO_WINDOW, timeout=40,
    )
    if result.returncode:
        raise RuntimeError(result.stderr.strip() or result.stdout.strip())


def close_player():
    powershell("""$ErrorActionPreference = 'Stop'
$players = @(Get-Process -Name RobloxPlayerBeta -ErrorAction SilentlyContinue)
foreach ($player in $players) {
    Stop-Process -Id $player.Id -Force
    if (-not $player.WaitForExit(10000)) { throw 'Roblox did not exit' }
}
if (Get-Process -Name RobloxPlayerBeta -ErrorAction SilentlyContinue) { throw 'Roblox is still running' }
""")


def read_shared(path):
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
        buffer = ctypes.create_string_buffer(65536)
        count = wintypes.DWORD()
        while True:
            if not kernel.ReadFile(handle, buffer, len(buffer), ctypes.byref(count), None):
                raise ctypes.WinError(ctypes.get_last_error())
            if not count.value:
                break
            chunks.append(buffer.raw[:count.value])
    finally:
        kernel.CloseHandle(handle)
    return b"".join(chunks).decode("utf-8", errors="replace")


def run_suite(args, completion_marker="SUNCIO-DONE", require_summary=True, place_id=PLACE_ID, completion_prefix=False):
    if os.name != "nt":
        raise RuntimeError("This runner requires Windows; --check-log works on any platform")
    close_player()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    if args.build:
        build_log = args.output.with_suffix(".build.log")
        with build_log.open("w", encoding="utf-8") as log:
            result = subprocess.run(
                [str(MSBUILD), "VynxVX.vcxproj", "/p:Configuration=Release", "/p:Platform=x64",
                 "/m", "/nologo", "/v:minimal"], cwd=ROOT, stdout=log, stderr=subprocess.STDOUT,
                creationflags=subprocess.CREATE_NO_WINDOW,
            )
        if result.returncode:
            raise RuntimeError(f"Build failed; see {build_log}")
    if not (VERSION / "Module.dll").is_file():
        raise RuntimeError(f"Missing {VERSION / 'Module.dll'}")

    module_log = VERSION / "Logs.log"
    prior = read_shared(module_log)
    start = time.time()
    os.startfile(f"roblox://placeId={place_id}")
    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
    kernel.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
    kernel.OpenProcess.restype = wintypes.HANDLE
    kernel.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
    kernel.CloseHandle.argtypes = [wintypes.HANDLE]
    process_handle = None
    player_log = None
    session = ""
    try:
        deadline = time.monotonic() + 45
        while time.monotonic() < deadline:
            content = read_shared(module_log)
            session = content[len(prior):] if content.startswith(prior) else content
            ids = re.findall(r"\[Module\].*pid=(\d+)", session)
            if ids and not process_handle:
                process_handle = kernel.OpenProcess(0x100000, False, int(ids[-1]))
                if not process_handle:
                    raise RuntimeError("The module's Player process exited before initialization")
            if process_handle and kernel.WaitForSingleObject(process_handle, 0) == 0:
                raise RuntimeError("Roblox exited during initialization")
            if "[TaskScheduler] Execution initialized:" in session:
                break
            time.sleep(0.25)
        else:
            raise RuntimeError("Module did not initialize within 45 seconds")

        source = args.script.read_bytes()
        # The module initializes before PlayerScripts and the test place's
        # server fixtures are necessarily available.
        readiness = b'''local players = game:GetService("Players")
if not game:IsLoaded() then game.Loaded:Wait() end
local player = players.LocalPlayer
while not player do task.wait(); player = players.LocalPlayer end
assert(player:WaitForChild("PlayerScripts", 60), "PlayerScripts did not initialize")
task.wait(3)
'''
        source = readiness + source
        with socket.create_connection(("127.0.0.1", 6969), timeout=5) as client:
            client.sendall(struct.pack("!I", len(source)) + source)
        print(f"Submitted {len(source)} bytes in place {place_id}", flush=True)
        deadline = time.monotonic() + args.timeout
        printed = set()
        while time.monotonic() < deadline:
            if process_handle and kernel.WaitForSingleObject(process_handle, 0) == 0:
                raise RuntimeError("Roblox process exited; this run is invalid")
            if player_log is None:
                candidates = [p for p in PLAYER_LOGS.glob("*Player*last.log")
                              if "CrashHandler" not in p.name and p.stat().st_mtime >= start - 2]
                if candidates:
                    player_log = max(candidates, key=lambda p: p.stat().st_mtime)
                    print(f"Player log: {player_log}", flush=True)
            if player_log:
                output = read_shared(player_log)
                args.output.write_text(output, encoding="utf-8")
                if "Client has been disconnected with reason:" in output:
                    raise RuntimeError("Roblox disconnected from the server; this run is invalid")
                lines, checks, summaries = parse_report(output)
                for index, line in enumerate(lines):
                    if index in printed:
                        continue
                    if "SUNCIO" in line or "\u274c" in line or "success rate" in line or "https://r.sunc.su/" in line:
                        print(line, flush=True)
                        printed.add(index)
                completed = any(line.startswith(completion_marker) for line in lines) if completion_prefix else completion_marker in lines
                if completed:
                    if require_summary and not summaries:
                        raise RuntimeError("The suite finished without a valid summary")
                    return output, player_log
            time.sleep(0.5)
        raise RuntimeError(f"Suite did not complete within {args.timeout} seconds")
    finally:
        content = read_shared(module_log)
        session = content[len(prior):] if content.startswith(prior) else content
        args.output.with_suffix(".module.log").write_text(session, encoding="utf-8")
        if process_handle:
            kernel.CloseHandle(process_handle)
        had_error = sys.exc_info()[0] is not None
        try:
            close_player()
        except (OSError, RuntimeError, subprocess.SubprocessError) as error:
            if not had_error:
                raise
            print(f"Roblox cleanup failed: {error}", file=sys.stderr, flush=True)


def main():
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / ".maintenance/sunc-current.log")
    parser.add_argument("--script", type=Path, default=DEFAULT_SCRIPT)
    parser.add_argument("--check-log", type=Path)
    parser.add_argument("--timeout", type=int, default=300)
    parser.add_argument("--build", action="store_true", help="Build Release x64 before launching")
    args = parser.parse_args()
    output = ""
    player_log = args.check_log
    artifact = None
    status = 0
    try:
        if args.check_log:
            output, player_log = args.check_log.read_text(encoding="utf-8"), args.check_log
        else:
            # Reusing a report path must never carry a previous session's score.
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_text("", encoding="utf-8")
            output, player_log = run_suite(args)
        report = validate_report(output)
        report["status"] = "passed"
    except (OSError, RuntimeError, ValueError, subprocess.SubprocessError) as error:
        if not args.check_log:
            try:
                output = args.output.read_text(encoding="utf-8", errors="replace")
            except OSError:
                output = ""
        report = failure_report(output, error)
        print(f"SUNC FAIL: {error}; log: {args.output}", flush=True)
        status = 1
    if not args.check_log:
        try:
            module = VERSION / "Module.dll"
            artifact = {"path": str(module), "bytes": module.stat().st_size,
                        "sha256": hashlib.sha256(module.read_bytes()).hexdigest()}
        except OSError:
            pass
    report.update(player_log=str(player_log) if player_log else None,
                  evidence_log=str(args.check_log or args.output),
                  completed_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
    if artifact:
        report["deployed_module"] = artifact
    try:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.with_suffix(".json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    except OSError as error:
        print(f"SUNC FAIL: Could not save result report: {error}", file=sys.stderr, flush=True)
        return 1
    if status:
        return status
    print(f"SUNC PASS: {EXPECTED_CHECKS}/{EXPECTED_CHECKS} in place {PLACE_ID}", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
