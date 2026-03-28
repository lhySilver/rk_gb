# 技术设计: GB OSD 配置职责下沉到媒体模块

## 技术方案
### 核心技术
- 新增媒体侧 OSD 控制接口 `VideoOsdControl`
- `ProtocolManager` 改为通过媒体接口进行 OSD 查询/设置
- 清理协议模块中的 OSD 直连底层实现

### 实现要点
- 在 `App/Media/VideoOsdControl.h/.cpp` 中定义并实现媒体侧 OSD 接口：
  - `ApplyVideoOsdConfig`
  - `QueryVideoOsdState`
  - `ResolveVideoOsdAnchor`
- 接口内部先承接当前 RK 平台实现，统一封装 `CaptureSetOSDSwitch`、`rk_osd_*`。
- `ProtocolManager` 继续负责：
  - GB `OSDConfig/SVAC*` 字段解析
  - `m_cfg.gb_osd` 配置持久化
  - GB 查询应答装配
- `ProtocolManager` 不再直接声明或调用 `rk_osd_*`，也不再维护 `m_gb_osd_time_enabled/event_enabled/alert_enabled`。

## 架构决策 ADR
### ADR-20260328-01: OSD 设备控制从 ProtocolManager 下沉到 Media 接口
**上下文:** 当前 OSD 获取/设置链路已可用，但设备能力落地写在 GB 协议模块内部，职责边界不清晰。
**决策:** 新增 `App/Media/VideoOsdControl.*` 作为媒体侧统一接口，GB 模块只调用接口。
**理由:** OSD 属于编码/媒体侧能力，不应由 GB 协议模块直接管理底层实现细节。
**替代方案:** 继续在 `ProtocolManager` 内维持 `rk_osd_*` 调用 → 拒绝原因: 后续编码模块接手时仍需要跨模块改协议代码。
**影响:** 媒体模块需要承担当前 RK OSD 的默认适配实现。

### ADR-20260328-02: 保留 GB 协议配置模型 `gb_osd`，但不再把它当成设备运行态所有者
**上下文:** 现有 HTTP/本地配置链路已经使用 `ProtocolExternalConfig::gb_osd`。
**决策:** 当前阶段保留 `gb_osd` 作为协议配置模型，设备运行态改由媒体模块查询提供。
**理由:** 这次目标是职责下沉，不是重做整个配置模型。
**替代方案:** 连同 `gb_osd` 一起迁出 `ProtocolExternalConfig` → 拒绝原因: 影响面过大，会连带改动本地配置和 HTTP 配置链路。
**影响:** `ProtocolManager` 仍保存 GB 协议视角的 OSD 配置，但不再直接控制底层 OSD 设备实现。

## 数据模型
```text
media::VideoOsdConfig
  - time_enabled
  - event_enabled
  - alert_enabled
  - text_template
  - time_format
  - position

media::VideoOsdState
  - master_enabled
  - time_enabled
  - text_enabled
  - time_x/time_y
  - text_x/text_y
  - text
  - has_* validity flags
```

## 安全与性能
- **安全:** 保持有界字符串读取和坐标合法性判断，不新增外部输入面。
- **性能:** OSD 控制仍发生在配置面，新增接口层不会影响媒体实时发送链路。

## 测试与部署
- **测试:** 进行 `ProtocolManager.cpp` 与 `VideoOsdControl.cpp` 的语法级编译检查。
- **测试:** 检查 GB 查询/设置调用路径是否全部改为媒体接口。
- **部署:** 不改变现有对外协议和配置字段，部署风险主要是编译级与运行时接口回归。
