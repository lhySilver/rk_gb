# GB28181 主动对讲 INVITE Subject 顺序修复

## 任务类型
- 轻量迭代 / 缺陷修复

## 背景
- 平台反馈 `对讲失败.pcap` 中主动 `INVITE` 的 `Subject` 里设备 ID 顺序反了。
- 抓包中失败报文为 `Subject: 35010101001320124942:0,35010000034000062043:0`，平台随后回复 `503 Service unavailable`。
- 当前链路中 `35010000034000062043` 来自 Broadcast `SourceID`/邀请目标，`35010101001320124942` 为本端媒体设备 ID。

## 任务清单
- [√] 解析 `对讲失败.pcap`，确认失败 `INVITE` 的 `Subject` 顺序与平台反馈一致。
- [√] 定位 `StartBroadcastStreamRequest()` 到 `StreamRequestEx()` 的主动广播音频 INVITE 组包链路。
- [√] 新增静态回归测试，锁定主动广播 `Subject` 应为平台源 ID 在前、本端设备 ID 在后。
- [√] 调整 `StartBroadcastStreamRequest()` 传入 `GenerateSubjectPair()` 的 sender/receiver 顺序。
- [√] 运行新增回归与相邻本地 SIP endpoint 回归测试。

## 验证
- `python3 tools/tests/gb_broadcast_invite_subject_order_regression.py`
- `python3 tools/tests/gb_broadcast_invite_local_endpoint_regression.py`

## 结果
- 主动广播/对讲 `INVITE` 的 `Subject` 从本端设备 ID 在前调整为平台 Broadcast `SourceID` 对应目标 ID 在前。
- 与上一轮本地 SIP endpoint 修复保持兼容。
