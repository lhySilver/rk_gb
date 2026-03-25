# 变更提案: rk830 交叉编译隔离修复

## 需求背景
当前需要在本机把 `rk_gb` 的 Rockchip IPC 工程交叉编译通过，但这台机器同时还在进行其他 SoC 的编译任务。现有工程依赖 `RK/` 目录下的本地工具链，并在 `CMakeLists.txt` 中直接写死编译器前缀；如果直接修改全局 `PATH`、shell 配置或系统工具链，会对其他构建任务造成污染。

## 变更内容
1. 采用独立构建目录和命令级环境注入的方式执行 `cmake` 与 `make`，确保编译环境仅对当前命令生效。
2. 识别并修复导致交叉编译失败的首个真实问题，优先进行最小范围的构建脚本或源码修复。
3. 将可复用的隔离构建方法和修复结果同步到 `helloagents/` 知识库。

## 影响范围
- **模块:** BuildRuntime、构建脚本、必要时涉及首个失败点所在源码模块
- **文件:** `rk_gb/build.sh`、`rk_gb/CMakeLists.txt`、`rk_gb/Middleware/CMakeLists.txt`、首个失败点相关文件、`helloagents/wiki/modules/build_runtime.md`、`helloagents/CHANGELOG.md`、`helloagents/history/index.md`
- **API:** 无对外 API 变更
- **数据:** 无持久化数据结构变更

## 核心场景

### 需求: 隔离交叉编译通过
**模块:** BuildRuntime
在不污染当前机器其他 SoC 编译工作的前提下，完成 `rk_gb` 的交叉编译配置与构建。

#### 场景: 使用本地 RK 工具链独立构建
条件描述:
- 工具链位于 `/home/jerry/silver/RK/arm-rockchip830-linux-uclibcgnueabihf/bin`
- 构建目录独立于已有 `cmake-build`
- 环境变量仅对当前构建命令生效
- 预期结果: `cmake` 能找到交叉编译器
- 预期结果: 构建不依赖修改系统级 `PATH` 或用户 shell 配置
- 预期结果: 产出可复用的独立构建命令

#### 场景: 修复首个阻断编译问题
条件描述:
- 独立构建过程中出现真实编译或链接错误
- 允许对项目内脚本或源码做最小修复
- 预期结果: 首个阻断问题被定位并修复
- 预期结果: 重试后构建继续推进，直到交叉编译通过或暴露新的外部依赖阻塞

## 风险评估
- **风险:** 复用工程默认 `cmake-build` 或全局 `PATH` 会影响其他 SoC 项目
- **缓解:** 固定使用新 build 目录，并在单条命令上注入 `PATH`/`CC`/`CXX`
- **风险:** 工程脚本可能隐含对目录名或就地构建的假设
- **缓解:** 优先保留现有目录结构，只调整构建入口和必要最小改动
