# 技术设计: GB28181 OSD 中文 Text 编码确认

## 技术方案
本任务先做只读确认，不改代码。

## 检查路径
1. XML 解析入口：
   - `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp`
   - `ParseOsdConfigElement()`
   - `CopyMarkupText()`
2. GB 协议层转换：
   - `App/Protocol/ProtocolManager.cpp`
   - `NormalizeGbOsdTextTemplate()`
   - `NormalizeVideoOsdStateForProtocol()`
   - `HandleGbConfigControl()` 中 `kOsdSetting` 分支
3. 媒体 OSD 下发：
   - `App/Media/VideoOsdControl.cpp`
   - `NormalizeVideoOsdTextTemplate()`
   - `ApplyVideoOsdConfig()`
   - `QueryVideoOsdState()`
4. 底层接口：
   - `rk_osd_set_display_text()`
   - `rk_osd_get_display_text()`

## 初步判断
- 当前链路是“原始字节透传”，不是显式 UTF-8 或 GBK 转换。
- 如果平台 XML body 是 UTF-8，则中文文本会以 UTF-8 字节进入 `VideoOsdState` 和底层 OSD。
- 如果平台 XML body 是 GBK，则当前代码也会把 GBK 字节原样传下去。
- `VideoOsdState` 不会主动修改编码格式，只会保留 `std::string`，并经过 trim / `"null"` 转空 / 空项过滤。

## 风险
- `OsdTextItem::Text` 固定 `64` 字节，中文 UTF-8 一个字通常 3 字节，GBK 一个字通常 2 字节；字节级截断可能截断半个中文字符。
- 代码中 OSD response 的 XML 声明目前不显式带 `encoding`，回显时同样按当前字节串输出。

## 验证建议
- 用抓包确认平台 `DeviceConfig/OSDConfig/Item/Text` 的 XML 声明和原始字节。
- 在板端分别下发 UTF-8 和 GBK 中文，观察 `rk_osd_set_display_text()` 是否能正确显示。
- 若底层只接受 UTF-8，应在 GB XML 进入 `VideoOsdState` 前统一转 UTF-8，并补字节长度保护。
