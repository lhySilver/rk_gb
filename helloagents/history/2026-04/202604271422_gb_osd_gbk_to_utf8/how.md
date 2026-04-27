# 实施方案

## 改动范围

- `App/Protocol/ProtocolManager.cpp`
  - 声明已有 `RK_encode_gbk_to_utf8()` C 符号。
  - 新增 `GetCompleteGbkByteLength()`，避免 `OSD_TEXT_LEN` 字节截断后把半个 GBK 字符送入转码函数。
  - 新增 `ConvertGbOsdTextToUtf8()`，把规范化后的 GBK 文本转成 UTF-8。
  - 在 `HandleGbConfigControl()` 的白皮书 `OSDConfig Item/Text` 分支中，写入 `VideoOsdTextItem::text` 前完成转换。

- `tools/tests/gb_osd_text_gbk_to_utf8_regression.py`
  - 静态回归保护协议入口转码位置。
  - 明确禁止 `VideoOsdControl.cpp` 感知 GBK 转码。

## 设计决策

### ADR-20260427-01: 转码放在 GB28181 协议入口

**决策:** 在 `OsdSetting -> media::VideoOsdState` 映射时做 GBK -> UTF-8。

**理由:** GBK 是平台协议输入格式，不是 video 模块通用能力。入口转换后，video 相关模块只处理 UTF-8，可以保持边界简单。

## 验证

- 先运行新增回归检查，确认现状失败。
- 实现后运行回归检查，确认通过。
- 对 `ProtocolManager.cpp` 做本地 `g++ -fsyntax-only` 语法检查。
- 运行 `git diff --check`。
