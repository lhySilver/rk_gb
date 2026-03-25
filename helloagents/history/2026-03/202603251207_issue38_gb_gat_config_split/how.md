# 技术设计: issue38 本地 GB/GAT 注册配置拆分

## 实现策略
1. 在 `LocalConfigProvider` 中把本地配置目录统一切到 `/userdata/conf/Config/GB/`，并拆出两个独立文件：
   - `gb28181.ini` 只保存 6 个 GB 注册字段
   - `gat1400.ini` 只保存 `GatRegisterParam`
2. 为 `LocalConfigProvider` 新增 `BuildDefaultGatRegisterConfig()`、`LoadOrCreateGatRegisterConfig()`、`UpdateGatRegisterConfig()`，语义与 GB 侧保持一致。
3. 新增旧文件兼容迁移逻辑：
   - 新路径文件不存在时，先尝试读取旧的 `/userdata/conf/Config/gb28181.ini`
   - GB 继续读原有 6 个字段
   - GAT 尝试读取历史 `gat_*` 字段并生成新的 `gat1400.ini`
4. 在 `ProtocolManager` 中新增 `SetGatRegisterConfig()` / `GetGatRegisterConfig()` / `RestartGatRegisterService()`：
   - `Set*` 只写 flash
   - `Get*` 只读 flash
   - `Restart*` 在管理器已启动时复用 `m_gat_client->Reload(m_cfg, m_cfg.gb_register)` 让运行态生效
5. 同步更新知识库，明确当前本地配置路径、持久化边界和 GAT 显式重载语义。

## 风险控制
- 保持 `gat_upload`、`gat_capture` 仍走代码默认值或 HTTP 配置链路，不把 issue38 范围扩大到全部 1400 参数。
- 旧路径只作为迁移回读入口，不在新逻辑中继续写回，避免双写和来源混乱。
- 使用 `g++ -fsyntax-only` 验证 `LocalConfigProvider.cpp` 与 `ProtocolManager.cpp`，确保新增接口与迁移逻辑无语法问题。
