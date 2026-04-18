# 变更提案: issue47 平台 SDK 冗余文件清理

## 需求背景
issue 47 要求在 `feature/dg_ipc_replay_20260415` 分支清理 `third_party/platform_sdk_port/CommonFile` 与 `third_party/platform_sdk_port/CommonLibSrc` 下未使用的头文件和源码文件。附件 `unused_commonlib_headers.md` 只提供了初步线索，其中包含一部分仅在旧 SDK 内部互相引用、但并未接入当前构建的文件，不能直接机械删除。

## 变更内容
1. 基于当前分支真实构建入口、仓库源码引用和目录内 include 关系重新识别可达文件。
2. 删除 `third_party/platform_sdk_port/CommonFile` 与 `third_party/platform_sdk_port/CommonLibSrc` 下仓库内不可达的冗余头文件与源码文件。
3. 更新知识库和变更记录，沉淀本次“按真实引用清理第三方移植 SDK”的判定口径。

## 影响范围
- **模块:** `BuildRuntime`、`third_party/platform_sdk_port`
- **文件:** `third_party/platform_sdk_port/CommonFile/**`、`third_party/platform_sdk_port/CommonLibSrc/**`、`helloagents/**`
- **API:** 无
- **数据:** 无

## 核心场景

### 需求: 清理平台 SDK 冗余文件
**模块:** BuildRuntime / third_party platform_sdk_port
删除当前构建与运行链路未触达的旧 SDK/工具层文件，同时保持 GB28181、SIP、GAT1400 现有构建入口不变。

#### 场景: 基于真实引用清理
前置条件:
- 当前基线为 `origin/feature/dg_ipc_replay_20260415`
- `App/CMakeLists.txt` 与 `App/Protocol/gb28181/sdk_port/CMakeLists.txt` 显式列出了当前参与编译的 `platform_sdk_port` 源文件
- issue 附件只作为初步线索，不能直接作为删除清单
- 预期结果1: 删除文件不在当前构建入口中，也不被仓库内其他活代码引用
- 预期结果2: 当前 GB28181 / SIP / GAT1400 编译入口保持不变
- 预期结果3: 清理后差异检查和编译验证通过

## 风险评估
- **风险:** 老文件存在内部相互引用、BOM、大小写和相对路径差异，简单 grep 容易误判
- **缓解:** 以 CMake 显式源码入口为根，结合仓库级 include 图和人工抽样复核，再执行编译验证
