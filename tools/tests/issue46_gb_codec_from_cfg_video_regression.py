#!/usr/bin/env python3
from pathlib import Path
import re
import sys


TARGET = Path("/home/jerry/silver/rk_gb/App/Protocol/config/LocalConfigProvider.cpp")


def require(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE) is None:
        raise AssertionError(message)


def reject(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE) is not None:
        raise AssertionError(message)


def main() -> int:
    text = TARGET.read_text(encoding="utf-8-sig")

    require(
        text,
        r"g_configManager\.getConfig\(getConfigName\(CFG_VIDEO\),\s*table\)",
        "LocalConfigProvider should read CFG_VIDEO instead of relying on fixed GB codec defaults.",
    )
    require(
        text,
        r"TExchangeAL<VideoConf_S>::getConfig\(table,\s*[A-Za-z_][A-Za-z0-9_]*\)",
        "LocalConfigProvider should decode CFG_VIDEO into VideoConf_S.",
    )
    reject(
        text,
        r'cfg\.gb_live\.video_codec\s*=\s*"h265"\s*;',
        "GB live codec is still hardcoded to h265.",
    )
    reject(
        text,
        r'cfg\.gb_video\.main_codec\s*=\s*"H\.265"\s*;',
        "GB main stream codec is still hardcoded to H.265.",
    )
    reject(
        text,
        r'cfg\.gb_video\.sub_codec\s*=\s*"H\.265"\s*;',
        "GB sub stream codec is still hardcoded to H.265.",
    )

    print("PASS: issue46 gb codec defaults follow CFG_VIDEO regression checks")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
