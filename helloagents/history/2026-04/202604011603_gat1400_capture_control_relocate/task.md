# 任务清单: 调整 GAT1400CaptureControl 代码归属路径

目录: `helloagents/history/2026-04/202604011603_gat1400_capture_control_relocate/`

---

## 1. 代码调整
- [√] 1.1 将 `rk_gb/App/Media/GAT1400CaptureControl.h/.cpp` 迁移到 `rk_gb/App/Protocol/gat1400/`
- [√] 1.2 调整 `GAT1400ClientService.h` 的 include，以及主工程 / `sdk_port` 构建脚本中的源文件归属

## 2. 知识库同步
- [√] 2.1 更新 `helloagents/wiki/modules/gat1400.md`
- [√] 2.2 更新 `helloagents/wiki/api.md`
- [√] 2.3 更新 `helloagents/wiki/data.md`
- [√] 2.4 更新 `helloagents/CHANGELOG.md`
- [√] 2.5 更新 `helloagents/history/index.md`

## 3. 验证
- [√] 3.1 执行 `git diff --check`
- [√] 3.2 执行最小语法检查，覆盖 `GAT1400CaptureControl.cpp` 与 `GAT1400ClientService.cpp`

## 执行结果
- `GAT1400CaptureControl.h/.cpp` 已从 `rk_gb/App/Media/` 迁移到 `rk_gb/App/Protocol/gat1400/`，实现位置与模块归属保持一致。
- `GAT1400ClientService.h` 已改为直接包含同目录下的 `GAT1400CaptureControl.h`；主工程与 `sdk_port` 的 CMake 源文件归属也已同步收口到 `Protocol_SRC`。
- 当前知识库中的运行态描述已统一改为新路径 `App/Protocol/gat1400/GAT1400CaptureControl.*`。
- 已执行 `git diff --check`，并基于 `cmake-build/App/CMakeFiles/dgiot.dir/flags.make` 映射本地 include/define 后，用主机 `g++ -std=gnu++11 -fsyntax-only` 检查 `GAT1400CaptureControl.cpp` 与 `GAT1400ClientService.cpp`；本次改动未引入新的语法错误。`GAT1400ClientService.cpp` 语法检查过程中仍会看到现存的 `GB_ID_LEN` 宏重复定义 warning，但不是本次路径调整引入的问题。
