# 任务清单: GB28181 OSD 中文 Text 编码确认

目录: `helloagents/plan/202604271145_gb_osd_text_encoding_check/`

---

## 1. 代码链路确认
- [√] 1.1 确认 `GB28181XmlParser.cpp::ParseOsdConfigElement()` 读取 `<Text>` 的方式。
- [√] 1.2 确认 `ProtocolManager.cpp` 从 `OsdSetting` 转到 `VideoOsdState` 的方式。
- [√] 1.3 确认 `VideoOsdControl.cpp` 对 `VideoOsdState::text_items[].text` 的处理方式。

## 2. 待实测项
- [ ] 2.1 抓包确认平台实际下发中文 `<Text>` 的 XML 编码和原始字节。
- [ ] 2.2 板端分别下发 UTF-8 / GBK 中文，确认 `rk_osd_set_display_text()` 实际期望编码。
- [ ] 2.3 确认 `OSD_TEXT_LEN=64` 字节截断是否会破坏中文多字节字符。

## 3. 后续决策
- [ ] 3.1 若底层统一要求 UTF-8，设计 GBK/UTF-8 检测与转换位置。
- [ ] 3.2 若底层接受原始字节或平台固定 UTF-8，仅补文档和长度风险说明。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
