# ZeroConfig 设备端配置清单

## 一、重定向阶段配置（出厂预置）

### 国标28181

| 配置项 | 说明 | 示例 |
|--------|------|------|
| `gb_redirect_server_ip` | 国标重定向服务器IP | 183.252.186.165 |
| `gb_redirect_server_port` | 国标重定向服务器端口 | 15566 |
| `gb_redirect_realm` | 国标重定向域 | domain.example.com |
| `gb_redirect_password` | 国标重定向鉴权口令 | password123 |
| `string_code` | 设备串码/CMEI | 35010200001190000010 |
| `mac_address` | 设备MAC地址 | XX:XX:XX:XX:XX:XX |

### GAT1400（如支持）

| 配置项 | 说明 | 示例 |
|--------|------|------|
| `gat_redirect_server_ip` | GAT1400重定向服务器IP | 183.252.186.166 |
| `gat_redirect_server_port` | GAT1400重定向服务器端口 | 33855 |
| `gat_redirect_device_id` | GAT1400重定向设备ID | 35010200001190000010 |
| `gat_redirect_password` | GAT1400重定向鉴权口令 | password123 |

---

## 二、正式平台配置（302响应后动态获取）

### 国标28181

| 配置项 | 说明 | 来源 |
|--------|------|------|
| `gb_device_id` | 正式平台分配的设备编码 | 302响应 |
| `gb_server_ip` | 正式平台服务器IP | 302响应 |
| `gb_server_port` | 正式平台服务器端口 | 302响应 |
| `gb_username` | 正式平台用户名 | 302响应 |
| `gb_password` | 正式平台密码 | 302响应 |

### GAT1400

| 配置项 | 说明 | 来源 |
|--------|------|------|
| `gat_device_id` | GAT1400设备编码 | 302响应 |
| `gat_server_ip` | GAT1400服务器IP | 302响应 |
| `gat_server_port` | GAT1400服务器端口 | 302响应 |
| `gat_username` | GAT1400用户名 | 302响应 |
| `gat_password` | GAT1400密码 | 302响应 |

---

## 三、配置持久化要求

| 类型 | 说明 |
|------|------|
| 静态配置 | 重定向服务器地址、端口、设备串码、MAC、鉴权口令（出厂预置） |
| 动态配置 | 正式平台deviceId、服务器地址、用户名密码（302返回后更新并持久化） |

---

## 四、注册流程

1. 设备使用**重定向配置**发起首次REGISTER
2. 重定向服务器返回302，携带正式平台信息
3. 设备保存正式平台信息到配置
4. 设备使用**正式配置**完成注册
5. 后续启动直接使用正式配置，无需再走重定向
