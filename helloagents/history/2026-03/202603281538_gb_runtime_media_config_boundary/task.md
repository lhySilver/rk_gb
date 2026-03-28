# 任务清单: 收口 GB/1400 对外部模块配置的运行态缓存

目录: `helloagents/history/2026-03/202603281538_gb_runtime_media_config_boundary/`

---

## 1. 现状确认
- [√] 1.1 确认 GB 仍在 `m_cfg.gb_video / gb_image / gb_osd` 中保存并回写外部模块运行态
- [√] 1.2 确认 1400 仍缓存 `m_gb_register / m_device_id` 这类跨模块派生值
- [√] 1.3 确认媒体侧当前还缺哪些正式 `get` 来源

## 2. 代码改动
- [√] 2.1 扩展媒体侧运行态来源，必要缓存下沉到媒体模块
- [√] 2.2 改造 `ProtocolManager`，去掉对 `m_cfg.gb_video / gb_image / gb_osd` 的运行态依赖
- [√] 2.3 改造 `GAT1400ClientService`，去掉对 `m_gb_register / m_device_id` 的跨模块缓存依赖

## 3. 知识库更新
- [√] 3.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 3.2 更新 `helloagents/wiki/modules/gat1400.md`
- [√] 3.3 更新 `helloagents/CHANGELOG.md`
- [√] 3.4 更新 `helloagents/history/index.md`
- [√] 3.5 新增本次变更历史包

## 4. 验证
- [√] 4.1 执行 `git diff --check`
- [√] 4.2 基于 `cmake-build/App/CMakeFiles/dgiot.dir/flags.make` 执行宿主机 `g++ -fsyntax-only` 语法编译验证

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
