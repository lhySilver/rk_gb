# 任务清单: 补齐 GB OSD / 编码参数控制链路的运行态查询闭环

目录: `helloagents/history/2026-03/202603281740_gb_osd_encode_control_runtime_query/`

---

## 1. 现状确认
- [√] 1.1 确认 OSD / 编码参数查询链路已分别显式调用 `QueryVideoOsdState()` / `QueryVideoEncodeState()`
- [√] 1.2 确认 OSD / 编码参数控制链路此前仍主要直接调用 `ApplyVideoOsdConfig()` / `ApplyVideoEncodeStreamConfig()`
- [√] 1.3 确认 `VideoOsdControl`、`VideoEncodeControl` 仍是当前唯一设备侧接口面

## 2. 代码改动
- [√] 2.1 扩展 `VideoOsdState`，补齐时间样式运行态字段
- [√] 2.2 在 `HandleGbConfigControl()` 中为 OSD 增加显式 `query -> compare -> apply`
- [√] 2.3 在 `HandleGbConfigControl()` 中为编码参数增加显式 `query -> compare -> apply`
- [√] 2.4 保持多条同码流设置命令可按字段 merge 后再统一下发

## 3. 知识库更新
- [√] 3.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 3.2 更新 `helloagents/CHANGELOG.md`
- [√] 3.3 更新 `helloagents/history/index.md`
- [√] 3.4 新增本次变更历史包

## 4. 验证
- [√] 4.1 执行 `git diff --check`
- [√] 4.2 重新生成隔离 `build-rk830` / `Middleware/build-rk830` 配置
- [√] 4.3 执行 `VideoOsdControl.cpp` / `ProtocolManager.cpp` 最小对象编译验证

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
