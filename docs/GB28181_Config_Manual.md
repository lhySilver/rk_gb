# GB28181 国标协议配置交互文档

## 概述

本文档描述GB28181国标协议中，编码、OSD等相关配置如何上报给平台，以及平台如何下发设备控制命令给设备。

## 一、配置上报流程（设备 → 平台）

### 1.1 设备信息上报 (DeviceInfo Query)

**触发条件**: 平台发起设备信息查询请求

**处理流程**:

```
平台发送MANSCDP查询请求 
    ↓
GB28181ClientSDK 接收请求
    ↓
GB28181ClientReceiverAdapter::OnQuery()
    ↓
ProtocolManager::HandleGbQueryRequest(handle, param)
    ↓
ProtocolManager::ResponseGbQueryDeviceInfo(handle, param)
    ↓
GB28181ClientSDK::ResponseDeviceInfo()
    ↓
发送响应XML给平台
```

**代码位置**:
- `App/Protocol/ProtocolManager.cpp:5516` - HandleGbQueryRequest
- `App/Protocol/ProtocolManager.cpp:5596` - ResponseGbQueryDeviceInfo

**关键数据结构** (`GB28181Defs.h`):
```cpp
struct DeviceInfo {
    char DeviceID[GB_ID_LEN];      // 设备ID
    char DeviceName[GB_ID_LEN];    // 设备名称
    char Manufacturer[STR_LEN];    // 制造商
    char Model[STR_LEN];            // 型号
    char Firmware[STR_LEN];        // 固件版本
    char StreamNumberList[STR_LEN];// 通道数
};
```

### 1.2 设备配置上报 (DeviceConfig Query)

**触发条件**: 平台发起设备配置查询请求

**处理流程**:

```
平台发送设备配置查询请求
    ↓
ProtocolManager::HandleGbQueryRequest()
    ↓
case kDeviceConfigQuery: ResponseGbQueryConfig()
    ↓
收集编码配置和OSD配置
    ↓
GB28181ClientSDK::ResponseDeviceConfigQuery()
    ↓
发送响应XML给平台
```

**代码位置**: `App/Protocol/ProtocolManager.cpp:5556` - kDeviceConfigQuery处理

#### 1.2.1 基础参数 (BasicParam)

```cpp
item.CfgType = kBasicParam;
item.UnionCfgParam.CfgBasic.DeviceName       = 设备名称;
item.UnionCfgParam.CfgBasic.Expiration      = 注册有效期(秒);
item.UnionCfgParam.CfgBasic.HeartBeatInterval = 心跳间隔(秒);
item.UnionCfgParam.CfgBasic.HeartBeatcount  = 心跳超时次数;
item.UnionCfgParam.CfgBasic.PosCapability   = 定位能力;
item.UnionCfgParam.CfgBasic.Longitude        = 经度;
item.UnionCfgParam.CfgBasic.Latitude         = 纬度;
```

#### 1.2.2 视频编码参数 (VideoParamOpt)

```cpp
item.CfgType = kVideoParamOpt;
item.UnionCfgParam.CfgVideoOpt.DownloadSpeedOpt = 下载速度;
item.UnionCfgParam.CfgVideoOpt.Resolution       = 分辨率(如"1920x1080/1280x720");
item.UnionCfgParam.CfgVideoOpt.ImageFlip         = 镜像模式(flip/mirror/close);
```

**获取编码参数代码** (`ProtocolManager.cpp:6000-6030`):
```cpp
std::string mainCodec = NormalizeRkVideoCodec(m_cfg.gb_video.main_codec);
std::string mainFrameRate = BuildFrameRateValue(m_cfg.gb_video.main_fps);
int mainBitrate = m_cfg.gb_video.main_bitrate_kbps;
rk_video_get_max_rate(0, &mainBitrate);
rk_video_get_gop(0, &mainGop);
```

#### 1.2.3 SVAC编码配置 (SVACEncodeConfig)

```cpp
item.CfgType = kSVACEncodeConfig;
item.UnionCfgParam.CfgEncode.stuRoiParam.ROIFlag = 0;           // ROI开关
item.UnionCfgParam.CfgEncode.stuSurveilParam.TimeShowFlag = 1; // 时间OSD
item.UnionCfgParam.CfgEncode.stuSurveilParam.EventShowFlag = 1;// 事件OSD
item.UnionCfgParam.CfgEncode.stuSurveilParam.AlertShowFlag = 1; // 告警OSD
```

**OSD配置来源** (`ProtocolManager.cpp:6120-6128`):
```cpp
runtimeOsdEnabled = rk_osd_get_enabled(kRkOsdDateTimeId, &enabled);
m_cfg.gb_osd.time_enabled   = 时间OSD开关;
m_cfg.gb_osd.event_enabled  = 事件OSD开关;
m_cfg.gb_osd.alert_enabled  = 告警OSD开关;
```

#### 1.2.4 OSD配置获取

**OSD参数获取函数** (`ProtocolManager.cpp:258-300`):
```cpp
// 日期格式
NormalizeRkOsdDateStyle(formatIn)
// 时间格式(12hour/24hour)
NormalizeRkOsdTimeStyle(formatIn)
// OSD位置解析
ResolveRkOsdAnchor(positionIn)  // top_left/top_right/bottom_left/bottom_right
```

**OSD相关API** (`PAL/Capture.h`):
```cpp
int rk_osd_get_enabled(int id, int *value);      // 获取OSD开关
int rk_osd_set_enabled(int id, int value);       // 设置OSD开关
int rk_osd_get_position_x(int id, int *value);   // 获取X坐标
int rk_osd_set_position_x(int id, int value);    // 设置X坐标
int rk_osd_get_position_y(int id, int *value);   // 获取Y坐标
int rk_osd_set_position_y(int id, int value);    // 设置Y坐标
int rk_osd_get_date_style(int id, const char **value);  // 获取日期格式
int rk_osd_set_date_style(int id, const char *value);  // 设置日期格式
int rk_osd_get_time_style(int id, const char **value);  // 获取时间格式
int rk_osd_set_time_style(int id, const char *value);   // 设置时间格式
int rk_osd_get_display_text(int id, const char **value); // 获取自定义文本
int rk_osd_set_display_text(int id, const char *value);  // 设置自定义文本
int rk_osd_restart();  // 重启OSD生效
```

### 1.3 设备目录上报 (Catalog Query)

**代码位置**: `App/Protocol/ProtocolManager.cpp:5786` - ResponseGbQueryCatalog

```cpp
catalogList.sum = 1;
catalogInfo.DeviceID = 设备ID;
catalogInfo.Name = 设备名称;
catalogInfo.Manufacturer = "IPC";
catalogInfo.Model = "RC0240";
catalogInfo.StreamNumberList = 流列表;
```

### 1.4 设备状态上报 (DeviceStatus Query)

**代码位置**: `App/Protocol/ProtocolManager.cpp:5700` - ResponseGbQueryDeviceStatus

```cpp
status.Online = 1;                      // 在线状态
status.Status = 0;                      // 设备状态(0=正常)
FillGbTimeString(status.DevDateTime);   // 设备时间
```

---

## 二、配置下发流程（平台 → 设备）

### 2.1 设备控制命令类型

**定义位置**: `third_party/.../GB28181Defs.h:872-886`

```cpp
enum ControlType {
    kUnknowControl = 0,
    kPtzControl = 1,           // 云台控制
    kTeleBootControl = 2,      // 远程启动
    kDeviceUpgradeControl = 3, // 设备升级
    kDragZoomInControl = 4,    // 变倍放大
    kDragZoomOutControl = 5,   // 变倍缩小
    kIFameControl = 10,        // 强制I帧
    kRecordControl = 11,       // 录像控制
    kGurdControl = 12,         // 布防/撤防
    kAlarmResetControl = 13,   // 告警复位
    kDevConfigControl = 14,    // 设备配置
    kHomePositionControl = 15  // 预置位/归位
};
```

### 2.2 控制命令处理流程

```
平台发送设备控制请求
    ↓
GB28181ClientSDK::OnDeviceControl()
    ↓
GB28181ClientReceiverAdapter::OnDeviceControl()
    ↓
ProtocolManager::HandleGbDeviceControl() [未实现]
    ↓
GBClientImpl根据命令类型处理
    ↓
返回响应给平台
```

**代码位置**: 
- `third_party/.../GB28181SDK/GB28181Client/GBClientImpl.cpp:2933` - OnDeviceControl
- `App/Protocol/gb28181/GB28181ClientReceiverAdapter.cpp:77` - OnDeviceControl回调

### 2.3 配置下发类型

#### 2.3.1 设备配置下发 (DeviceConfig)

```cpp
// 下发配置参数结构
struct ConfigSetParam {
    unsigned int Num;           // 配置数量
    SettingParam* arySetParam;  // 配置参数数组
};

enum SettingType {
    kBasicSetting = 1,    // 基础配置
    kEncodeSetting = 2,   // 编码配置
    kDecodeSetting = 3,   // 解码配置
    kImageSetting = 4     // 图像配置
};
```

**处理位置**: `GB28181Defs.h:755-759`

#### 2.3.2 强制I帧 (IFrame)

```cpp
cmd.type = kIFameControl;
// 触发编码器产生I帧
// 响应: SendSipMessageWithOk(data)
```

#### 2.3.3 录像控制 (Record)

```cpp
cmd.type = kRecordControl;
cmd.control_param.record_cmd = true/false;
// 启动/停止本地录像
```

#### 2.3.4 云台控制 (PTZ)

```cpp
cmd.type = kPtzControl;
cmd.control_param.ptz_cmd.code = 云台命令码;
cmd.control_param.ptz_cmd.param1 = 参数1;
cmd.control_param.ptz_cmd.param2 = 参数2;
```

**相关函数**: 
- `ProtocolManager::HandleGbPtzControl()` (在ProtocolManager.h:318声明)

---

## 三、关键代码文件汇总

### 3.1 核心处理文件

| 文件路径 | 功能描述 |
|---------|---------|
| `App/Protocol/ProtocolManager.cpp` | 协议管理器，处理设备查询和响应 |
| `App/Protocol/gb28181/GB28181ClientReceiverAdapter.cpp` | GB28181客户端接收适配器 |
| `third_party/.../GB28181SDK/GB28181Client/GBClientImpl.cpp` | GB28181客户端SDK实现 |
| `third_party/.../GB28181Defs.h` | 国标协议数据结构定义 |

### 3.2 关键函数

| 函数名 | 位置 | 功能 |
|--------|------|------|
| HandleGbQueryRequest | ProtocolManager.cpp:5516 | 处理平台查询请求 |
| ResponseGbQueryDeviceInfo | ProtocolManager.cpp:5596 | 响应设备信息查询 |
| ResponseGbQueryConfig | ProtocolManager.cpp:5556 | 响应设备配置查询 |
| ResponseGbQueryCatalog | ProtocolManager.cpp:5786 | 响应设备目录查询 |
| ResponseGbQueryDeviceStatus | ProtocolManager.cpp:5700 | 响应设备状态查询 |
| OnDeviceControl | GBClientImpl.cpp:2933 | 处理设备控制命令 |

### 3.3 数据结构定义文件

- `third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` - 核心数据结构
- `Middleware/libmpp/Include/rockchip/rk_venc_cmd.h` - 编码参数结构
- `Middleware/libmpp/Include/PAL/Capture.h` - Capture和OSD API

---

## 四、配置参数详解

### 4.1 视频编码参数

| 参数 | 获取/设置函数 | 说明 |
|------|--------------|------|
| 编码格式 | rk_video_set_output_data_type | H.265/H.264 |
| 分辨率 | rk_video_set_resolution | 主码流/子码流 |
| 帧率 | rk_video_set_frame_rate | 如"25" |
| 码率 | rk_video_set_max_rate | 单位:kbps |
| GOP | rk_video_set_gop | I帧间隔 |

### 4.2 OSD参数

| 参数 | ID | 说明 |
|------|-----|------|
| 时间OSD | 1 | 日期时间叠加 |
| 自定义文本OSD | 2 | 自定义文字叠加 |

### 4.3 图像参数

| 参数 | 获取/设置函数 | 说明 |
|------|--------------|------|
| 镜像模式 | rk_isp_set_image_flip | flip/mirror/none |

---

## 五、消息流程图

### 5.1 配置上报流程

```
┌─────────┐     Query     ┌──────────┐     Query     ┌─────────────┐
│  平台   │ ────────────► │GB28181   │ ────────────► │ Protocol    │
│         │               │ClientSDK │               │ Manager     │
└─────────┘               └──────────┘               └─────────────┘
                                                            │
                              ┌─────────────┐               │
                              │DeviceInfo   │◄──────────────┘
                              │DeviceConfig │               │
                              │Catalog      │               │
                              │DeviceStatus │               │
                              └─────────────┘               │
                                                            ▼
                              ┌─────────────┐     Response ┌─────────────┐
◄─────────────────────────────│GB28181     │◄─────────────│  Platform   │
         Response             │ClientSDK   │              │             │
                              └─────────────┘              └─────────────┘
```

### 5.2 配置下发流程

```
┌─────────┐     Control    ┌──────────┐     Control    ┌─────────────┐
│  平台   │ ────────────► │GB28181   │ ────────────► │ Protocol    │
│         │               │ClientSDK │               │ Manager     │
└─────────┘               └──────────┘               └─────────────┘
                                                            │
                              ┌─────────────┐               │
                              │DeviceConfig │               │
                              │PTZ          │──────────────┼───► 处理控制
                              │Record       │               │
                              │IFrame       │               │
                              └─────────────┘               │
                                                            ▼
                              ┌─────────────┐     Response ┌─────────────┐
◄─────────────────────────────│GB28181     │◄─────────────│  Platform   │
         Response             │ClientSDK   │              │             │
                              └─────────────┘              └─────────────┘
```

---

## 六、注意事项

1. **HandleGbDeviceControl未实现**: 当前版本ProtocolManager中HandleGbDeviceControl函数未实现，设备配置下发功能由GBClientImpl直接处理

2. **编码/OSD配置上报**: 配置信息通过ResponseDeviceConfigQuery上报，包含视频编码参数和OSD配置

3. **配置生效**: OSD配置修改后需要调用rk_osd_restart()使配置生效

4. **线程安全**: 协议相关操作使用mutex保护，参见m_gb_lifecycle_mutex
