# 变更提案: issue38 最新评论跟进，拆分 GB 注册配置读写与服务重启

## 背景
- issue38 前两轮修复已经让 `gb28181.ini` 收敛为 6 个注册字段，并让 `GetGbRegisterConfig()` / `SetGbRegisterConfig()` 走本地文件。
- 但当前 `SetGbRegisterConfig()` 仍会直接改 `m_cfg.gb_register` 并自动 `Stop/Start` GB 生命周期，这和同事在 issue38 于 2026-03-25 的最新评论不一致。
- 另外，`ProtocolManager::Start()` 依赖 `Init()` 阶段拉到的缓存，存在“先写 flash、后启动时仍沿用旧缓存”的风险。

## 目标
- 让 `GetGbRegisterConfig()` 只负责从 flash 读；无文件时补默认并返回。
- 让 `SetGbRegisterConfig()` 只负责写 flash，不直接改运行态。
- 新增单独的 `RestartGbRegisterService()`，由外部显式触发 GB 服务重载。
- 保证 `Init()` 后、`Start()` 前写入 flash 的注册配置，也会在真正启动时重新生效。

## 影响范围
- `App/Protocol/ProtocolManager.h`
- `App/Protocol/ProtocolManager.cpp`
- `helloagents/wiki/api.md`
- `helloagents/wiki/modules/protocol.md`
- `helloagents/wiki/data.md`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
