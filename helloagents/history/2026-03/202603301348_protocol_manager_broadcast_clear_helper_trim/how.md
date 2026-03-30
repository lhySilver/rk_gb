# 技术设计: ProtocolManager 私有广播清理 helper 瘦身

## 技术方案
- 从 `ProtocolManager.h` 删除 `ClearGbBroadcastSessionState()` 的私有声明。
- 从 `ProtocolManager.cpp` 删除其对应实现。

## 实现要点
- `ClearGbBroadcastSessionState()` 当前仅做：
  - 加锁 `m_gb_broadcast_mutex`
  - 将 `m_gb_broadcast_session` 重置为默认值
- 当前源码树内没有任何调用点；广播会话的重置逻辑已经在其它实际路径内直接展开，不依赖该 helper。

## 验证方案
- 全仓搜索 `ClearGbBroadcastSessionState`，确认删除后已无剩余引用。
- 尽量对 `ProtocolManager.cpp` 做最小语法检查；如环境头文件不完整，则记录阻塞原因。

## 验证结果
- `App/`、`doc/` 与 `helloagents/wiki/` 中已无 `ClearGbBroadcastSessionState` 活引用
- 本地尝试执行 `ProtocolManager.cpp` 的最小语法检查时，因缺少 `libavcodec/avcodec.h` 头文件而中止，已记录为环境阻塞
