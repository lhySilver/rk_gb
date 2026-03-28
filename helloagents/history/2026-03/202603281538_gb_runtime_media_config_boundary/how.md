# 技术设计: 收口 GB/1400 对外部模块配置的运行态缓存

## 技术方案
### 核心技术
- 在 `App/Media/*Control` 中补齐运行态读取能力，必要时由媒体模块自己维护最小逻辑缓存，供协议层查询。
- `ProtocolManager::HandleGbConfigControl()` 改为基于媒体运行态做 delta merge / compare / apply，不再回写 `m_cfg.gb_video / gb_image / gb_osd`。
- `ProtocolManager::ResponseGbQueryConfig()` 与能力回包改为优先直接使用媒体运行态，不再以 `m_cfg.gb_*` 作为正常返回来源。
- `GAT1400ClientService` 只缓存 1400 自己的配置；涉及 GB 注册派生值时，通过 `ProtocolManager::InstanceIfCreated()` 现取。

## 实现要点
- `App/Media/VideoOsdControl.*`
  - 为 `event/alert` 等设备无原生 getter 的字段下沉最小缓存
  - 让 `QueryVideoOsdState()` 能返回协议回显所需的完整逻辑状态
- `App/Media/VideoImageControl.*`
  - 在媒体侧保留最后一次成功设置的翻转模式兜底，避免 GB 侧再保留镜像缓存
- `App/Protocol/ProtocolManager.cpp`
  - 移除启动/重载阶段对 `gb_image` 的主动下发
  - `HandleGbConfigControl()` 改为使用局部目标值和媒体运行态
  - `ResponseGbQueryConfig()` / `BuildGbStreamParamCapabilityValue()` 改为按媒体运行态组包
- `App/Protocol/gat1400/GAT1400ClientService.*`
  - 收口 `SnapshotConfig()`，移除 `m_gb_register / m_device_id` 依赖
  - 新增按需解析当前 1400 设备 ID 的 helper

## 架构决策 ADR
### ADR-20260328-03: 协议层不再保存外部模块配置的运行态缓存
**上下文:** 用户明确要求“其他模块的配置，GB 模块和 1400 模块都不应该保存缓存，直接去 get 就好了”，同时 OSD/编码/画面反转已存在清晰的媒体边界。
**决策:** 协议层只做协议字段映射与调度；运行态以媒体/编码模块的 `get/set` 为准，必要缓存下沉到能力归属模块内部。
**理由:** 这样可以避免 GB/1400 继续维护一份与设备实际状态脱节的副本，也让后续媒体模块同事接手实现时边界更稳定。

## 测试与部署
- 使用 `git diff --check` 校验补丁格式
- 当前环境无 `cmake` / 交叉编译器可执行，实际改为复用 `cmake-build/App/CMakeFiles/dgiot.dir/flags.make` 中的 `define/include`，用宿主机 `g++ -fsyntax-only` 对本次改动的协议/媒体源文件做语法编译验证
