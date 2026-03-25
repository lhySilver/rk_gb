# 技术设计

## 方案
1. 在 `ProtocolExternalConfig::GbRegisterParam` 中增加 `enabled` 字段，补齐默认值。
2. 扩展 `LocalConfigProvider`，新增本地目录创建、INI 文件读取和写回逻辑，固定路径为 `/userdata/conf/Config/gb28181.ini`。
3. 在 `PullLatest()` 和 `PushApply()` 中保持“文件优先 + 成功校验后回写”的一致行为。
4. 在 `ProtocolManager` 中把 `enabled` 纳入配置 diff 与重载判定，禁用时跳过 `StartGbClientLifecycle()` / `RegisterGbClient()`。
5. 为保持配置模型一致性，同步兼容 `HttpConfigProvider` 的 `gb_register_enabled` 字段。

## 影响范围
- `App/Protocol/config/ProtocolExternalConfig.h`
- `App/Protocol/config/LocalConfigProvider.cpp`
- `App/Protocol/config/HttpConfigProvider.cpp`
- `App/Protocol/ProtocolManager.cpp`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
- `helloagents/wiki/data.md`
- `helloagents/wiki/modules/protocol.md`
- `helloagents/wiki/arch.md`

## 风险与规避
- 风险: 本地文件目录不存在或首次启动无配置文件。
- 规避: 启动时递归创建 `/userdata/conf/Config`，缺省自动落盘默认配置。
- 风险: 禁用 GB 后仍误触发旧的重注册逻辑。
- 规避: 在启动、重载、显式注册三个入口统一判断 `gb_register.enabled`。

## ADR
### ADR-20260320-issue31
- 决策: GB28181 接入参数以本地配置文件优先，缺省值只作为兜底，同时引入显式启停开关。
- 原因: 该需求明确要求设备脱离重新编译即可切换接入参数，且必须适配量产设备上的固定配置目录。
