# 任务清单: zero_config 独立本地配置文件拆分

目录: `helloagents/history/2026-03/202603270956_zero_config_ini_split/`

---

- [√] 1. 在 `LocalConfigProvider` 中新增独立 `zero_config.ini` 读写，并把 `gb28181.ini` 收缩为 GB 注册基础字段
- [√] 2. 在编译期开启 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 且 `zero_config.ini` 缺失时，记录日志并返回错误，不做兼容迁移
- [√] 3. 同步更新 `helloagents` 知识库、`CHANGELOG` 与 `history/index`
- [√] 4. 使用 `git diff --check` 与 `g++ -fsyntax-only` 做最小静态验证

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
