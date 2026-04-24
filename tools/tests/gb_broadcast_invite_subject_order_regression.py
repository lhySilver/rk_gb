#!/usr/bin/env python3
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[2]
GB_CLIENT_SOURCE = (ROOT / "third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp").read_text(
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
        GB_CLIENT_SOURCE,
        r"const\s+char\s*\*\s+subject_sender_id\s*=\s*target_id\s*;",
        "广播主动 INVITE 的 Subject 首段应使用平台 Broadcast SourceID 对应的 target_id。",
    )
    require(
        GB_CLIENT_SOURCE,
        r"const\s+char\s*\*\s+subject_receiver_id\s*=\s*request\.DeviceID\[0\]\s*!=\s*'\\0'\s*\?\s*request\.DeviceID\s*:\s*target_id\s*;",
        "广播主动 INVITE 的 Subject 第二段应使用本次媒体设备 ID，缺失时才回退 target_id。",
    )
    require(
        GB_CLIENT_SOURCE,
        r"StreamRequestEx\s*\(\s*&request\s*,\s*target_id\s*,\s*subject_sender_id\s*,\s*subject_receiver_id\s*,\s*result\s*,\s*stream_handle\s*\)",
        "StartBroadcastStreamRequest 应按 sender/receiver 顺序传入 Subject ID。",
    )
    reject(
        GB_CLIENT_SOURCE,
        r"const\s+char\s*\*\s+subject_sender_id\s*=\s*request\.DeviceID\[0\]\s*!=\s*'\\0'\s*\?\s*request\.DeviceID\s*:\s*target_id\s*;\s*return\s+StreamRequestEx\s*\(\s*&request\s*,\s*target_id\s*,\s*subject_sender_id\s*,\s*target_id",
        "广播主动 INVITE 不应再把本设备媒体 ID 放在 Subject 首段、平台 SourceID 放在第二段。",
    )

    print("PASS: gb broadcast active INVITE subject uses platform source before local device")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
