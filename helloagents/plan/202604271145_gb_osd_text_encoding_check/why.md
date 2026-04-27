# 变更提案: GB28181 OSD 中文 Text 编码确认

## 背景
需要确认 GB28181 平台下发 `OSDConfig/Item/Text` 时，中文字符在当前设备代码中以什么格式进入 OSD 链路，以及 `VideoOsdState` 是否会修改字符编码或格式。

## 当前代码级初步结论
- `OSDConfig/Item/Text` 在 XML 解析层通过 `markup.GetData()` 取出，再 `strncpy` 到 `OsdSetting::Item[].Text`。
- `ProtocolManager` 通过 `SafeStr()` 和 `NormalizeGbOsdTextTemplate()` 放入 `media::VideoOsdTextItem::text`。
- `VideoOsdState` 使用 `std::string` 承载文本，不记录编码类型。
- `VideoOsdControl` 只做 trim / `"null"` 转空 / 空项过滤，最终把 `std::string::c_str()` 原样传给 `rk_osd_set_display_text()`。
- 当前代码没有看到 GBK、GB2312、UTF-8 之间的转码逻辑。

## 待确认问题
- 平台实际下发 XML 是否声明或默认使用 UTF-8。
- 底层 `rk_osd_set_display_text()` 期望输入 UTF-8、GBK，还是跟字体/OSD 实现有关。
- `OSD_TEXT_LEN=64` 的字节级截断是否可能截断 UTF-8 中文多字节字符。

## 非目标
- 本任务先确认链路与风险，不直接修改 OSD 编码逻辑。
- 不改 GB28181 XML 解析器或媒体 OSD 接口。

## 验收标准
- 给出代码链路结论。
- 如需修改，明确修改点应放在 GB 协议层还是媒体 OSD 层。
- 明确是否需要抓包或板端中文 OSD 实测。
