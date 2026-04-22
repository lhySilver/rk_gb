# 任务清单: GB 广播主动 INVITE 使用运行态本地 SIP 地址

目录: `helloagents/plan/202604221113_gb_broadcast_invite_local_endpoint/`

---

## 1. 回归约束
- [√] 1.1 在 `tools/tests/gb_broadcast_invite_local_endpoint_regression.py` 中增加回归脚本，约束 SipSDK 暴露运行态本地地址 getter，且 `GBClientImpl.cpp` 的主动 `INVITE` 分支不再直接使用 `m_xml_parser->m_local_ip/m_port` 组 `From`，验证 why.md#需求-广播主动-invite-的-from-使用真实本地-sip-地址-场景-广播通知后设备主动发起-invite

## 2. 代码调整
- [√] 2.1 在 `third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/SipEventManager.h`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/client/SipClientImpl.*`、`third_party/platform_sdk_port/CommonFile/CommonLib/SipStackSDK.h`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/SipStackSDK.cpp` 中补齐运行态本地 SIP 地址 getter 透传链路，验证 why.md#需求-广播主动-invite-的-from-使用真实本地-sip-地址-场景-广播通知后设备主动发起-invite
- [√] 2.2 在 `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp` 中把广播主动 `INVITE` 的 `From` 地址收口为优先读取 SipSDK 运行态本地 `IP/Port`，缺失时回退旧值，验证 why.md#需求-广播主动-invite-的-from-使用真实本地-sip-地址-场景-广播通知后设备主动发起-invite

## 3. 安全检查
- [√] 3.1 复核新增 getter 只读、不触发额外网络动作，也不改动现有注册/心跳/消息发送参数

## 4. 文档更新
- [√] 4.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 4.2 更新 `helloagents/CHANGELOG.md`
- [√] 4.3 更新 `helloagents/history/index.md`

## 5. 测试
- [√] 5.1 执行 `python3 tools/tests/gb_broadcast_invite_local_endpoint_regression.py`
- [√] 5.2 执行 `git -C /home/jerry/silver/rk_gb diff --check`
- [√] 5.3 对受影响源文件执行宿主机 `g++ -std=gnu++11 -fsyntax-only` 语法校验

## 执行结果
- `tools/tests/gb_broadcast_invite_local_endpoint_regression.py` 已先失败后通过，先锁定缺少运行态 SIP endpoint getter，再确认 `GBClientImpl` 的主动 `INVITE` 分支改为优先读取 `m_sip_client->GetLocalIp()/GetLocalPort()`。
- `SipEventManager -> CSipClientImpl -> SipUserAgentClient` 当前已补齐只读 getter 透传链路；运行态真实监听地址仍由 `SipEventManager` 维护，不改现有注册和消息发送主流程。
- `CGBClientImpl::StreamRequestEx()` 在广播主动 `INVITE` 的 `request_id` 分支中，只有当运行态本地 SIP `IP/Port` 有效时才覆盖旧值；否则继续回退 `m_xml_parser->m_local_ip/m_port`，保证改动面只落在本轮根因点。
- 验证完成：`python3 tools/tests/gb_broadcast_invite_local_endpoint_regression.py` 通过；`git diff --check` 通过；`SipStackSDK.cpp`、`SipClientImpl.cpp`、`GBClientImpl.cpp` 的宿主机 `g++ -std=gnu++11 -fpermissive -fsyntax-only` 语法校验均通过。
