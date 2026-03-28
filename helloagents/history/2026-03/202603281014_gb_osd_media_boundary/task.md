# 任务清单: GB OSD 配置职责下沉到媒体模块

目录: `helloagents/history/2026-03/202603281014_gb_osd_media_boundary/`

---

## 1. 媒体侧接口抽象
- [√] 1.1 新增 `App/Media/VideoOsdControl.h/.cpp`，定义媒体侧 OSD 配置/查询接口并承接当前 RK OSD 默认实现，验证 `why.md#变更目标`
- [√] 1.2 更新构建脚本，将 `VideoOsdControl.cpp` 纳入编译，依赖任务1.1

## 2. GB 模块改造
- [√] 2.1 在 `App/Protocol/ProtocolManager.cpp` 中移除 `rk_osd_*` 直连逻辑，改为调用媒体侧接口，验证 `why.md#需求-gb-设置-osd`
- [√] 2.2 在 `App/Protocol/ProtocolManager.h/.cpp` 中清理 `m_gb_osd_*` 运行态镜像，并调整查询应答走媒体侧运行态，验证 `why.md#需求-gb-查询-osd`

## 3. 安全检查
- [√] 3.1 检查 OSD 查询/设置回退逻辑，确认媒体接口返回不完整运行态时仍能使用现有 `m_cfg.gb_osd` 回显

## 4. 知识库更新
- [√] 4.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 4.2 更新 `helloagents/CHANGELOG.md`
- [√] 4.3 更新 `helloagents/history/index.md`

## 5. 验证
- [√] 5.1 执行最小语法编译验证，确认新增媒体接口和 `ProtocolManager` 改造无编译错误
> 备注: 已使用 `g++ -fsyntax-only -std=gnu++11` 分别对 `App/Media/VideoOsdControl.cpp` 与 `App/Protocol/ProtocolManager.cpp` 做语法级编译验证；`ProtocolManager.cpp` 编译时仍存在 `CMS1400Struct.h` 与 `GB28181Defs.h` 对 `GB_ID_LEN` 的既有重定义告警，但本次改动未新增该问题。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
