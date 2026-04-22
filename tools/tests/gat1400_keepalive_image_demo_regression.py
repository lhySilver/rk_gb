#!/usr/bin/env python3
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[2]
HEADER = (ROOT / "App/Protocol/gat1400/GAT1400ClientService.h").read_text(encoding="utf-8-sig", errors="ignore")
SOURCE = (ROOT / "App/Protocol/gat1400/GAT1400ClientService.cpp").read_text(encoding="utf-8-sig", errors="ignore")


def require(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is None:
        raise AssertionError(message)


def forbid(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is not None:
        raise AssertionError(message)


def main() -> int:
    require(
        HEADER,
        r"std::atomic<bool>\s+m_keepalive_demo_pending;",
        "GAT1400ClientService 应增加一次性心跳 demo 触发状态位。",
    )
    require(
        SOURCE,
        r'"/mnt/sdcard/test\.jpeg"',
        "demo 应使用 /mnt/sdcard/test.jpeg 作为全景图输入。",
    )
    require(
        SOURCE,
        r'"/mnt/sdcard/face\.jpeg"',
        "demo 应使用 /mnt/sdcard/face.jpeg 作为人脸图输入。",
    )
    require(
        SOURCE,
        r"FACE_DETECT_EVENT",
        "demo 图像和人脸子图都应标记为 FACE_DETECT_EVENT。",
    )
    require(
        SOURCE,
        r"m_keepalive_demo_pending\.store\(true\);",
        "注册成功后应重置首次心跳 demo 触发位。",
    )
    require(
        SOURCE,
        r"const\s+bool\s+should_post_demo\s*=\s*m_keepalive_demo_pending\.exchange\(false\);",
        "心跳成功后应以 exchange(false) 的方式保证 demo 只触发一次。",
    )
    require(
        SOURCE,
        r"if\s*\(should_post_demo\)\s*\{[^}]*PostKeepaliveImageDemo\(\)",
        "首次心跳成功分支应调用 PostKeepaliveImageDemo()。",
    )
    require(
        SOURCE,
        r"NotifyFaces\(face_list\)",
        "demo helper 应改为复用现有 NotifyFaces() 发送人脸通知。",
    )
    require(
        SOURCE,
        r"IMAGE_SENCE",
        "demo 人脸通知应携带全景图子图。",
    )
    forbid(
        SOURCE,
        r"PostImages\(image_list\)",
        "demo helper 不应继续直接调用 PostImages()。",
    )
    require(
        SOURCE,
        r"Width\s*=\s*1920;",
        "全景图宽度应固定为 1920。",
    )
    require(
        SOURCE,
        r"Height\s*=\s*1080;",
        "全景图高度应固定为 1080。",
    )

    print("PASS: gat1400 keepalive face demo is wired for one-shot post-register notify upload")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
