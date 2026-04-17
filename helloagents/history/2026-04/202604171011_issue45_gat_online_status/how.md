# 技术设计: issue45 增加 GAT1400 在线状态查询接口

## 技术方案
1. 在 `ProtocolManager.h/.cpp` 新增 `bool GetGatOnlineStatus() const;`。
2. 在 `GAT1400ClientService.h/.cpp` 新增 `bool IsRegistered() const;`，内部在 `m_state_mutex` 保护下返回 `m_registered`。
3. `ProtocolManager::GetGatOnlineStatus()` 直接复用 `m_gat_client->IsRegistered()`；若服务对象不存在，则返回 `false`。
4. 新增最小回归脚本，覆盖：
   - `ProtocolManager` 对外声明了 `GetGatOnlineStatus()`
   - `ProtocolManager.cpp` 已通过 `m_gat_client->IsRegistered()` 透传 1400 在线态
   - `GAT1400ClientService` 已暴露 `IsRegistered()` 并返回 `m_registered`
5. 同步知识库、`CHANGELOG.md` 和 `history/index.md`。

## 设计取舍
- **推荐方案:** 复用 `GAT1400ClientService` 的现有 `m_registered`。
  - 优点: 最小改动、语义最准确、与 GB 在线态接口对称。
  - 缺点: 只能表达“已注册”，不能表达“已启动但未注册”。
- **未采用方案:** 在 `ProtocolManager` 再维护一份 `m_gat_client_registered`。
  - 原因: 会引入重复状态源，和当前 GAT 生命周期实现脱节。

## 验证计划
- 先运行新回归脚本，确认在旧代码上失败。
- 实现接口后重新运行回归脚本至通过。
- 额外执行 `git diff --check` 与相关文件语法校验，避免引入格式或编译级回退。
