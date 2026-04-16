# BuildRuntime

## 目的
说明项目目录职责、构建入口、交叉编译工具链和运行启动链。

## 模块概述
- **职责:** 区分 `RK/` 与 `rk_gb/`；描述主程序启动顺序和协议初始化入口
- **状态:** ✅稳定
- **最后更新:** 2026-04-16
- **关键入口:** `PROTOCOL_CONFIG_ENDPOINT = http://127.0.0.1:18080/openapi/v1/ipc/protocol`
- **隔离构建结论:** 不要直接用 `rk_gb/build.sh`，优先使用独立 build 目录和命令级 `PATH` 注入

## 规范

### 需求: 分析 RK IPC 项目结构
**模块:** BuildRuntime
明确工具链目录、业务代码目录、构建入口与运行入口，避免把第三方或工具链误判为主逻辑。

#### 场景: 代码初读
前置条件:
- 仓库同时包含 `RK/` 和 `rk_gb/`
- 需要快速确定真正的设备应用入口
- 预期结果1: 明确 `RK/` 是交叉编译工具链
- 预期结果2: 明确 `rk_gb/` 是业务源码主体
- 预期结果3: 明确应用入口和协议启动点

#### 场景: 本机隔离交叉编译
前置条件:
- 宿主机不允许修改系统级 `PATH`、`~/.bashrc` 或全局工具链安装
- 本地工具链位于 `RK/arm-rockchip830-linux-uclibcgnueabihf/bin`
- `rk_gb/build.sh` 会写入固定目录 `cmake-build`、`Middleware/cmake-build`，不适合作为隔离入口
- 预期结果1: `Middleware` 使用独立目录 `rk_gb/Middleware/build-rk830`
- 预期结果2: 主工程使用独立目录 `rk_gb/build-rk830`
- 预期结果3: 仅当前命令临时注入工具链路径，其他 SoC 构建环境不受影响
- 预期结果4: 最终产物为 `rk_gb/Bin/dgiot`

### 需求: 协议基线叠加 dg_ipc 板级适配
**模块:** BuildRuntime
在 `GB28181/GAT1400` 协议线继续前进的前提下，回放 `feature/dg_ipc` 的 IPC 适配改动，避免把 `dg_ipc` 误当成新的协议基线。

#### 场景: replay 分支整理
前置条件:
- 本地 `feature/gb-zero-config-macro-switch-20260326` 与远端协议分支各自多出 `1` 个提交
- `feature/dg_ipc` 的核心适配提交为 `0fb527e`
- 仓库长期版本化管理板级库、固件和打包资源，不能把所有二进制一律当作构建垃圾删除
- 预期结果1: 新分支先补齐协议线本地/远端独有提交，再吸收 `dg_ipc` 改动
- 预期结果2: 只清理 `cmake-build`、调试目录、压缩包、资料类等明显残留
- 预期结果3: 新 IPC 对应的构建选项、板型和打包目录关系被保留

## API接口

### main -> CSofia::preStart
**描述:** 设备正常启动主流程入口。
**输入:** 进程参数
**输出:** 初始化系统、媒体、网络、协议、ONVIF 等模块

## 数据模型

### 目录角色
| 字段 | 类型 | 说明 |
|------|------|------|
| `RK/` | 目录 | Rockchip 交叉编译工具链 |
| `rk_gb/` | 目录 | 设备业务代码主体 |
| `rk_gb/App/` | 目录 | 应用和协议桥接层 |
| `rk_gb/third_party/` | 目录 | 第三方库与移植 SDK |
| `rk_gb/build-rk830/` | 目录 | 主工程独立构建目录 |
| `rk_gb/Middleware/build-rk830/` | 目录 | 中间件独立构建目录 |

### 隔离构建命令
| 步骤 | 命令 |
|------|------|
| 中间件配置 | `env PATH="/home/jerry/silver/RK/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH" CC=arm-rockchip830-linux-uclibcgnueabihf-gcc CXX=arm-rockchip830-linux-uclibcgnueabihf-g++ /home/jerry/silver/.tools/cmake-4.2.3-linux-x86_64/bin/cmake -S /home/jerry/silver/rk_gb/Middleware -B /home/jerry/silver/rk_gb/Middleware/build-rk830 -DRV1106_DUAL_IPC=ON -DRC0240_LGV10=ON -Drelease=ON -DCMAKE_C_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-g++` |
| 中间件编译 | `env PATH="/home/jerry/silver/RK/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH" /home/jerry/silver/.tools/cmake-4.2.3-linux-x86_64/bin/cmake --build /home/jerry/silver/rk_gb/Middleware/build-rk830 -j4` |
| 主工程配置 | `env PATH="/home/jerry/silver/RK/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH" CC=arm-rockchip830-linux-uclibcgnueabihf-gcc CXX=arm-rockchip830-linux-uclibcgnueabihf-g++ /home/jerry/silver/.tools/cmake-4.2.3-linux-x86_64/bin/cmake -S /home/jerry/silver/rk_gb -B /home/jerry/silver/rk_gb/build-rk830 -DRV1106_DUAL_IPC=ON -DRC0240_LGV10=ON -DAIC8800DL=ON -Drelease=ON -DCMAKE_C_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-rockchip830-linux-uclibcgnueabihf-g++` |
| 主工程编译 | `env PATH="/home/jerry/silver/RK/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH" /home/jerry/silver/.tools/cmake-4.2.3-linux-x86_64/bin/cmake --build /home/jerry/silver/rk_gb/build-rk830 -j4` |

## 实现要点
- `rk_gb/build.sh` 不适合隔离构建，因为会固定使用源码树内的 `cmake-build`/`Middleware/cmake-build`，并伴随清理逻辑
- `rk_gb/CMakeLists.txt` 与 `rk_gb/Middleware/CMakeLists.txt` 原先把优化选项误写为 `-o3`，会导致 GCC 将其解释为输出文件参数，进而报 `cc1: error: too many filenames given`
- 两个入口 CMake 已修正为 `-O3`，并将 `cmake_minimum_required` 提升到 `3.5`
- 当前宿主机没有系统级 `cmake`，本次使用工作区私有工具 `/home/jerry/silver/.tools/cmake-4.2.3-linux-x86_64/bin/cmake`
- 交叉编译结果已验证为 ARM 目标: `ELF 32-bit LSB executable, ARM, EABI5, interpreter /lib/ld-uClibc.so.0`
- GitHub issue repair 工作流复用了同一套隔离交叉编译命令，并封装在 `rk_gb/tools/issue_bot/build_verify.sh`
- 自动化 repair 必须运行在专用 self-hosted runner 上，使用独立 worktree 和临时 build 根目录，避免污染人工开发工作区
- `dg_ipc` 整理分支采用“协议线独有提交先合流，再回放 `0fb527e`”的顺序，避免遗漏 `feature/gb-zero-config-macro-switch-20260326` 分叉后继续前进的协议修复
- 当前 `build.sh` 已补充 `RC0240/RC0240V20/RC0240V30/RC0240V40` 板型选择，默认 IPC 打包目录与 `packaging/`、`packaging-$BOARD_TYPE` 相关联
- 当前 `feature/dg_ipc_replay_20260415` 分支已将默认 `packaging/` 收敛为 `packaging.tar.xz`；`build.sh` 会在目录缺失但归档存在时自动解压恢复，再继续执行原有 `make -C $PACKAGING`
- 对这类仓库进行分支整理时，`cmake-build/`、`Middleware/cmake-build/`、调试目录、压缩包和资料文件属于可直接清理项；板级库、固件、rootfs、打包脚本需按项目既有版本管理口径保留

## 依赖
- `rk_gb/CMakeLists.txt`
- `rk_gb/build.sh`
- `rk_gb/envsetup.sh`
- `rk_gb/App/CMakeLists.txt`
- `rk_gb/Middleware/CMakeLists.txt`
- `rk_gb/App/Main.cpp`
- `rk_gb/App/Protocol/ProtocolManager.cpp`
- `rk_gb/packaging.tar.xz`
- `rk_gb/packaging-RC0240_LGV10/`

## 变更历史
- 2026-03-12: 修复主工程和 Middleware 的 `-o3` 构建参数错误，补齐隔离交叉编译命令，验证 `rk_gb/Bin/dgiot` 交叉编译通过
- 2026-04-15: 以协议基线回放 `feature/dg_ipc` 的 IPC 适配改动，记录板型、打包目录与“只清理明显构建残留”的分支整理原则
- 2026-04-16: 将默认 `packaging/` 目录收敛为 `packaging.tar.xz`，并在 `build.sh` 中增加缺目录时的自动解压恢复逻辑
