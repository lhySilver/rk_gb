# 任务清单: GB28181 远程升级与远程重启修复

目录: `helloagents/plan/202603210923_gb_upgrade_reboot_fix/`

---

## 1. 协议链路修正
- [√] 1.1 在 `App/Protocol/ProtocolManager.cpp` 中修正 GB 升级状态管理和本地执行入口，验证 why.md#需求-远程升级结果语义修正、why.md#需求-远程升级接入本地执行链路
- [-] 1.2 在 `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp` 中修正 `DeviceUpgrade` 控制应答行为，验证 why.md#需求-远程升级结果语义修正，依赖任务1.1
  说明: 审核后确认现有 SDK 已满足“先 SIP 200 OK，再独立发送控制业务应答”的行为，本次问题根因在 `ProtocolManager` 提前上报最终升级成功，故未改 SDK。

## 2. 配置与能力补齐
- [√] 2.1 在 `App/Protocol/config/LocalConfigProvider.cpp` 中补齐 `gb_upgrade` / `gb_reboot` 本地配置读写和能力声明，验证 why.md#需求-远程重启与配置接入补齐
- [√] 2.2 在 `App/Protocol/config/HttpConfigProvider.cpp` 中补齐 `gb_upgrade` / `gb_reboot` HTTP 配置字段和能力声明，验证 why.md#需求-远程重启与配置接入补齐，依赖任务2.1

## 3. 安全检查
- [√] 3.1 执行安全检查，确认升级失败清理、结果上报时序和重启冷却逻辑没有退化

## 4. 文档更新
- [√] 4.1 更新 `helloagents/wiki/modules/protocol.md`
- [√] 4.2 更新 `helloagents/wiki/modules/gb28181-2022-baseline.md`
- [√] 4.3 更新 `helloagents/CHANGELOG.md`

## 5. 测试
- [√] 5.1 对改动文件执行最小交叉编译或静态验证，确认不污染其他 SoC 编译环境

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
