# GAT1400 配置说明

## 配置文件位置

`App/Protocol/config/ProtocolExternalConfig.h`

## 配置结构

GAT1400 相关配置有以下三类：

---

## 1. GatRegisterParam (注册参数)

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| server_ip | string | - | 服务器IP |
| server_port | int | - | 服务器端口 |
| device_id | string | - | 设备ID |
| username | string | - | 用户名 |
| password | string | - | 密码 |
| auth_method | string | "digest" | 认证方式 |
| listen_port | int | 18080 | 监听端口 |
| expires_sec | int | 3600 | 过期时间(秒) |
| keepalive_interval_sec | int | 60 | 心跳间隔(秒) |
| max_retry | int | 3 | 最大重试次数 |

---

## 2. GatUploadParam (上传参数)

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| batch_size | int | 10 | 批量大小 |
| flush_interval_ms | int | 500 | 刷新间隔(毫秒) |
| retry_policy | string | "1,2,5" | 重试策略 |

---

## 3. GatCaptureParam (抓拍参数)

| 参数 | 类型 | 默认值 | 说明 |
|------|------|--------|------|
| face_profile | string | - | 人脸配置 |
| nonmotor_profile | string | - | 非机动车配置 |
| vehicle_profile | string | - | 机动车配置 |
| concurrent_limit | int | 3 | 并发限制 |

---

## 使用方式

在代码中通过 `ProtocolExternalConfig` 结构体访问：

```cpp
protocol::ProtocolExternalConfig cfg;
cfg.gat_register.server_ip = "192.168.1.100";
cfg.gat_register.server_port = 8080;
cfg.gat_upload.batch_size = 20;
cfg.gat_capture.concurrent_limit = 5;
```
