#!/usr/bin/env python3
"""
Send a Lua script to the module's local script server (127.0.0.1:6969).

Protocol (Communication.cpp):
  - connect TCP 127.0.0.1:6969
  - send uint32 script length, network byte order (big-endian)
  - send script bytes
  - close

Usage:
  python send_script.py "print('hi')"
  python send_script.py -f path/to/script.lua
  echo "print('hi')" | python send_script.py -
"""

from __future__ import annotations

import argparse
import socket
import struct
import sys


def send(script: str, host: str = "127.0.0.1", port: int = 6969, timeout: float = 5.0) -> None:
    data = script.encode("utf-8")
    if not data:
        raise SystemExit("script is empty")
    if len(data) > 8 * 1024 * 1024:
        raise SystemExit("script too large (>8MB)")

    with socket.create_connection((host, port), timeout=timeout) as s:
        s.sendall(struct.pack("!I", len(data)))
        s.sendall(data)


def main() -> int:
    ap = argparse.ArgumentParser(description="Send a Lua script to 127.0.0.1:6969")
    ap.add_argument("script", nargs="?", help="Lua script text, or '-' to read stdin")
    ap.add_argument("-f", "--file", help="Read script from a file")
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=6969)
    ap.add_argument("--timeout", type=float, default=5.0)
    args = ap.parse_args()

    if args.file:
        with open(args.file, "r", encoding="utf-8-sig") as f:
            script = f.read()
    elif args.script == "-":
        script = sys.stdin.read()
    elif args.script is not None:
        script = args.script
    else:
        ap.error("provide a script, -f FILE, or '-' for stdin")

    try:
        send(script, host=args.host, port=args.port, timeout=args.timeout)
    except Exception as e:
        print(f"error: {e}", file=sys.stderr)
        return 1

    print(f"sent {len(script.encode('utf-8'))} bytes to {args.host}:{args.port}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
