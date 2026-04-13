# 任务清单: issue 44 GB 报警 DeviceID 对齐当前 SIP 本端 ID

目录: `helloagents/history/2026-04/202604131152_issue44_gb_alarm_sip_local_id/`

---

## 1. 根因收口
- [√] 1.1 基于 `245.pcap` 复核当前 SIP 会话里设备真实连接的本端 ID，确认报警 XML 应与 SDK 当前 `local_code` 对齐，而不是仅按 `string_code` 或订阅回调缓存取值

## 2. 代码调整
- [√] 2.1 在 `third_party/platform_sdk_port/CommonFile/CommonLib/GB28181ClientSDK.h` 与 `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/*` 中增加当前 `local_code` 读取接口
- [√] 2.2 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 的 `NotifyGbAlarm()` 中改为优先读取 SDK 当前本端 SIP ID，再归一化报警 `DeviceID/AlarmID`
- [√] 2.3 删除 `ProtocolManager` 中仅为报警补丁引入的订阅回调缓存字段 `m_gb_runtime_device_id`

## 3. 知识库同步
- [√] 3.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 3.2 更新 `helloagents/wiki/modules/protocol.md`
- [√] 3.3 更新 `helloagents/CHANGELOG.md`
- [√] 3.4 更新 `helloagents/history/index.md`

## 4. 验证
- [√] 4.1 执行 `git -C /home/jerry/silver/rk_gb diff --check`
- [√] 4.2 对 `rk_gb/App/Protocol/ProtocolManager.cpp` 与新增 SDK getter 相关实现执行编译级语法校验

## 执行结果
- `245.pcap` 显示当前 SIP 会话里 `Keepalive`、`SUBSCRIBE` 查询体、SIP `From/Contact` 使用的是 `35010101001320124935`，而报警 `NOTIFY` XML 的 `<DeviceID>` 仍是 `C0440326101000000010`；因此本轮将“报警应跟谁对齐”的根因收口为“应跟 SDK 当前真实连接的 SIP 本端 ID 对齐”。
- `GB28181ClientSDK` / `CGBClientImpl` 现新增 `GetLocalGbCode()`，直接暴露 SDK 当前 `m_xml_parser->m_local_code`；`ProtocolManager::NotifyGbAlarm()` 发送前优先读取这份值，再归一化 `DeviceID/AlarmID`。
- 已删除 `ProtocolManager` 中之前为 issue 44 临时加的 `m_gb_runtime_device_id` 订阅回调缓存字段，避免继续依赖回调时序或多维护一份运行态 ID。
- 验证完成：`git diff --check` 通过；基于 `cmake-build/App/CMakeFiles/dgiot.dir/flags.make` 的 include/define 映射，对 `ProtocolManager.cpp`、`GB28181ClientSDK.cpp`、`GBClientImpl.cpp` 执行了宿主机 `g++ -std=gnu++11 -fsyntax-only` 校验，新增改动无编译错误。宿主机编译 `GBClientImpl.cpp` 仍需 `-fpermissive` 才能压下仓库原有的 `void* -> int` 精度告警；另外仍有既有 `GB_ID_LEN` 宏重定义、`strstream` 过时头、`PFHelper.h` typedef warning。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
