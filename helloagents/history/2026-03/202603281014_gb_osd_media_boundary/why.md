# 变更提案: GB OSD 配置职责下沉到媒体模块

## 需求背景
当前 GB28181 的 OSD 获取和设置虽然已经联调打通，但设备侧落地实现仍然直接写在 `ProtocolManager` 里：
- GB 查询路径会直接读取 `rk_osd_*`
- GB 设置路径会直接调用 `CaptureSetOSDSwitch` 和 `rk_osd_*`
- `ProtocolManager` 内还额外维护了一份 `m_gb_osd_*` 运行态镜像

这导致协议模块同时承担了“协议字段映射”和“设备编码/OSD 能力控制”两层职责，代码链路偏绕，也不利于后续由编码模块维护者继续接手。

## 变更目标
1. 将设备 OSD 落地能力抽到 `App/Media` 侧接口中，由媒体/编码模块对外提供统一入口。
2. `ProtocolManager` 仅保留 GB 协议字段与设备 OSD 接口之间的映射，不再直接访问 `rk_osd_*`。
3. 清理 `ProtocolManager` 内部多余的 GB OSD 运行态镜像，避免协议模块维护设备状态副本。

## 影响范围
- **模块:** `ProtocolManager`、`Media`
- **文件:** `App/Protocol/ProtocolManager.*`、新增 `App/Media/VideoOsdControl.*`、构建脚本、知识库
- **行为边界:** 不改变当前 GB OSD 查询/设置对外协议语义，重点调整内部职责归属

## 核心场景

### 需求: GB 设置 OSD
**模块:** `ProtocolManager` / `Media`
平台下发 `SVACEncodeConfig/SVACDecodeConfig` 或白皮书 `OSDConfig`。

#### 场景: GB 只做映射，媒体模块负责落地
- `ProtocolManager` 解析协议字段
- `ProtocolManager` 更新协议配置模型 `m_cfg.gb_osd`
- `ProtocolManager` 调用媒体模块 OSD 接口
- 媒体模块内部决定如何调用 `CaptureSetOSDSwitch`、`rk_osd_*`

### 需求: GB 查询 OSD
**模块:** `ProtocolManager` / `Media`
平台查询 `OSDConfig` 或 `SVAC*` OSD 位。

#### 场景: GB 通过媒体接口读取设备运行态
- `ProtocolManager` 仍负责组装 GB 查询应答
- 当前设备实际 OSD 开关、位置、文本由媒体模块提供
- 若媒体模块未返回完整运行态，则按已有 GB 配置回退

## 风险评估
- **风险:** 这次重构会同时触达协议模块、媒体模块和构建脚本。
- **缓解:** 只抽离 OSD 相关能力，不改 GB XML/消息模型，也不调整其他视频参数链路。
- **风险:** 编码模块后续可能替换具体 OSD 实现。
- **缓解:** 先固化 `App/Media/VideoOsdControl.*` 作为稳定接口面，后续只需要替换接口内部实现。
