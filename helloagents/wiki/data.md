# 数据模型

## 概述
协议协商核心数据围绕 GB28181 `MediaInfo`、SIP 对话标识和运行态会话结构展开。

## 关键结构

### ProtocolExternalConfig.gb_register

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `enabled` | int | GB28181 总开关，`0` 为禁用，`1` 为启用 |
| `server_ip` | string | GB 平台 SIP 接入地址 |
| `server_port` | int | GB 平台 SIP 接入端口 |
| `device_id` | string | 设备国标编码 |
| `username` | string | 注册用户名/接入编码 |
| `password` | string | 注册密码 |

### gb28181.ini

| 键名 | 说明 |
|------|------|
| `enable` | GB28181 开关 |
| `username` | 接入编码 |
| `server_ip` | 接入 IP |
| `server_port` | 接入端口 |
| `device_id` | 设备编码 |
| `password` | 设备密码 |

默认路径: `/userdata/conf/Config/gb28181.ini`

### MediaInfo

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `RtpType` | enum | UDP/TCP 及 active/passive 模式 |
| `DeviceID` | char[] | 设备国标 ID |
| `IP` | char[] | 对端媒体地址 |
| `Port` | uint | 对端媒体端口 |
| `Ssrc` | char[] | 媒体 SSRC |
| `RequestType` | enum | 实时流/回放/下载/语音 |

### GbLiveSession

| 字段名 | 类型 | 说明 |
|--------|------|------|
| `active` | bool | 实时流会话是否有效 |
| `stream_handle` | 指针 | SIP/GB 流会话句柄 |
| `acked` | bool | 是否已收到 ACK |
| `audio_requested` | bool | 请求是否携带音频 |
