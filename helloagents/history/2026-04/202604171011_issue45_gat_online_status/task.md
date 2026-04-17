# 任务清单: issue45 增加 GAT1400 在线状态查询接口

## 1. 最小回归
- [√] 1.1 新增回归脚本，先固定 `GetGatOnlineStatus()` / `IsRegistered()` 缺失时的失败行为

## 2. 代码实现
- [√] 2.1 在 `GAT1400ClientService` 增加线程安全的 `IsRegistered()` 只读接口
- [√] 2.2 在 `ProtocolManager` 增加 `GetGatOnlineStatus()`，向外透传 1400 在线态

## 3. 知识库与验证
- [√] 3.1 更新 `helloagents/wiki`、`CHANGELOG.md`、`history/index.md`
- [√] 3.2 执行回归与静态验证并记录结果

## 执行结果

- 已新增 `tools/tests/issue45_gat_online_status_regression.py`，并先在缺少接口的旧代码状态下跑出失败，再在实现后转绿。
- `ProtocolManager` 已新增 `GetGatOnlineStatus()`，内部直接透传 `m_gat_client->IsRegistered()`；未初始化、未注册或停服时统一返回 `false`。
- `GAT1400ClientService` 已新增线程安全的 `IsRegistered()`，语义严格等同于当前注册态 `m_registered`，不额外引入第二份状态缓存。
- fresh 验证已通过：`python3 tools/tests/issue45_gat_enabled_regression.py`、`python3 tools/tests/issue45_gat_online_status_regression.py`、`git diff --check` 和相关语法编译均通过；仅保留现有 `GB_ID_LEN` 重定义 warning。
