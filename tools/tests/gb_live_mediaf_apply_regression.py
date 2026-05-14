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


def function_body(text: str, signature_pattern: str, message: str) -> str:
    match = re.search(signature_pattern, text, re.MULTILINE)
    if match is None:
        raise AssertionError(message)

    open_brace = text.find("{", match.end())
    if open_brace == -1:
        raise AssertionError(message)

    depth = 0
    for index in range(open_brace, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return text[open_brace + 1 : index]

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

    live_body = function_body(
        PROTOCOL_SOURCE,
        r"int\s+ProtocolManager::HandleGbLiveStreamRequest\s*\(",
        "ProtocolManager should keep HandleGbLiveStreamRequest.",
    )
    apply_index = live_body.find("MaybeApplyGbLiveMediaFVideoConfig(requestedStreamNum, mediaF, gbCode)")
    reconfigure_index = live_body.find("ReconfigureGbLiveSender(input, gbCode, kLiveStream)")
    if apply_index == -1 or reconfigure_index == -1 or apply_index > reconfigure_index:
        raise AssertionError(
            "HandleGbLiveStreamRequest should apply f= encode parameters before sender reconfiguration."
        )

    reject(
        live_body,
        r"MaybeApplyGbLiveMediaFVideoConfig\s*\([^\)]*\)\s*;\s*if\s*\(\s*ret\s*!=\s*0\s*\)\s*\{\s*return\s+ret\s*;",
        "GB live INVITE should not reject the session only because best-effort f= encode apply failed.",
    )

    helper_body = function_body(
        PROTOCOL_SOURCE,
        r"static\s+int\s+MaybeApplyGbLiveMediaFVideoConfig\s*\(",
        "ProtocolManager should keep MaybeApplyGbLiveMediaFVideoConfig.",
    )
    require(
        helper_body,
        r"media::ApplyVideoEncodeStreamConfig\s*\(",
        "MaybeApplyGbLiveMediaFVideoConfig should still dispatch parsed f= values to media encode control.",
    )
    reject(
        helper_body,
        r"return\s+ret\s*;",
        "MaybeApplyGbLiveMediaFVideoConfig should log media apply failures but return success to live INVITE.",
    )

    reject(
        PROTOCOL_SOURCE,
        r"//\s*MaybeApplyGbLiveMediaFVideoConfig\s*\(",
        "GB live f= encode apply call should not be commented out.",
    )

    print("PASS: gb live INVITE applies f= video parameters as best-effort before sender setup")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
