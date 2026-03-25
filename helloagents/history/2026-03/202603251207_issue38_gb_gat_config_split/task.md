# 任务清单: issue38 本地 GB/GAT 注册配置拆分

- [√] 1. 将 `LocalConfigProvider` 的本地注册配置目录迁移到 `/userdata/conf/Config/GB/`，并新增 `gat1400.ini`
- [√] 2. 补齐 `GatRegisterParam` 的 `BuildDefault/LoadOrCreate/Update` flash 接口，以及旧 `gb28181.ini` 的兼容迁移逻辑
- [√] 3. 在 `ProtocolManager` 新增 `GetGatRegisterConfig()` / `SetGatRegisterConfig()` / `RestartGatRegisterService()`
- [√] 4. 同步更新 `helloagents` 知识库、`CHANGELOG` 与 `history/index`
- [√] 5. 使用 `git diff --check` 与 `g++ -fsyntax-only` 做最小静态验证
