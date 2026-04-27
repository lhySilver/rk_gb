# GB OSD 文本 GBK 转 UTF-8

## 背景

平台下发 GB28181 `OSDConfig/Item/Text` 时，中文文本按 GBK 字节发送；当前代码在 XML 解析后直接把原始字节写入 `media::VideoOsdState.text_items[].text`，`VideoOsdControl` 再原样调用 `rk_osd_set_display_text()`。

## 问题

- GBK 中文原样进入 video 相关模块时，底层按 UTF-8 渲染会显示乱码。
- `VideoOsdState` 只是运行态/应用态数据结构，不应该承担“平台 GBK”这类协议入口语义。
- 不能为这次修复引入 Makefile/CMake 调整。

## 目标

1. 在 GB28181 协议入口把平台 GBK 文本转成 UTF-8。
2. 让 `VideoOsdState` 和 `VideoOsdControl` 继续只接收普通 UTF-8 字符串。
3. 复用已有 RK 转码实现，不新增大表或新编译目标。
