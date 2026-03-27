# 任务清单: GB 运行时注册模式切换

目录: `helloagents/plan/202603271028_gb_runtime_register_mode/`

---

## 1. 配置模型
- [√] 1.1 在 `ProtocolExternalConfig.h` 中新增 `gb_register.register_mode`，并补齐默认值与运行时判定辅助函数

## 2. 配置读写链路
- [√] 2.1 在 `LocalConfigProvider.cpp` 中让 `gb28181.ini` 持久化 `register_mode`，并改成依据运行时模式决定是否要求 `zero_config.ini`
- [√] 2.2 在 `HttpConfigProvider.cpp` 中补齐 `gb_register_mode` 的默认值、JSON 序列化/反序列化和校验

## 3. 注册流程切换
- [√] 3.1 在 `ProtocolManager.cpp` 中把标准/零配置分支改为基于 `register_mode` 的运行时判断
- [√] 3.2 在 `GBClientImpl.cpp` 中移除 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 的注册分支限制，改为使用 `GBRegistParam.use_zero_config`

## 4. 构建与知识库
- [√] 4.1 移除或停用构建脚本中对 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 的切换依赖
- [√] 4.2 更新 `helloagents` 知识库、`CHANGELOG` 与 `history/index`

## 5. 验证
- [√] 5.1 执行 `git diff --check`
- [√] 5.2 对 `LocalConfigProvider.cpp`、`HttpConfigProvider.cpp`、`ProtocolManager.cpp`、`GBClientImpl.cpp` 做 `g++ -fsyntax-only` 校验

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
