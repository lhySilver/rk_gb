# 需求说明: ProtocolManager 私有广播清理 helper 瘦身

## 背景
- 用户要求继续“瘦身”，优先删除不影响当前行为的死代码。
- `ProtocolManager` 中仍有少量私有 helper 已经不再被任何代码路径调用。

## 目标
- 删除 `ProtocolManager` 中零调用的私有方法 `ClearGbBroadcastSessionState()`。

## 约束
- 不改协议行为，不调整广播会话状态机逻辑。
- 仅删除未调用的私有 helper，并同步更新变更记录与历史索引。
