# 变更提案: issue45 增加 GAT1400 在线状态查询接口

## 需求背景
issue 45 在原有“1400 enabled 控制注册/停服”基础上，最新评论补充要求：
- 在 `ProtocolManager` 增加一个供外部模块调用的接口，用来获取 GAT1400 在线状态。

当前代码里已经存在 `ProtocolManager::GetGbOnlineStatus()`，可供外部模块读取 GB28181 是否已注册到平台；但 GAT1400 侧只有 `GAT1400ClientService` 内部持有 `m_registered`，还没有对称的只读查询接口。

## 变更目标
1. 在 `ProtocolManager` 暴露 `GetGatOnlineStatus()`，供外部模块统一读取 1400 在线态。
2. 在 `GAT1400ClientService` 增加线程安全的只读状态接口，直接透传当前 `m_registered`。
3. 保持语义与 `GetGbOnlineStatus()` 对齐：只表示“是否已注册到平台”，不扩展为“服务线程是否启动”。

## 非目标
- 不修改 GAT1400 注册、注销、保活主链路。
- 不新增额外 observer、回调或状态缓存。
- 不把 1400 在线态接到 `Main.cpp` 或 UI 显示链路，本次只补外部模块可调用接口。
