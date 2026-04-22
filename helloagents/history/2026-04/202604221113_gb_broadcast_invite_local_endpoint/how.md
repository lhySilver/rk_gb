# 技术设计: GB 广播主动 INVITE 使用运行态本地 SIP 地址

## 技术方案
### 核心技术
- C++11
- 现有 SipSDK `SipEventManager -> CSipClientImpl -> SipUserAgentClient` 调用链
- GB28181 SDK `CGBClientImpl::StreamRequestEx()`

### 实现要点
- 在 `CSipEventManager` 上暴露 `GetLocalIp()`，复用其已缓存的真实监听地址。
- 在 `CSipClientImpl` 和 `SipUserAgentClient` 上增加只读透传接口，保持调用方向单向，不改已有注册/发包语义。
- 在 `CGBClientImpl::StreamRequestEx()` 的 `request_id` 分支中，组主动 `INVITE` 的 `From` 时优先取 `m_sip_client` 的运行态本地 `IP/Port`；仅当运行态值不可用时回退到 `m_xml_parser->m_local_ip/m_port`。

## 架构决策 ADR
### ADR-20260422-01: 广播主动 INVITE 的本地地址从 SipSDK 运行态读取
**上下文:** `ProtocolManager` 启动 GB SDK 时明确传入 `"0.0.0.0", 0` 作为本地 SIP 监听占位参数。`GBClientImpl::StreamRequestEx()` 在广播主动 `INVITE` 分支里直接把这组启动参数拼进 `From`，导致抓包中出现 `0.0.0.0:0`。  
**决策:** 不修改上层启动参数语义，而是在 SipSDK 暴露当前真实监听 `IP/Port` 的只读 getter，由 `GBClientImpl` 在主动 `INVITE` 组包时优先读取这组运行态值。  
**理由:** 改动面最小，根因最直接；同时不会影响当前 `ProtocolManager` 用 `0` 端口触发随机本地 SIP 监听的既有策略。  
**替代方案:** 直接改 `ProtocolManager::Start()` 传入固定本地 IP/Port → 拒绝原因: 会把当前随机端口策略和启动阶段配置边界一起改动，超出本轮修复范围。  
**影响:** SipSDK 头文件多出 2 个轻量 getter；`GBClientImpl` 的主动 `INVITE` `From` 将对齐运行态 SIP 监听地址。

## 安全与性能
- **安全:** 不引入新外部输入，不修改网络重试和认证逻辑。
- **性能:** 只是读取内存中已缓存的本地地址，不增加额外网络操作。

## 测试与部署
- **测试:** 新增 `tools/tests/gb_broadcast_invite_local_endpoint_regression.py` 做回归检查；随后执行 `git diff --check` 和受影响源文件 `g++ -fsyntax-only`。
- **部署:** 无额外部署动作，保持现有构建参数不变。
