#!/usr/bin/env python3
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[2]
SOURCE = (ROOT / "App/Media/VideoEncodeControl.cpp").read_text(
    encoding="utf-8-sig",
    errors="ignore",
)

RK_VIDEO_FUNCTIONS = [
    "rk_video_get_gop",
    "rk_video_set_gop",
    "rk_video_get_max_rate",
    "rk_video_set_max_rate",
    "rk_video_get_RC_mode",
    "rk_video_set_RC_mode",
    "rk_video_get_output_data_type",
    "rk_video_set_output_data_type",
    "rk_video_get_frame_rate",
    "rk_video_set_frame_rate",
    "rk_video_set_resolution",
]

CFG_VIDEO_FUNCTIONS = {
    "rk_video_get_gop": "gop",
    "rk_video_set_gop": "gop",
    "rk_video_get_max_rate": "bit_rate",
    "rk_video_set_max_rate": "bit_rate",
    "rk_video_get_output_data_type": "enc_type",
    "rk_video_set_output_data_type": "enc_type",
    "rk_video_get_frame_rate": "frmae_rate",
    "rk_video_set_frame_rate": "frmae_rate",
}


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return re.sub(r"//.*", "", text)


def first_function_body(name: str) -> str:
    match = re.search(rf"\bint\s+{name}\s*\([^)]*\)\s*\{{", SOURCE)
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
    for name in RK_VIDEO_FUNCTIONS:
        body = strip_comments(first_function_body(name))
        if re.search(r"\breturn\b", body) is None:
            raise AssertionError(
                f"{name} must return a deterministic status; empty int stubs create random apply results."
            )

    for name, field in CFG_VIDEO_FUNCTIONS.items():
        body = strip_comments(first_function_body(name))
        if "CFG_VIDEO" not in body:
            raise AssertionError(f"{name} should read/write CFG_VIDEO like Main.cpp.")
        if f".{field}" not in body:
            raise AssertionError(f"{name} should use VideoConf_S.chan[].{field}.")

    print("PASS: VideoEncodeControl rk_video compatibility functions return deterministic status")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
