# 变更提案: 收口 GB/1400 对外部模块配置的运行态缓存

## 需求背景
代码审查继续指出，OSD、编码参数、画面反转这些能力本质上属于媒体/编码模块，但当前 `ProtocolManager` 仍在 `m_cfg.gb_osd / gb_video / gb_image` 中保留并回写运行态；`GAT1400ClientService` 也保留了 `m_gb_register / m_device_id` 这类跨模块派生缓存。这样会让协议层和能力归属边界继续混在一起。

## 变更目标
1. GB28181 查询/设置外部模块配置时，统一走媒体/编码侧 `get/set`，不再把运行态维护在 `ProtocolManager::m_cfg.gb_*` 中。
2. 对于媒体侧无法直接从设备读取但协议又需要回显的字段，把最小缓存下沉到媒体模块自身，而不是放在 GB 模块。
3. GAT1400 运行时需要 GB 注册信息或设备 ID 时，改为按需从 `ProtocolManager` 获取，不再长期缓存跨模块派生值。

## 影响范围
- **模块:** `GB28181`、`GAT1400`、`Media`
- **文件:** `App/Protocol/ProtocolManager.cpp`、`App/Protocol/gat1400/GAT1400ClientService.*`、`App/Media/VideoOsdControl.*`、必要的媒体控制实现与知识库
- **行为边界:** 不改变 GB/GAT 自身注册配置；只调整“其他模块配置”的运行态归属和读取方式
