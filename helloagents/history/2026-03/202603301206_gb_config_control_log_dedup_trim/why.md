# 变更提案: 收敛 GB ConfigControl 中的重复日志展开

## 需求背景
上一轮已经清理了 `ProtocolManager.cpp` 中一批 OSD 多文本改动遗留的无效 helper 和重复日志，但
`ProtocolManager::HandleGbConfigControl()` 里仍然保留了两组较长的重复日志展开：
- `image_flip` 的 `skip_apply / dispatch`
- `video_param_attribute` 的 `skip_apply / dispatch`

这些日志参数几乎一致，只是动作名和是否带 `ret` 不同。继续把它们收口到 helper，可以在不改行为的前提下再压一轮目标文件体积。

## 变更目标
1. 保持日志文本和字段完全不变，只消除重复展开代码。
2. 继续把瘦身范围限制在 `ProtocolManager.cpp`，不扩散到媒体侧行为。
3. 用同编译宏下的单目标文件对比，确认这轮修改继续让 `ProtocolManager.o` 变小。

## 影响范围
- `rk_gb/App/Protocol/ProtocolManager.cpp`
- `rk_gb/helloagents/CHANGELOG.md`
- `rk_gb/helloagents/history/index.md`

## 风险评估
- 风险: helper 收口时误改日志文本，影响现有排障关键词。
- 缓解: 保持 `skip_apply` / `dispatch ret=%d` 两套原始格式不变，只把公共参数整理到 helper。
