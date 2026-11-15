#!/usr/bin/env python3
"""Verify the existing 140-check Myriad suite on Windows."""
import argparse
import datetime
import json
from pathlib import Path
import re
import sys
from types import SimpleNamespace

from sunc_runner import ROOT, console_lines, run_suite

PLACE_ID = 79035306837882
SCRIPT = Path(r"D:\NERO\MacOS\tests\myriad_full.lua")


def validate_report(output):
    lines = console_lines(output)
    checks = [re.fullmatch(r"MYR (\d+)\|([^|]+)\|([^|]+)\|(.*)", line) for line in lines]
    checks = [match for match in checks if match]
    if lines.count(f"MYR-PLACE {PLACE_ID}") != 1 or len(checks) != 140:
        raise ValueError(f"Incomplete Myriad report: {len(checks)}/140 checks")
    if len({(check[2], check[3]) for check in checks}) != 140:
        raise ValueError("Repeated Myriad checks")
    failures = [f"{check[2]}.{check[3]}: {check[4]}" for check in checks if check[1] != "200"]
    if failures:
        raise ValueError("; ".join(failures))
    if lines.count("MYR-DONE ran=140 200=140") != 1:
        raise ValueError("Missing or repeated successful completion marker")
    return {"place_id": PLACE_ID, "ran": 140, "passed": 140, "failed": 0}


def main():
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=ROOT / ".maintenance/myriad-regression.log")
    parser.add_argument("--build", action="store_true")
    parser.add_argument("--check-log", type=Path)
    args = parser.parse_args()
    try:
        if args.check_log:
            output, log = args.check_log.read_text(encoding="utf-8"), args.check_log
        else:
            runtime = SimpleNamespace(build=args.build, script=SCRIPT, output=args.output, timeout=400)
            output, log = run_suite(runtime, "MYR-DONE ", False, PLACE_ID, True)
        report = validate_report(output)
        report.update(player_log=str(log), completed_utc=datetime.datetime.now(datetime.timezone.utc).isoformat())
        args.output.with_suffix(".json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    except (OSError, RuntimeError, ValueError) as error:
        print(f"MYRIAD FAIL: {error}; log: {args.output}", flush=True)
        return 1
    print("MYRIAD PASS: 140/140", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
