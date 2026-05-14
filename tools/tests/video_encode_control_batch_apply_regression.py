#!/usr/bin/env python3
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[2]
SOURCE = (ROOT / "App/Media/VideoEncodeControl.cpp").read_text(
    encoding="utf-8-sig",
    errors="ignore",
)


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return re.sub(r"//.*", "", text)


def function_body(name: str) -> str:
    match = re.search(
        rf"\b(?:static\s+)?(?:int|bool|void)\s+{name}\s*\([^)]*\)\s*\{{",
        SOURCE,
    )
    if match is None:
        raise AssertionError(f"{name} should exist in VideoEncodeControl.cpp")

    open_brace = SOURCE.find("{", match.end() - 1)
    depth = 0
    for index in range(open_brace, len(SOURCE)):
        if SOURCE[index] == "{":
            depth += 1
        elif SOURCE[index] == "}":
            depth -= 1
            if depth == 0:
                return SOURCE[open_brace + 1 : index]

    raise AssertionError(f"{name} body should be parseable")


def main() -> int:
    apply_body = strip_comments(function_body("ApplyVideoEncodeStreamConfig"))
    if "ApplyCfgVideoEncodeStreamConfig" not in apply_body:
        raise AssertionError(
            "ApplyVideoEncodeStreamConfig should batch CFG_VIDEO-backed fields before applying f=."
        )

    forbidden = [
        "rk_video_set_output_data_type",
        "rk_video_set_frame_rate",
        "rk_video_set_max_rate",
    ]
    for symbol in forbidden:
        if re.search(rf"\b{symbol}\s*\(", apply_body):
            raise AssertionError(
                f"ApplyVideoEncodeStreamConfig should not apply f= CFG_VIDEO fields through per-field {symbol}."
            )

    helper_body = strip_comments(function_body("ApplyCfgVideoEncodeStreamConfig"))
    if helper_body.count("g_configManager.getConfig(getConfigName(CFG_VIDEO)") != 1:
        raise AssertionError("CFG_VIDEO batch apply should read current config once.")
    if helper_body.count("g_configManager.setConfig(getConfigName(CFG_VIDEO)") != 1:
        raise AssertionError("CFG_VIDEO batch apply should write changed fields once.")
    for field in [".enc_type", ".frmae_rate", ".bit_rate"]:
        if field not in helper_body:
            raise AssertionError(f"CFG_VIDEO batch apply should compare and update {field}.")
    if not re.search(r"\bchanged\b", helper_body):
        raise AssertionError("CFG_VIDEO batch apply should skip setConfig when values already match.")

    print("PASS: VideoEncodeControl batches f= CFG_VIDEO encode parameters into one config apply")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
