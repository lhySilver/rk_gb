# 任务清单: 补齐 GB 画面反转控制链路的运行态查询闭环

目录: `helloagents/history/2026-03/202603281505_gb_image_flip_control_runtime_query/`

---

## 1. 现状确认
- [√] 1.1 确认 `ProtocolManager::ResponseGbQueryConfig()` 已显式调用 `media::QueryVideoImageFlipMode()`
- [√] 1.2 确认 `ProtocolManager::HandleGbConfigControl()` 控制侧此前只显式调用 `media::ApplyVideoImageFlipMode()`
- [√] 1.3 确认 `VideoImageControl` 仍是当前唯一设备侧接口面

## 2. 代码改动
- [√] 2.1 在 `HandleGbConfigControl()` 的 `kImageSetting` 路径中补入显式运行态查询
- [√] 2.2 在 `applyImageFlip` 阶段按运行态决定是否真正 dispatch 设置

## 3. 知识库更新
- [√] 3.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 3.2 更新 `helloagents/CHANGELOG.md`
- [√] 3.3 更新 `helloagents/history/index.md`
- [√] 3.4 新增本次变更历史包

## 4. 验证
- [ ] 4.1 执行 `git diff --check`
- [ ] 4.2 执行 `ProtocolManager.cpp` 最小编译验证

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
