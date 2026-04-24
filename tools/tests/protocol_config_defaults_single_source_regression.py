#!/usr/bin/env python3
from pathlib import Path
import re


ROOT = Path(__file__).resolve().parents[2]
CONFIG_HEADER = (ROOT / "App/Protocol/config/ProtocolExternalConfig.h").read_text(
    encoding="utf-8-sig",
    errors="ignore",
)
LOCAL_PROVIDER_SOURCE = (ROOT / "App/Protocol/config/LocalConfigProvider.cpp").read_text(
    encoding="utf-8-sig",
    errors="ignore",
)


def require(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is None:
        raise AssertionError(message)


def require_count(text: str, needle: str, expected: int, message: str) -> None:
    count = text.count(needle)
    if count != expected:
        raise AssertionError(f"{message} 当前数量={count}, 期望={expected}")


def main() -> int:
    combined = CONFIG_HEADER + "\n" + LOCAL_PROVIDER_SOURCE

    require(CONFIG_HEADER, r"kProtocolDefaultVersion", "协议配置默认版本应在头文件中提供单一常量。")
    require(CONFIG_HEADER, r"kGbDefaultManufacturer", "GB 默认厂商应在头文件中提供单一常量。")
    require(CONFIG_HEADER, r"kGbDefaultModel", "GB 默认型号应在头文件中提供单一常量。")

    require(
        CONFIG_HEADER,
        r"custom_protocol_version\(\s*kGbDefaultCustomProtocolVersion\s*\)",
        "GbRegisterParam 构造函数应使用默认自定义协议版本常量。",
    )
    require(
        CONFIG_HEADER,
        r"manufacturer\(\s*kGbDefaultManufacturer\s*\)",
        "GbRegisterParam 构造函数应使用默认厂商常量。",
    )
    require(
        CONFIG_HEADER,
        r"model\(\s*kGbDefaultModel\s*\)",
        "GbRegisterParam 构造函数应使用默认型号常量。",
    )
    require(
        CONFIG_HEADER,
        r"ProtocolExternalConfig\(\)\s*:\s*version\(\s*kProtocolDefaultVersion\s*\)",
        "ProtocolExternalConfig 构造函数应使用默认版本常量。",
    )
    require(
        LOCAL_PROVIDER_SOURCE,
        r"cfg\.version\s*=\s*protocol::kProtocolDefaultVersion\s*;",
        "InitDefaultLocalConfig 应使用默认版本常量。",
    )
    require(
        LOCAL_PROVIDER_SOURCE,
        r"m_cached_cfg\.version\s*=\s*protocol::kProtocolDefaultVersion\s*;",
        "LocalConfigProvider 构造函数刷新版本时应使用默认版本常量。",
    )
    require(
        LOCAL_PROVIDER_SOURCE,
        r"next\.version\s*=\s*protocol::kProtocolDefaultVersion\s*;",
        "PullLatest 刷新版本时应使用默认版本常量。",
    )
    require(
        LOCAL_PROVIDER_SOURCE,
        r"param\.manufacturer\s*=\s*protocol::kGbDefaultManufacturer\s*;",
        "BuildDefaultGbRegisterParam 应使用默认厂商常量。",
    )
    require(
        LOCAL_PROVIDER_SOURCE,
        r"param\.model\s*=\s*protocol::kGbDefaultModel\s*;",
        "BuildDefaultGbRegisterParam 应使用默认型号常量。",
    )

    require_count(combined, '"1.0.3"', 1, "默认版本字面值只能保留在单一常量定义处。")
    require_count(combined, '"CMIoT"', 1, "默认厂商字面值只能保留在单一常量定义处。")
    require_count(combined, '"C4611"', 1, "默认型号字面值只能保留在单一常量定义处。")

    print("PASS: protocol config defaults use single source constants")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
