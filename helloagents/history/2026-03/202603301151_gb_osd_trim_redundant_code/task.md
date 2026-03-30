# 任务清单: GB OSD 冗余代码瘦身

目录: `helloagents/history/2026-03/202603301151_gb_osd_trim_redundant_code/`

## 1. 收缩最近 OSD 改动引入的冗余代码
- [√] 1.1 清理 `ProtocolManager.cpp` 中未使用的 OSD helper / 无效前置声明
- [√] 1.2 合并 OSD apply 日志中的重复字符串与重复函数调用，减少无必要代码体积

## 2. 验证与收尾
- [-] 2.1 重新编译 `dgiot` 并记录构建后体积，确认未引入行为回退；当前 `cmake-build/Makefile` 依赖的 `/opt/cmake/cmake-3.16.3-Linux-x86_64/bin/cmake` 缺失，改以同编译宏下的 `ProtocolManager.o` 对比验证，`dec` 从 `237955` 降到 `236490`
- [√] 2.2 更新必要留痕并迁移方案包到 `history/`
