#!/usr/bin/env python3
from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[2]
SIP_EVENT_HEADER = (ROOT / "third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/SipEventManager.h").read_text(encoding="utf-8-sig", errors="ignore")
SIP_CLIENT_HEADER = (ROOT / "third_party/platform_sdk_port/CommonLibSrc/SipSDK/client/SipClientImpl.h").read_text(encoding="utf-8-sig", errors="ignore")
SIP_CLIENT_SOURCE = (ROOT / "third_party/platform_sdk_port/CommonLibSrc/SipSDK/client/SipClientImpl.cpp").read_text(encoding="utf-8-sig", errors="ignore")
SIP_STACK_HEADER = (ROOT / "third_party/platform_sdk_port/CommonFile/CommonLib/SipStackSDK.h").read_text(encoding="utf-8-sig", errors="ignore")
SIP_STACK_SOURCE = (ROOT / "third_party/platform_sdk_port/CommonLibSrc/SipSDK/SipStackSDK.cpp").read_text(encoding="utf-8-sig", errors="ignore")
GB_CLIENT_SOURCE = (ROOT / "third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp").read_text(encoding="utf-8-sig", errors="ignore")


def require(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is None:
        raise AssertionError(message)


def reject(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is not None:
        raise AssertionError(message)


def main() -> int:
    require(
        SIP_EVENT_HEADER,
        r"const\s+char\s*\*\s+GetLocalIp\(\)\s+const",
        "SipEventManager 应暴露运行态本地 SIP IP getter。",
    )
    require(
        SIP_CLIENT_HEADER,
        r"const\s+char\s*\*\s+GetLocalIp\(\)\s+const",
        "CSipClientImpl 应透传本地 SIP IP getter。",
    )
    require(
        SIP_CLIENT_HEADER,
        r"uint16_t\s+GetLocalPort\(\)\s+const",
        "CSipClientImpl 应透传本地 SIP port getter。",
    )
    require(
        SIP_STACK_HEADER,
        r"const\s+char\s*\*\s+GetLocalIp\(\)\s+const",
        "SipUserAgentClient 应暴露本地 SIP IP getter。",
    )
    require(
        SIP_STACK_HEADER,
        r"uint16_t\s+GetLocalPort\(\)\s+const",
        "SipUserAgentClient 应暴露本地 SIP port getter。",
    )
    require(
        SIP_CLIENT_SOURCE,
        r"return\s+m_event_manager->GetLocalIp\(\);",
        "CSipClientImpl::GetLocalIp() 应直接透传 SipEventManager 的运行态本地 IP。",
    )
    require(
        SIP_CLIENT_SOURCE,
        r"return\s+m_event_manager->GetLocalPort\(\);",
        "CSipClientImpl::GetLocalPort() 应直接透传 SipEventManager 的运行态本地端口。",
    )
    require(
        SIP_STACK_SOURCE,
        r"SipUserAgentClient::GetLocalIp\(\)\s+const\s*\{\s*return\s+m_client_impl->GetLocalIp\(\);\s*\}",
        "SipUserAgentClient::GetLocalIp() 应透传到底层 client impl。",
    )
    require(
        SIP_STACK_SOURCE,
        r"SipUserAgentClient::GetLocalPort\(\)\s+const\s*\{\s*return\s+m_client_impl->GetLocalPort\(\);\s*\}",
        "SipUserAgentClient::GetLocalPort() 应透传到底层 client impl。",
    )
    require(
        GB_CLIENT_SOURCE,
        r"m_sip_client->GetLocalIp\(\)",
        "GBClientImpl 主动 INVITE 组包时应优先读取运行态本地 SIP IP。",
    )
    require(
        GB_CLIENT_SOURCE,
        r"m_sip_client->GetLocalPort\(\)",
        "GBClientImpl 主动 INVITE 组包时应优先读取运行态本地 SIP 端口。",
    )
    reject(
        GB_CLIENT_SOURCE,
        r'from_builder\s*<<\s*"sip:"\s*<<\s*m_xml_parser->m_local_code\s*<<\s*"@"\s*<<\s*m_xml_parser->m_local_ip\s*<<\s*":"\s*<<\s*m_xml_parser->m_port',
        "广播主动 INVITE 的 From 不应再直接使用 XML 启动参数里的本地 IP/Port。",
    )

    print("PASS: gb broadcast active INVITE prefers runtime SIP local endpoint")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
