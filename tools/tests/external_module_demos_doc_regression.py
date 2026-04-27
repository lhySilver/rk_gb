#!/usr/bin/env python3
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
DOC = ROOT / "helloagents/wiki/modules/external_module_demos.md"
API_DOC = ROOT / "helloagents/wiki/api.md"
OVERVIEW_DOC = ROOT / "helloagents/wiki/overview.md"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise AssertionError(message)


def main() -> int:
    text = DOC.read_text(encoding="utf-8-sig", errors="ignore")
    api = API_DOC.read_text(encoding="utf-8-sig", errors="ignore")
    overview = OVERVIEW_DOC.read_text(encoding="utf-8-sig", errors="ignore")

    required_sections = [
        "## 1. 设置标准 GB28181 注册参数",
        "## 2. 设置 GB 零配置串码和 MAC",
        "## 3. 设置 GAT1400 注册参数或停服",
        "## 4. 查询 GB28181 / GAT1400 在线状态",
        "## 5. 上报 GAT1400 结构化对象",
    ]
    for section in required_sections:
        require(section in text, f"missing demo section: {section}")

    required_snippets = [
        "SetGbRegisterConfig",
        "SetGbZeroConfig",
        "RestartGbRegisterService",
        "SetGatRegisterConfig",
        "RestartGatRegisterService",
        "GetGbOnlineStatus",
        "GetGatOnlineStatus",
        "NotifyGatFaces",
        "NotifyGatMotorVehicles",
        "NotifyGatNonMotorVehicles",
        "只表示写 flash 成功",
        "不新增编译目标",
        "不改 Makefile/CMake",
    ]
    for snippet in required_snippets:
        require(snippet in text, f"missing required snippet: {snippet}")

    require(
        "modules/external_module_demos.md" in api,
        "api.md should link to external_module_demos.md.",
    )
    require(
        "modules/external_module_demos.md" in overview,
        "overview.md should list external module demos.",
    )

    print("PASS: external module demo documentation is discoverable")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f"FAIL: {exc}")
        raise SystemExit(1)
