# 变更提案: GB 广播主动 INVITE 使用运行态本地 SIP 地址

## 需求背景
`23.pcap` 显示当前广播通知后的主动音频 `INVITE` 被平台直接回复 `503 Service unavailable`。同一事务里，设备先正常回复 `Broadcast Notify` 的 `200 OK` 和业务 `Response`，随后主动发起新的 `INVITE` 时，`From` 头变成了 `sip:35010101001320124942@0.0.0.0:0`。这说明问题不在广播通知事务本身，而在主动 `INVITE` 组包阶段错误复用了启动时传给 SDK 的占位本地地址。

## 变更内容
1. 给 SipSDK 补齐当前监听成功后的运行态本地 SIP `IP/Port` 只读获取接口。
2. 广播主动 `INVITE` 组 `From` 时优先使用运行态本地 SIP 地址，拿不到时再回退旧的 XML 启动参数。
3. 增加最小回归脚本，约束主动 `INVITE` 不再直接使用 `m_local_ip/m_port` 这组占位值。

## 影响范围
- **模块:** GB28181、SipSDK、知识库
- **文件:** `third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/SipEventManager.h`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/client/SipClientImpl.*`、`third_party/platform_sdk_port/CommonFile/CommonLib/SipStackSDK.h`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/SipStackSDK.cpp`、`third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp`
- **API:** SipSDK 内部新增运行态本地地址 getter
- **数据:** 无

## 核心场景

### 需求: 广播主动 INVITE 的 From 使用真实本地 SIP 地址
**模块:** GB28181 / SipSDK
当平台下发广播通知后，设备会主动发起新的音频 `INVITE`，该请求的 `From` 必须与当前 SIP 客户端真实监听地址一致，不能继续使用启动参数里的 `0.0.0.0:0`。

#### 场景: 广播通知后设备主动发起 INVITE
前置条件:
- SIP 客户端已完成启动并建立真实监听地址
- `ProtocolManager::HandleGbBroadcastNotifyResponse()` 触发 `StartBroadcastStreamRequest()`
- 预期结果1: `GBClientImpl` 构造主动 `INVITE` 的 `From` 时优先读取 SipSDK 运行态本地 `IP/Port`
- 预期结果2: 若运行态值缺失，仍保留旧回退逻辑，不扩大其他流程改动面
- 预期结果3: 回归脚本能识别 getter 透传链路和 `GBClientImpl` 的新取值方式

## 风险评估
- **风险:** 若 SipSDK getter 取值来源不对，可能把广播主动 `INVITE` 继续绑到旧占位地址，或者把非广播流程一并牵动。
- **缓解:** 只新增只读 getter，不修改 `REGISTER`、`MESSAGE` 等现有发送路径；`GBClientImpl` 仅在广播主动 `INVITE` 这条 `request_id` 分支切换取值，并保留旧值回退。
