#!/usr/bin/env python3
from pathlib import Path
import re
import sys


PROTOCOL_TARGET = Path("/home/jerry/silver/rk_gb/App/Protocol/ProtocolManager.cpp")
LOCAL_CONFIG_TARGET = Path("/home/jerry/silver/rk_gb/App/Protocol/config/LocalConfigProvider.cpp")


def require(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE) is None:
        raise AssertionError(message)


def reject(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE) is not None:
        raise AssertionError(message)


def main() -> int:
    protocol_text = PROTOCOL_TARGET.read_text(encoding="utf-8-sig")
    local_config_text = LOCAL_CONFIG_TARGET.read_text(encoding="utf-8-sig")

    require(
        protocol_text,
        r"g_configManager\.getConfig\(getConfigName\(CFG_VIDEO\),\s*table\)",
        "GB preview should read CFG_VIDEO when starting the preview stream.",
    )
    require(
        protocol_text,
        r"TExchangeAL<VideoConf_S>::getConfig\(table,\s*[A-Za-z_][A-Za-z0-9_]*\)",
        "GB preview should decode CFG_VIDEO into VideoConf_S when starting the preview stream.",
    )
    require(
        protocol_text,
        r"ReconfigureGbLiveSender",
        "Regression target should stay on the GB preview startup path.",
    )
    reject(
        local_config_text,
        r"g_configManager\.getConfig\(getConfigName\(CFG_VIDEO\),\s*table\)",
        "CFG_VIDEO lookup should not live in LocalConfigProvider defaults anymore.",
    )

    print("PASS: issue46 gb preview codec is fetched from CFG_VIDEO at stream start")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
