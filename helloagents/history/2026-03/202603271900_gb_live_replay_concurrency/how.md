# 技术设计: GB 实时预览与录像回放并发

## 技术方案

### 核心技术
- C++11
- 现有 `GB28181RtpPsSender`
- 现有 `ProtocolManager` live/replay 会话模型

### 实现要点
- 在 `ProtocolManager` 内维护两套 sender 运行态:
  - live: sender + ssrc + local port
  - replay: sender + ssrc + local port
- `ReconfigureGbLiveSender` 改为按 `StreamRequestType` 选择目标 sender，并只重配目标 sender。
- `BuildGbResponseMediaInfo` 改为按 `requestType` 读取对应 sender 的端口和 SSRC，避免 live/replay 应答串用。
- `HandleGbStopStreamRequest` 改为按 handle 精确匹配 live/replay/talk/broadcast，`handle == NULL` 时仅用于全局 stop 场景。
- `StartGbReplaySession` 不再为了建链主动停止 live，会话替换仅限定在 replay/download 内部。

## 架构决策 ADR
### ADR-20260327-01: 采用双 sender 隔离 live 与 replay 媒体链路
**上下文:** 当前 live 与 replay 共用一个 RTP sender，任何一方重配目标 IP/端口或关闭 socket 都会影响另一方。
**决策:** 保持现有单 `ProtocolManager` 模型不变，只将媒体发送运行态拆成 live/replay 两套独立 sender。
**理由:** 改动面小，能覆盖当前“实时预览 + 回放/下载并发”需求，不必重写 GB 会话管理。
**替代方案:** 引入统一媒体会话表和多实例 sender 调度器 → 拒绝原因: 当前需求只要求 live 与 replay/download 并发，完整会话框架重构收益不足。
**影响:** 需要在应答、ACK、停流、回放发帧路径中区分 sender，但不影响外部接口。

## 安全与性能
- **安全:** 不新增外部输入面，仍沿用现有 RTP/PS 发送流程
- **性能:** 新增一个 replay sender 实例，内存与 socket 开销可控；live 与 replay 解耦后可避免频繁互相重建会话

## 测试与部署
- **测试:** 编译验证；人工验证 live + playback/download 并发、独立 stop、独立 ACK
- **部署:** 无额外部署步骤，按现有流程编译替换即可
