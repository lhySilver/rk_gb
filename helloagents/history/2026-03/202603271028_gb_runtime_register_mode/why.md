# 变更提案: GB 注册模式从编译期开关改为运行时配置

## 背景
- 当前标准国标注册和零配置重定向注册通过 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 编译期开关切换。
- 每次切换模式都需要重新编译，联调和现场切换成本高，也不利于通过 flash / HTTP 配置统一控制。

## 目标
1. 在 `gb28181.ini` 中新增运行时控制字段，直接决定当前走标准国标还是零配置注册链路。
2. 保持 `zero_config.ini` 只承载零配置专属参数，不承担模式判定职责。
3. 让 `ProtocolManager`、`LocalConfigProvider`、`HttpConfigProvider` 和 `GBClientImpl` 都基于同一配置字段做运行时判断。

## 成功标准
- 无需重新编译即可通过配置切换 `standard` / `zero_config` 两种注册模式。
- `register_mode=standard` 时不要求 `zero_config.ini` 存在。
- `register_mode=zero_config` 时继续要求 `zero_config.ini` 和零配置关键字段有效。
- 原有标准国标注册行为不回归，零配置重试状态机继续可用。
