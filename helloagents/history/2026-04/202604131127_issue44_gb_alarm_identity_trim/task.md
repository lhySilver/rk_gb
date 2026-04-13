# 任务清单: issue 44 GB 报警身份实现收缩

目录: `helloagents/history/2026-04/202604131127_issue44_gb_alarm_identity_trim/`

---

## 1. 代码收缩
- [√] 1.1 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中内联 GB 报警 `DeviceID/AlarmID` 归一化逻辑，保持“运行态 DeviceID 优先”行为不变
- [√] 1.2 删除 `rk_gb/App/Protocol/gb28181/GbAlarmIdentity.*` 与 `rk_gb/App/Protocol/tests/GbAlarmIdentityTest.cpp`，并同步清理构建脚本中的多余源文件引用

## 2. 知识库同步
- [√] 2.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 2.2 更新 `helloagents/wiki/modules/protocol.md`
- [√] 2.3 更新 `helloagents/CHANGELOG.md`
- [√] 2.4 更新 `helloagents/history/index.md`

## 3. 验证
- [√] 3.1 执行 `git -C /home/jerry/silver/rk_gb diff --check`
- [√] 3.2 对 `rk_gb/App/Protocol/ProtocolManager.cpp` 执行编译级语法校验，确认精简后仍可通过

## 执行结果
- `ProtocolManager.cpp` 现直接在匿名命名空间内保留 `ResolveGbAlarmRuntimeDeviceId()` 与 `NormalizeGbAlarmIdentity()` 两个静态 helper，`NotifyGbAlarm()` 继续优先使用订阅回调带回的运行态 `gbCode`，`AlarmID` 为空时补同一设备编号。
- 已删除 `App/Protocol/gb28181/GbAlarmIdentity.cpp/.h` 和 `App/Protocol/tests/GbAlarmIdentityTest.cpp`，并从 `App/CMakeLists.txt`、`App/Protocol/gb28181/sdk_port/CMakeLists.txt` 中移除多余源文件引用，收缩嵌入式构建面。
- 知识库已同步更新当前口径：行为修复保留，但实现不再通过新增独立编译单元落地。
- 验证完成：`git diff --check` 通过；基于 `cmake-build/App/CMakeFiles/dgiot.dir/flags.make` 的 include/define 映射，对 `ProtocolManager.cpp` 执行 `g++ -std=gnu++11 -fsyntax-only` 通过，仅保留仓库原有 `GB_ID_LEN` 宏重复定义 warning。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
