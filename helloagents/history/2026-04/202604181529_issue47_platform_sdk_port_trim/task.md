# 任务清单: issue47 平台 SDK 冗余文件清理

目录: `helloagents/plan/202604181529_issue47_platform_sdk_port_trim/`

---

## 1. 可达性分析
- [√] 1.1 基于 `App/CMakeLists.txt`、`App/Protocol/gb28181/sdk_port/CMakeLists.txt` 和仓库级 `#include` 关系，识别 `third_party/platform_sdk_port/CommonFile` 与 `third_party/platform_sdk_port/CommonLibSrc` 的真实可达文件集合，验证 why.md#需求-清理平台-sdk-冗余文件-场景-基于真实引用清理
- [√] 1.2 对候选删除集做人工抽样复核，排除 BOM、相对路径和同名文件导致的误判，验证 why.md#需求-清理平台-sdk-冗余文件-场景-基于真实引用清理，依赖任务1.1

## 2. 文件清理
- [√] 2.1 删除 `third_party/platform_sdk_port/CommonFile` 下仓库内不可达的冗余头文件与源码文件，验证 why.md#需求-清理平台-sdk-冗余文件-场景-基于真实引用清理，依赖任务1.2
- [√] 2.2 删除 `third_party/platform_sdk_port/CommonLibSrc` 下仓库内不可达的冗余头文件与源码文件，验证 why.md#需求-清理平台-sdk-冗余文件-场景-基于真实引用清理，依赖任务2.1

## 3. 安全检查
- [√] 3.1 复核被删文件不在当前构建脚本显式源码列表中，且不存在来自活代码的直接或递归 include 依赖

## 4. 文档更新
- [√] 4.1 更新 `helloagents/wiki/modules/build_runtime.md`
- [√] 4.2 更新 `helloagents/CHANGELOG.md`、`helloagents/history/index.md`

## 5. 测试
- [√] 5.1 运行 `git diff --check`
- [√] 5.2 运行主工程配置与编译验证，确认 issue47 清理未破坏现有构建链
  > 备注: 先修复当前分支遗留的 `-o3` 与 `cmake_minimum_required=3.0` 构建基线问题，再执行 `REPO_DIR=/home/jerry/silver/rk_gb bash tools/issue_bot/build_verify.sh`，验证日志位于 `/tmp/rk_gb-build-verify.phHMEl/logs`

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
