# 任务清单

- [√] 梳理 `OnNotify()` 现有广播处理链路，拆分“事务 `200 OK`”与“独立广播应答 `MESSAGE`”发送逻辑
- [√] 复用 `StartLiveStreamRequest()` 的通用 `INVITE` 能力，按 `RequestType=kAudioStream` 实现广播主动建链
- [√] 调整 `ProtocolManager` 广播状态机，改为“notify -> response message -> active invite -> media -> bye”
- [√] 校正广播 SDP/传输方向，确保广播桥按“设备收流”语义应用参数
- [√] 补齐广播全链路日志，并增加失败分类日志
- [√] 使用隔离目录完成交叉编译验证
- [√] 同步 `helloagents` 知识库并迁移方案包到 `history/`
