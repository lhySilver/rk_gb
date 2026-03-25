# 技术设计: issue38 最新评论跟进

## 实现策略
1. 在 `ProtocolManager::Start()` 真正拉起各协议链路前先执行一次 `ReloadExternalConfig()`，确保启动前会重新从 flash 刷新本地协议配置。
2. 重写 `SetGbRegisterConfig()`，只调用 `LocalConfigProvider::UpdateGbRegisterConfig()` 持久化 `gb28181.ini`，不再直接修改 `m_cfg`，也不再自动重启 GB 生命周期。
3. 重写 `GetGbRegisterConfig()`，每次都走 `LocalConfigProvider::LoadOrCreateGbRegisterConfig()`；文件缺失时自动生成默认配置后返回。
4. 新增 `RestartGbRegisterService()`，从 flash 重读注册配置，更新 `m_cfg.gb_register` 缓存，并根据 `ProtocolManager` 是否已启动、GB 生命周期是否已运行以及 `enabled` 值决定：
   - 未启动: 只刷新缓存
   - 已启动且新配置禁用: 停止 GB 生命周期
   - 已启动且新配置启用: 按需启动或重启 GB 生命周期
5. 同步更新知识库与变更记录，明确外部调用顺序已调整为“`SetGbRegisterConfig()` 落盘 -> `RestartGbRegisterService()` 生效”。

## 风险控制
- `ProtocolManager` 当前 `m_provider` 仍是 `LocalConfigProvider`，因此 `Start()` 前新增的 `ReloadExternalConfig()` 会确实回到 flash，而不是被其他远端配置源覆盖。
- `RestartGbRegisterService()` 只处理 `gb_register` 生命周期，不改广播、对讲、监听和 GAT1400 运行态，避免扩大影响面。
- 使用 `g++ -fsyntax-only` 对 `ProtocolManager.cpp` 做最小编译级校验，确认新增接口和控制流没有语法问题。
