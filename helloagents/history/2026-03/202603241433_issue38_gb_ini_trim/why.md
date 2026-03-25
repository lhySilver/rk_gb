# 变更提案: issue38 最新评论跟进，收缩 gb28181.ini 字段

## 背景
- issue38 初版修复已经让 `GetGbRegisterConfig()` / `SetGbRegisterConfig()` 接入本地文件并支持 `init` 前访问与运行中生效。
- 但 `LocalConfigProvider` 仍会把大量 GAT、广播、对讲、升级等配置项一起写进 `/userdata/conf/Config/gb28181.ini`。
- 同事在 issue38 最新评论中明确要求：该文件当前只保留 6 个国标注册字段，其余项暂时不配置，统一写死在代码中。

## 目标
- 让 `gb28181.ini` 的生成和回写内容只包含 `enable`、`username`、`server_ip`、`server_port`、`device_id`、`password`。
- 其余协议配置项全部恢复为代码默认值，不再依赖本地 ini 持久化。

## 影响范围
- `App/Protocol/config/LocalConfigProvider.cpp`
- `helloagents/wiki/modules/protocol.md`
- `helloagents/CHANGELOG.md`
