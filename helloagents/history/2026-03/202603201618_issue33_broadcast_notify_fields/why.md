# 变更提案

## 背景
- issue #33 在首轮修复“重复 SIP 应答”后仍被同事 reopen，平台侧继续显示语音广播失败。
- 最新附件里的 `debug.log` 只有广播 `NOTIFY` 接收和 `200 OK` 发送日志，没有出现后续 `INVITE`，说明失败点仍停留在广播通知应答阶段。

## 根因判断
- `CGB28181XmlParser::UnPackBroadcastNotify()` 的 `code` 参数按值传递，导致解出的广播目标编号没有写回 `NotifyInfo.GBCode`。
- 同一函数没有把 `broadcast_notify_t.SN` 回填到 `sn`，最终广播响应里的 `SN` 固定为 `0`。
- 原实现还把 `SourceID` 当成 `code` 来源，而广播响应应返回被叫设备自身的 `TargetID`。

## 成功标准
- 广播 `NOTIFY` 解析后能把原始 `SN` 和 `TargetID` 正确带入 `PackResponse("Broadcast", ...)`。
- `ProtocolManager` 建立广播会话时不再出现空 `gb_code`，即使上层传入为空也能回退到 `TargetID`/本机设备 ID。
- 隔离交叉编译通过，不影响当前机器上其他 SoC 的构建环境。
