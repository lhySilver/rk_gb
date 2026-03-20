# 变更历史索引

## 索引

| 时间戳 | 功能名称 | 类型 | 状态 | 方案包路径 |
|--------|----------|------|------|------------|
| 202603201728 | issue33_broadcast_active_invite_sdp | 修复 | ✅已完成 | [2026-03/202603201728_issue33_broadcast_active_invite_sdp](2026-03/202603201728_issue33_broadcast_active_invite_sdp/) |
| 202603201705 | gb28181_2022_baseline | 知识库 | ✅已完成 | [2026-03/202603201705_gb28181_2022_baseline](2026-03/202603201705_gb28181_2022_baseline/) |
| 202603201644 | gb_broadcast_2022_alignment | 方案/实现 | ✅已完成 | [2026-03/202603201644_gb_broadcast_2022_alignment](2026-03/202603201644_gb_broadcast_2022_alignment/) |
| 202603201618 | issue33_broadcast_notify_fields | 修复 | ✅已完成 | [2026-03/202603201618_issue33_broadcast_notify_fields](2026-03/202603201618_issue33_broadcast_notify_fields/) |
| 202603201544 | gb_broadcast_session_closure | 修复 | ✅已完成 | [2026-03/202603201544_gb_broadcast_session_closure](2026-03/202603201544_gb_broadcast_session_closure/) |
| 202603201516 | issue33_gb_broadcast | 修复 | ✅已完成 | [2026-03/202603201516_issue33_gb_broadcast](2026-03/202603201516_issue33_gb_broadcast/) |
| 202603201130 | issue32_gb_control_config | 修复 | ✅已完成 | [2026-03/202603201130_issue32_gb_control_config](2026-03/202603201130_issue32_gb_control_config/) |
| 202603201017 | issue31_gb_local_config | 修复 | ✅已完成 | [2026-03/202603201017_issue31_gb_local_config](2026-03/202603201017_issue31_gb_local_config/) |
| 202603191035 | issue30_gb_time_record | 修复 | ✅已完成 | [2026-03/202603191035_issue30_gb_time_record](2026-03/202603191035_issue30_gb_time_record/) |
| 202603161102 | issue27_gb_live_tcp | 修复 | ✅已完成 | [2026-03/202603161102_issue27_gb_live_tcp](2026-03/202603161102_issue27_gb_live_tcp/) |

## 按月归档

### 2026-03

- [202603201728_issue33_broadcast_active_invite_sdp](2026-03/202603201728_issue33_broadcast_active_invite_sdp/) - 修复广播主动 INVITE 收到有效 200 OK 后仍被误判为 SDP 解析失败的问题
- [202603201705_gb28181_2022_baseline](2026-03/202603201705_gb28181_2022_baseline/) - 新增 GB/T 28181-2022 调试基线知识库，统一注册、保活、流媒体、广播和校时的排查口径
- [202603201644_gb_broadcast_2022_alignment](2026-03/202603201644_gb_broadcast_2022_alignment/) - 按 GB/T 28181-2022 第 9.12 节重构广播通知、业务应答和设备主动建链流程
- [202603201618_issue33_broadcast_notify_fields](2026-03/202603201618_issue33_broadcast_notify_fields/) - 修复广播通知响应中的 `SN/DeviceID` 字段丢失，避免平台继续判失败
- [202603201544_gb_broadcast_session_closure](2026-03/202603201544_gb_broadcast_session_closure/) - 补齐 GB 广播通知会话管理、真实应答地址和 BYE 清理复位
- [202603201516_issue33_gb_broadcast](2026-03/202603201516_issue33_gb_broadcast/) - 修复 GB 广播通知重复 SIP 应答导致平台侧判定设备未响应
- [202603201130_issue32_gb_control_config](2026-03/202603201130_issue32_gb_control_config/) - 补齐 GB 远程重启冷却保护与画面翻转配置主链路
- [202603201017_issue31_gb_local_config](2026-03/202603201017_issue31_gb_local_config/) - 将 GB28181 接入参数切换为本地配置文件读取并支持缺省自动生成
- [202603191035_issue30_gb_time_record](2026-03/202603191035_issue30_gb_time_record/) - 补齐 GB 注册时间同步与录像文件列表查询
- [202603161102_issue27_gb_live_tcp](2026-03/202603161102_issue27_gb_live_tcp/) - 修正 GB28181 实时流 TCP 协商时序与 `setup` 方向
