# 变更提案: issue38 最新评论跟进，迁移本地 GB/GAT 注册配置目录并补齐 GAT flash 接口

## 背景
- issue38 在 2026-03-25 的最新评论明确要求把本地 GB 配置从 `/userdata/conf/Config/gb28181.ini` 迁到 `/userdata/conf/Config/GB/gb28181.ini`。
- 同一条评论还要求新增 `/userdata/conf/Config/GB/gat1400.ini`，并让接口实现方式与 `GetGbRegisterConfig()` / `SetGbRegisterConfig()` 保持一致。
- 当前代码里只有 GB 的 flash 读写 / 显式重载接口，GAT1400 仍然依赖代码默认值和整体配置热加载，缺少对称的本地持久化入口。

## 目标
- 把本地 GB 注册配置切换到 `/userdata/conf/Config/GB/gb28181.ini`。
- 新增独立的 `/userdata/conf/Config/GB/gat1400.ini`，只持久化 `GatRegisterParam`。
- 补齐 `GetGatRegisterConfig()` / `SetGatRegisterConfig()` / `RestartGatRegisterService()`，让 GAT1400 也具备“写 flash”和“显式生效”两步语义。
- 兼容迁移旧的 `/userdata/conf/Config/gb28181.ini`，避免升级后直接丢失历史录参。

## 影响范围
- `App/Protocol/config/LocalConfigProvider.h`
- `App/Protocol/config/LocalConfigProvider.cpp`
- `App/Protocol/ProtocolManager.h`
- `App/Protocol/ProtocolManager.cpp`
- `helloagents/project.md`
- `helloagents/wiki/arch.md`
- `helloagents/wiki/modules/protocol.md`
- `helloagents/wiki/api.md`
- `helloagents/wiki/data.md`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
