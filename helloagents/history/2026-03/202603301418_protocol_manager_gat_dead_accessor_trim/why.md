# 需求说明: ProtocolManager / GAT 死 accessor 瘦身

## 背景
- 用户要求继续生成一轮 `dgiot` 体积缩减方案包，约束是“无行为变化”，验收目标是最终 `dgiot` 变小。
- 最近几轮瘦身已经连续清理了 `ProtocolManager` 中的死包装方法、私有 helper 和重复声明，但在 GB / GAT 交界处仍残留少量“只声明/定义、不参与真实调用链”的 accessor 壳代码。
- 当前全仓搜索显示：
  - `ProtocolManager::GetGatClientService() const` 只出现在声明/定义和 `LowerGAT1400SDK.cpp` 的本地 class skeleton 中，实际运行路径走的是非 `const` 版本。
  - `ProtocolManager::GetGbClientReceiver() const` 只出现在声明/定义处，实际绑定 SDK receiver 时走的是非 `const` 版本。
  - `LowerGAT1400SDK.cpp` 的本地 `ProtocolManager` 声明里还保留了一个未被使用的 `const GetGatClientService()` 前置声明。

## 目标
- 删除 `ProtocolManager` 与 GAT lower adapter 之间零调用的 `const accessor` / 冗余声明，继续收紧协议胶水层。
- 保持 `LOWER_1400_*` 导出接口、GB28181 SDK 绑定、GAT1400 服务获取路径和运行时行为不变。
- 在可构建环境中验证 `dgiot` 体积继续减小；若整工程构建受阻，至少记录相关目标文件的减小结果。

## 变更内容
1. 删除 `ProtocolManager` 中零调用的 `const GetGatClientService()` 与 `const GetGbClientReceiver()` 声明/实现。
2. 删除 `LowerGAT1400SDK.cpp` 中与之对应的无用 `const` 前置声明，保留真实使用的非 `const` 服务获取路径。
3. 以“先搜引用、再删符号、后做尺寸验证”的顺序执行，避免把潜在外部依赖误判为死代码。

## 影响范围
- **模块:** `ProtocolManager`、`LowerGAT1400SDK`、GAT1400 运行时导出层
- **文件:** `rk_gb/App/Protocol/ProtocolManager.h`、`rk_gb/App/Protocol/ProtocolManager.cpp`、`rk_gb/App/Protocol/gat1400/LowerGAT1400SDK.cpp`
- **API:** 不改变 `LOWER_1400_*` 导出函数和运行时协议行为；仅收窄仓库内未被实际使用的 accessor 公开面
- **数据:** 无

## 核心场景

### 需求: 保持 GAT lower adapter 取服务实例路径不变
**模块:** GAT1400
当前 `LowerGAT1400SDK.cpp` 通过 `ProtocolManager::Instance()` 获取运行态服务实例，这条实际调用链不能受本次瘦身影响。

#### 场景: LOWER_1400_* 导出层获取 GAT 服务
前置条件:
- `LOWER_1400_START/STOP/REGISTER/...` 通过本地 helper 转到 `ProtocolManager::Instance()`
- `ProtocolManager` 已持有 `m_gat_client`
- 预期结果1: 仍然通过非 `const` `GetGatClientService()` 取得服务实例
- 预期结果2: `LOWER_1400_*` 的行为和返回码不变

### 需求: 保持 GB28181 client receiver 绑定路径不变
**模块:** Protocol
`BindGbClientSdk()` 当前通过 `GetGbClientReceiver()` 把 `m_gb_receiver` 绑定给 SDK，本次仅允许清理未使用的 `const` 重载。

#### 场景: GB SDK 绑定 receiver
前置条件:
- `BindGbClientSdk()` 被正常调用
- `m_gb_client_sdk` 已创建
- 预期结果1: 仍然绑定同一个 `m_gb_receiver`
- 预期结果2: 不改变 GB 注册、保活、查询、媒体协商行为

## 风险评估
- **风险:** `ProtocolManager` 头文件中的 `const` accessor 理论上可能被仓库外代码编译依赖
- **缓解:** 执行前先做全仓引用搜索；若发现真实调用或外部接口约束不清，则缩小到仅删除 `LowerGAT1400SDK.cpp` 的冗余声明，不贸然收窄头文件接口
