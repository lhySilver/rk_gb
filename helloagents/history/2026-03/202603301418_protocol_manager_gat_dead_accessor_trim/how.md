# 技术设计: ProtocolManager / GAT 死 accessor 瘦身

## 技术方案

### 核心技术
- C++ 现有协议桥接代码最小改动
- `rg` 全仓引用搜索
- 隔离交叉编译或目标文件尺寸对比验证

### 实现要点
- 先确认 `ProtocolManager::GetGatClientService() const`、`ProtocolManager::GetGbClientReceiver() const` 在源码树内没有真实调用点，只剩声明/定义和本地 class skeleton 引用。
- 若确认零调用，则从 `ProtocolManager.h/.cpp` 删除对应 `const` 重载，只保留当前真实使用的非 `const` accessor。
- 同步删除 `LowerGAT1400SDK.cpp` 本地 `ProtocolManager` 声明中的 `const GetGatClientService()`，避免 adapter translation unit 继续保留死声明。
- 保持 `LOWER_1400_*`、`BindGbClientSdk()`、`GetProtocolManagerInstance()`、`GetGatClientServiceInstance()` 的运行时路径不变，不引入逻辑重排。

## 架构设计
- 无新增模块、无新依赖、无流程变更。
- 本次仅收紧现有 GB / GAT 交界处的符号面和壳代码数量。

## 安全与性能
- **安全:** 不涉及配置写回、网络参数、认证逻辑、媒体会话状态、线程生命周期或外部系统连接。
- **性能:** 预期略微减小 `ProtocolManager.o` / `dgiot` 体积，并减少无用符号与声明噪音；不追求运行时性能变化。

## 测试与部署
- **测试:**  
  1. 对目标 accessor 做全仓引用搜索，确认删除后无剩余活引用。  
  2. 优先使用隔离交叉编译命令重新构建 `dgiot`，记录二进制体积前后变化。  
  3. 如整工程构建受当前环境阻塞，则退化为同编译宏下对相关目标文件做尺寸对比，并记录阻塞原因。
- **部署:** 无独立部署动作；按现有打包链路进入 `dgiot` 常规构建即可。

## 验证结果
- 全仓复核后，`GetGatClientService() const` 与 `GetGbClientReceiver() const` 只剩声明/定义命中；删除后源码树内仍只保留真实使用的非 `const` 访问路径。
- 按 `build_runtime.md` 的隔离交叉编译路径尝试重建 `dgiot`，但当前源码树顶层 `rk_gb/CMakeLists.txt` 仍带 `add_definitions(-o3)`，导致 CMake 生成的编译命令触发 `cc1: error: too many filenames given`，整工程构建未完成。
- 已退化为同工具链、同宏条件的单文件交叉编译验证：
  - `ProtocolManager.o` 的 `dec` 从 `167103` 降到 `167071`，减少 `32` 字节。
  - `LowerGAT1400SDK.o` 的 `dec` 保持 `20468` 不变。
