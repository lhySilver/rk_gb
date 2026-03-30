# 需求说明: ProtocolManager 无调用转发方法瘦身

## 背景
- 用户要求继续“瘦身”，优先删除不会改变当前行为的死代码和空壳包装。
- 上一轮已经删除 `GBManager_Start()` / `GBManager_Stop()` / `OnGbConfigChanged()` 这类无调用壳层，当前 `ProtocolManager` 中仍有几处同类方法残留。

## 目标
- 删除 `ProtocolManager` 中当前树内无任何调用、仅做成员转发的公开方法：
  - `PushListenAudioFrame()`
  - `ApplyGbBroadcastSdpOffer()`
  - `SetGbBroadcastPcmCallback()`
- 顺手删除 `ProtocolManager.cpp` 中两处重复的静态 helper 前置声明，继续压缩无意义编译内容。

## 约束
- 不改协议逻辑、不改运行时行为。
- 不触碰 `cmake-build/` 生成产物。
- 同步更新 `CHANGELOG.md`、`history/index.md` 并归档方案包。
