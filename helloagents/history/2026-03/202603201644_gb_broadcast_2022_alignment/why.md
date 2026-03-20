# 变更提案

## 背景
- issue #33 当前暴露的不只是广播通知字段错误，更是 GB/T 28181-2022 第 9.12 节广播链路与现有实现模型不一致。
- 现有实现是“平台发 `MESSAGE Notify` -> 设备在同一事务 `200 OK` 中携带业务 XML -> 设备等待平台后续 `INVITE`”。
- 2022 版标准要求的是“平台发 `MESSAGE Notify` -> 设备先回纯 SIP `200 OK` -> 设备再单独发广播应答 `MESSAGE` -> 平台回 `200 OK` -> 设备主动发 `INVITE` 建立广播会话”。

## 根因判断
- `CGBClientImpl::OnNotify()` 当前把 `Broadcast` 业务应答 XML 直接塞进 `Response()`，没有拆分成“事务 `200 OK` + 独立业务 `MESSAGE`”。
- 当前广播会话依赖 `OnStreamRequest()` 被动接收平台发来的 `INVITE`，没有走“设备主动 `INVITE`”能力。
- 现有 `GB28181ClientSDK` 只暴露了通用 `StartLiveStreamRequest()`，没有为广播单独封装“先业务应答、再主动建链”的业务入口。

## 成功标准
- 广播 `Notify` 收到后，设备先返回纯 SIP `200 OK`，不在事务响应里携带业务 XML。
- 设备随后独立发送一条 `Broadcast` 应答 `MESSAGE`，平台对其返回 `200 OK`。
- 广播媒体会话由设备侧主动 `INVITE` 发起，设备收到 `200 OK` 后 `ACK`，再进入广播桥收流/播音。
- 广播流程日志能明确区分 `notify-200`、`broadcast-message`、`invite`、`ack`、`bye` 五个阶段。

## 方案选择
- 推荐方案: 完整对齐 GB/T 28181-2022 第 9.12 节广播流程。
- 不推荐方案: 继续修补当前“平台主动 `INVITE`”旧流程。该方案改动小，但无法保证与 2022 版标准平台互通。
