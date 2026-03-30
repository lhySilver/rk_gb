# 技术设计: OSD 统一状态模型与多文本协议兼容

## 技术方案
### 核心技术
- 删除 `VideoOsdConfig`，统一使用 `media::VideoOsdState` 作为 OSD 读写结构
- 为 `VideoOsdState` 增加多文本项数组/列表与对应有效性标记
- 在媒体层维护“协议 OSD 状态”与“底层 RK OSD 已落地状态”的桥接

### 实现要点
- `App/Media/VideoOsdControl.h`
  - 删除 `VideoOsdConfig`
  - 为 `VideoOsdState` 增加多文本项结构，例如 `VideoOsdTextItem`
  - 显式补齐：
    - `has_text_items`
    - `text_item_count`
    - `text_items[]`
    - `has_protocol_time_format` / `protocol_time_format`（如需要保留统一格式语义）
- `App/Media/VideoOsdControl.cpp`
  - `ApplyVideoOsdConfig` 改为接受 `const VideoOsdState&`
  - 设置时按统一状态模型读取：
    - 总开关/时间开关/事件/告警
    - 时间格式
    - 时间坐标
    - 文本项列表
  - RK OSD 真实落地规则保持现状：
    - 只消费第 1 个文本项
    - 文本位置仍映射到现有 custom text 通道
  - 查询时返回统一状态模型：
    - 底层可读字段从 `rk_osd_*` 读取
    - 无 getter 的逻辑状态继续由媒体层缓存
    - 多文本项优先返回最近一次协议设置/配置持久化后的统一状态
- `App/Protocol/ProtocolManager.cpp`
  - 删除 `BuildVideoOsdConfigFromRuntimeState` / `IsVideoOsdConfigMatched` 对 `VideoOsdConfig` 的依赖
  - 改为直接基于 `VideoOsdState` 做 merge / compare / apply
  - `BuildGbOsdQueryConfig` 改为从统一状态模型组装 `SumNum + Item[]`
  - `HandleGbConfigControl` 接收 `OSDConfig` 时，完整写入 `Item[]`
- `App/Protocol/config/ProtocolExternalConfig.h`
  - `gb_osd` 配置模型扩展多文本项，用于设备重启后的协议侧回显一致性
  - 旧单文本字段保留兼容读取，内部转换为首条文本项

## 架构决策 ADR
### ADR-20260330-01: OSD 读写统一以 VideoOsdState 为唯一结构
**上下文:** 当前 `ApplyVideoOsdConfig(VideoOsdConfig)` 与 `QueryVideoOsdState(VideoOsdState*)` 使用两套结构，开发者需要额外理解“目标配置模型”和“运行态模型”的转换。
**决策:** 删除 OSD 专用写入结构，统一以 `VideoOsdState` 作为媒体层 OSD 唯一数据模型。
**理由:** 统一接口面后，协议层 `query -> merge -> apply -> query` 逻辑会更直接，调用方不再需要在两套结构之间来回折叠。
**替代方案:** 保留 `VideoOsdConfig`，仅在命名和注释上解释差异。
**拒绝原因:** 不能真正消除认知负担，也无法自然承载多文本项。

### ADR-20260330-02: 多文本先进入统一状态模型，底层设备继续尽力映射首条文本
**上下文:** GB28181 白皮书 `OSDConfig` 已支持 `Item[]`，但 RK OSD 当前只有一条自定义文本能力。
**决策:** 协议与媒体统一状态模型完整支持多文本，底层 RK OSD 只落地第 1 个文本项，其余文本项保留在统一状态中供 GB 查询回显。
**理由:** 满足“GB 接口先支持”的联调目标，同时不虚构 SoC 实际能力。
**替代方案:** 因 SoC 不支持而继续限制统一模型只能有 1 条文本。
**拒绝原因:** 会把设备能力边界错误上推到协议模型，后续再扩展多文本时改动更大。

## 数据模型
```text
media::VideoOsdTextItem
  - bool has_text
  - std::string text
  - bool has_position
  - int x
  - int y

media::VideoOsdState
  - has_master_enabled / master_enabled
  - has_event_enabled / event_enabled
  - has_alert_enabled / alert_enabled
  - has_time_enabled / time_enabled
  - has_text_enabled / text_enabled
  - has_date_style / date_style
  - has_time_style / time_style
  - has_time_position / time_x / time_y
  - has_text_items / text_item_count / text_items[]
```

## 安全与性能
- **安全:** 多文本解析继续使用有界拷贝，不直接信任平台上送长度。
- **安全:** 状态缓存只保存协议语义，不新增外部写文件面时不持久化敏感数据。
- **性能:** 多文本仅影响配置面和查询应答组包，对媒体发送链路没有额外热路径开销。

## 测试与部署
- 搜索所有 `VideoOsdConfig` 调用点，确保签名切换后无遗留引用
- 对 `App/Media/VideoOsdControl.cpp`、`App/Protocol/ProtocolManager.cpp` 做最小编译验证
- 重点回归：
  - 单文本 `OSDConfig` 查询/设置不回退
  - 多文本 `OSDConfig` 设置可被解析并在查询中回显
  - SoC 仍能正确落首条文本与时间样式
