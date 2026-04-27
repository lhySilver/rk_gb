# 任务清单

- [√] 1.1 确认 GB28181 OSD 文本当前从 XML 原始字节进入 `VideoOsdState`
- [√] 1.2 确认可复用的 RK GBK -> UTF-8 转码实现已在 `libmpp` 源码构建范围内
- [√] 2.1 新增 `tools/tests/gb_osd_text_gbk_to_utf8_regression.py` 并确认修复前失败
- [√] 3.1 在 `ProtocolManager.cpp` 增加 GBK 完整字节长度保护和 UTF-8 转换 helper
- [√] 3.2 在 `OSDConfig Item/Text` 映射到 `VideoOsdTextItem::text` 前调用转换 helper
- [√] 4.1 更新知识库和变更记录
- [√] 5.1 运行回归、语法和 diff 校验

## 验证记录

- `python3 tools/tests/gb_osd_text_gbk_to_utf8_regression.py` 通过
- `g++ -std=c++11 -fpermissive -fsyntax-only ... App/Protocol/ProtocolManager.cpp` 通过；仅保留仓库既有 `GB_ID_LEN` 宏重定义 warning
- `git diff --check` 通过
