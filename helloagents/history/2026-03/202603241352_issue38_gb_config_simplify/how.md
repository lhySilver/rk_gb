# 技术设计: issue38 国标配置相关代码精简

## 设计思路
1. 在 `LocalConfigProvider` 中补一个面向 `GbRegisterParam` 的轻量静态入口，专门处理默认值、文件缺省创建、读取和更新。
2. `SetGbRegisterConfig()` 不再直接覆盖 `m_cfg.gb_register`，而是先落盘，再把运行态 `GB28181` 生命周期按变化结果做最小重载。
3. `GetGbRegisterConfig()` 不再依赖 `m_cfg` 是否已经初始化，而是每次从本地文件读取；文件不存在则先生成默认配置再返回。

## 关键决策
- 只把 issue38 明确要求的 6 个外部字段当作 `SetGbRegisterConfig()` 的可编辑集合：
  - `enabled`
  - `username`
  - `server_ip`
  - `server_port`
  - `device_id`
  - `password`
- `device_name`、`expires_sec` 等未在 issue38 中声明为外部可改字段，因此在 `SetGbRegisterConfig()` 中保留现有文件值，不做额外覆盖。
- 运行中生效只收口到 `GB` 注册生命周期重载，不触发整套 broadcast/listen/GAT 配置刷新，避免把 issue38 扩大成全量配置热更新。

## 风险与规避
- 风险: 落盘成功但运行态重启 GB 生命周期失败。
- 规避: 保持与当前 `ReloadExternalConfig()` 同级别的失败语义，返回错误码并打印明确日志，文件内容仍保留为用户最后一次设置值。

## 验证
- 检查 `GetGbRegisterConfig()` 在未 `Init()` 时仍能返回默认/文件值。
- 检查 `SetGbRegisterConfig()` 会写回 `gb28181.ini`，并只改 issue38 要求的 6 个字段。
- 检查运行态调用 `SetGbRegisterConfig()` 后会按配置差异执行 `StopGbClientLifecycle()` / `StartGbClientLifecycle()`。
