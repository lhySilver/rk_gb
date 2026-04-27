#!/usr/bin/env python3
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[2]
PROTOCOL_SOURCE = ROOT / "App/Protocol/ProtocolManager.cpp"
VIDEO_OSD_SOURCE = ROOT / "App/Media/VideoOsdControl.cpp"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def main() -> int:
    protocol = PROTOCOL_SOURCE.read_text(encoding="utf-8-sig", errors="ignore")
    video_osd = VIDEO_OSD_SOURCE.read_text(encoding="utf-8-sig", errors="ignore")

    require(
        "ConvertGbOsdTextToUtf8" in protocol,
        "ProtocolManager.cpp should define a GB OSD text conversion helper.",
    )
    require(
        "RK_encode_gbk_to_utf8" in protocol,
        "GB OSD conversion should reuse the existing RK GBK-to-UTF-8 implementation.",
    )
    require(
        re.search(
            r"const\s+std::string\s+nextText\s*=\s*ConvertGbOsdTextToUtf8\s*\(\s*"
            r"NormalizeGbOsdTextTemplate\s*\(",
            protocol,
            re.MULTILINE,
        )
        is not None,
        "OSDConfig Item/Text should be normalized and converted before item.text is assigned.",
    )
    require(
        "RK_encode_gbk_to_utf8" not in video_osd
        and "ConvertGbOsdTextToUtf8" not in video_osd,
        "VideoOsdControl.cpp should receive UTF-8 text and stay unaware of GB platform encoding.",
    )

    print("PASS: GB OSD text is converted to UTF-8 before entering VideoOsdState")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
