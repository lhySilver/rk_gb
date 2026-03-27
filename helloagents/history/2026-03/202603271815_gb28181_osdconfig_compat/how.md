# 技术设计: GB28181 OSDConfig 设备侧兼容

## 技术方案
### 核心技术
- GB28181 共享配置结构扩展
- `GB28181XmlParser` 手工补充 `OSDConfig` 编解码
- `ProtocolManager` 复用现有 `gb_osd` 与 `rk_osd_*` 生效链

### 实现要点
- `OSDConfig` 不再复用 `SVACEncodeConfig/SVACDecodeConfig`，而是在共享结构中单独建模。
- 查询链路先识别 `ConfigType=OSDConfig`，再在 `ResponseGbQueryConfig` 中生成 `OSDConfig` 数据并由 XML 层编码成白皮书报文。
- 设置链路在 `UnPackConfigControl` 中直接解析 `<OSDConfig>`，生成新的 `kOsdSetting` 配置项，避免再落成空 `config_set_param`。
- `ProtocolManager` 统一把 `TimeX/TimeY/TimeEnable/TimeType/TextEnable/Item` 映射到现有 `gb_osd.position/time_enabled/time_format/text_template`。
- 自定义文字保持当前设备能力边界：最多消费 1 个 `Item`，多余项忽略并保留日志。

## 架构决策 ADR
### ADR-20260327-01: 为 OSDConfig 单独建模，不继续挤在 SVAC 配置里
**上下文:** 当前平台使用白皮书 `OSDConfig`，而设备仅支持 `SVAC*` 下的 OSD 三个位，导致查询返回空、设置返回 `-82`。
**决策:** 在共享 GB28181 结构中新增 `kOsdConfig/kOsdSetting` 和对应 OSD 数据结构，由 XML 层与 `ProtocolManager` 直接消费。
**理由:** 白皮书 `OSDConfig` 与 `SVAC*` 字段语义不同，继续复用旧模型只会扩大兼容性债务。
**替代方案:** 用 `SVACEncodeConfig` 硬映射 `OSDConfig` → 拒绝原因: 无法表达 `TimeX/TimeY/TextEnable/Item`。
**影响:** 需要同时改共享结构、XML 解析和 App 配置应用逻辑。

### ADR-20260327-02: RK OSD 显示能力不足的字段先做协议兼容，再保留能力缺口
**上下文:** 白皮书 `TimeType=1` 要求中文日期格式，但 RK OSD 当前仅暴露 `CHR-YYYY-MM-DD` 等样式。
**决策:** 协议层完整解析和回显 `TimeType`，配置层保存该值对应的格式语义，但底层显示仍按当前 RK OSD 能力尽力映射。
**理由:** 这次联调阻塞点是“通信模型不兼容”，优先恢复报文互通和配置闭环。
**替代方案:** 因底层不支持而拒绝解析 `TimeType=1` → 拒绝原因: 仍无法完成当前平台联调。
**影响:** 需要在知识库中明确“协议兼容已补齐，底层显示仍有限制”。

## 数据模型
```text
ConfigType:
  + kOsdConfig

SettingType:
  + kOsdSetting

CfgOsdConfig:
  - Length
  - Width
  - TimeX
  - TimeY
  - TimeEnable
  - TimeType
  - TextEnable
  - SumNum
  - Item[Text, X, Y]

OsdSetting:
  - Length
  - Width
  - TimeX
  - TimeY
  - TimeEnable
  - TimeType
  - TextEnable
  - SumNum
  - Item[Text, X, Y]
```

## 安全与性能
- **安全:** XML 解析时限制 `Item` 数量，避免异常大报文导致越界或非预期分配。
- **安全:** 自定义文字内容使用有界拷贝，禁止直接信任平台长度。
- **性能:** `OSDConfig` 仅影响 SIP 控制面 XML 组包与配置应用，对媒体通路无额外开销。

## 测试与部署
- **测试:** 用现有 `123.pcap` 对照日志验证两条路径：
  - `ConfigDownload + OSDConfig` 不再回空配置
  - `DeviceConfig + OSDConfig` 不再进入 `ret=-82`
- **测试:** 做一次最小编译或静态检查，确认新增枚举与结构不会破坏现有分支。
- **部署:** 保持当前运行分支，改动完成后由后续联调抓包确认平台是否接受 `OSDConfig` 应答细节。
