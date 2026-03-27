# 变更提案: GB 实时预览与录像回放并发

## 需求背景
当前 `ProtocolManager` 将 GB 实时预览与录像回放/下载复用同一个 RTP PS sender 和同一组媒体端口状态，导致任一媒体请求重配都会影响另一条会话。现有代码中，回放/下载启动前还会主动停止已有流，因此平台无法同时维持 GB 实时预览与 GB 录像回放/下载。

## 变更内容
1. 将 GB 实时预览与 GB 回放/下载的媒体发送状态拆分，避免共享 sender、SSRC、local port。
2. 调整停流与应答路径，使 stop/ack/200 OK 只作用于目标会话，不再误伤另一类媒体会话。
3. 保持现有“一路 live + 一路 replay/download”模型，不引入多路并发回放。

## 影响范围
- **模块:** `ProtocolManager`、GB RTP PS 发送链路、GB28181 知识库
- **文件:** `App/Protocol/ProtocolManager.h`、`App/Protocol/ProtocolManager.cpp`、`helloagents/wiki/modules/gb28181.md`、`helloagents/CHANGELOG.md`
- **API:** 无对外接口签名变化
- **数据:** 无持久化数据结构变化

## 核心场景

### 需求: 实时预览与录像回放并存
**模块:** GB28181
平台在设备已开启实时预览后，再发起录像回放或下载，请求应能成功建立且不影响原有实时预览。

#### 场景: 先预览后回放
设备正在发送 GB 实时预览流，平台再次发起回放请求。
- 实时预览保持发送
- 回放/下载成功建立独立媒体发送链路
- 回放控制仅作用于回放会话

#### 场景: 先回放后预览
设备正在执行 GB 回放或下载，平台再次发起实时预览请求。
- 回放/下载保持发送
- 实时预览成功建立独立媒体发送链路
- 实时预览 stop 不影响回放会话

### 需求: 停流精准命中
**模块:** GB28181
平台对指定 handle 发起停止请求时，只停止对应类型的媒体会话。

#### 场景: 停止实时预览
平台停止 live handle。
- live sender 关闭
- replay/download 会话保持发送

#### 场景: 停止回放
平台停止 replay/download handle。
- replay/download sender 关闭
- live 会话保持发送

## 风险评估
- **风险:** 媒体应答 SDP 使用错误的 local port/SSRC，导致平台建链异常
- **缓解:** 按请求类型绑定 sender 运行态端口和 SSRC，并分别覆盖 live/replay 的 200 OK 构建逻辑
