#!/usr/bin/env python3
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[2]
PROTOCOL_SOURCE = (ROOT / "App/Protocol/ProtocolManager.cpp").read_text(
    encoding="utf-8-sig",
    errors="ignore",
)


def require(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is None:
        raise AssertionError(message)


def reject(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is not None:
        raise AssertionError(message)


def main() -> int:
    require(
        PROTOCOL_SOURCE,
        r"static\s+bool\s+ParseGbMediaFVideoConfig\s*\(",
        "ProtocolManager should keep the GB live f= video parameter parser.",
    )
    require(
        PROTOCOL_SOURCE,
        r"static\s+int\s+MaybeApplyGbLiveMediaFVideoConfig\s*\(",
        "ProtocolManager should keep the GB live f= encode apply helper.",
    )
    require(
        PROTOCOL_SOURCE,
        r"int\s+ret\s*=\s*MaybeApplyGbLiveMediaFVideoConfig\s*\(\s*requestedStreamNum\s*,\s*mediaF\s*,\s*gbCode\s*\)\s*;\s*if\s*\(\s*ret\s*!=\s*0\s*\)\s*\{\s*return\s+ret\s*;\s*\}\s*ret\s*=\s*ReconfigureGbLiveSender\s*\(",
        "HandleGbLiveStreamRequest should apply f= encode parameters before sender reconfiguration.",
    )
    reject(
        PROTOCOL_SOURCE,
        r"//\s*MaybeApplyGbLiveMediaFVideoConfig\s*\(",
        "GB live f= encode apply call should not be commented out.",
    )

    print("PASS: gb live INVITE applies f= video encode parameters before sender setup")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
