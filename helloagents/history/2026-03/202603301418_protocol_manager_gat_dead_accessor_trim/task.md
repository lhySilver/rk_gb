# 任务清单: ProtocolManager / GAT 死 accessor 瘦身

目录: `helloagents/history/2026-03/202603301418_protocol_manager_gat_dead_accessor_trim/`

---

## 1. 死代码确认
- [√] 1.1 在 `rk_gb/App/Protocol/ProtocolManager.h/.cpp` 与 `rk_gb/App/Protocol/gat1400/LowerGAT1400SDK.cpp` 中复核 `GetGatClientService() const`、`GetGbClientReceiver() const` 的引用情况，确认只有声明/定义或壳声明命中
- [-] 1.2 若发现真实 `const` 调用链或仓库内接口依赖，先回退方案范围并同步修正 `why.md` / `how.md`
> 备注: 实际未发现真实 `const` 调用链，未触发回退范围条件。

## 2. 代码瘦身
- [√] 2.1 在 `rk_gb/App/Protocol/ProtocolManager.h/.cpp` 删除零调用的 `const GetGatClientService()` 与 `const GetGbClientReceiver()` 声明/实现，仅保留真实使用的非 `const` accessor
- [√] 2.2 在 `rk_gb/App/Protocol/gat1400/LowerGAT1400SDK.cpp` 删除对应的冗余 `const` 前置声明，确保 `LOWER_1400_*` 仍沿用原有服务获取路径

## 3. 安全检查与知识库同步
- [√] 3.1 重新执行全仓搜索，确认被删符号不再有残留引用，且未误伤 GB / GAT 运行逻辑
- [√] 3.2 更新 `rk_gb/helloagents/wiki/modules/protocol.md`、`rk_gb/helloagents/wiki/modules/gat1400.md`、`rk_gb/helloagents/CHANGELOG.md`、`rk_gb/helloagents/history/index.md`

## 4. 体积验证与收尾
- [X] 4.1 优先按 `helloagents/wiki/modules/build_runtime.md` 的隔离构建方式重新编译 `dgiot`，记录瘦身前后体积，验收标准为 `dgiot` 继续减小
> 备注: 隔离交叉编译配置可以完成，但整工程构建被当前源码树 `rk_gb/CMakeLists.txt` 中的 `add_definitions(-o3)` 阻塞，报错为 `cc1: error: too many filenames given`。
- [√] 4.2 若整工程构建受 `cmake` / 交叉工具链 / 头文件环境阻塞，则改为对 `ProtocolManager.o` 与 `LowerGAT1400SDK.o` 做同宏条件下的尺寸对比，并在结果中明确说明阻塞原因
- [√] 4.3 执行完成后更新任务状态，并将方案包迁移到 `helloagents/history/2026-03/`

## 执行结果
- `ProtocolManager` 中零调用的 `const GetGatClientService()` / `const GetGbClientReceiver()` 已删除，`LowerGAT1400SDK.cpp` 的冗余 `const` 壳声明已同步清理。
- 全仓搜索后，源码树内只剩真实使用的非 `const` `GetGatClientService()` / `GetGbClientReceiver()` 路径。
- 同工具链、同宏条件下，`ProtocolManager.o` 的 `dec` 从 `167103` 降到 `167071`，减少 `32` 字节；`LowerGAT1400SDK.o` 的 `dec` 保持 `20468` 不变。
- 已尝试按隔离交叉编译路径重建 `dgiot`，但整工程仍被顶层 `-o3` 构建参数阻塞，未拿到最终二进制体积结果。
