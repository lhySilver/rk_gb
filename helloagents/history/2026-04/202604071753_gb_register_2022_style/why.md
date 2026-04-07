# 变更提案: GB 注册 2022 风格对齐

## 需求背景
当前设备注册主链仍按 `GB/T 28181-2016` 口径运行，注册请求没有显式携带 `2022` 新增的标准协议版本标识。此前已经确认，`2022` 在注册阶段新增了 `X-GB-Ver` 头域要求，版本号 `3.0` 对应 `GB/T 28181-2022`。如果设备继续只发旧式注册，平台侧无法明确识别设备的协议版本能力。

## 变更内容
1. 给注册流程补上标准 `X-GB-Ver: 3.0` 头域。
2. 在 SDK 内补充 `kGB2022Version`，并让现有 `2016` 分支逻辑兼容 `2022`。
3. 将当前 GB 客户端启动版本和默认注册版本号切到 `2022` 对应口径。

## 影响范围
- **模块:** `ProtocolManager`、`GB28181SDK`、`SipSDK`、本地配置默认值、GB28181 知识库
- **文件:** `App/Protocol/ProtocolManager.cpp`、`App/Protocol/config/ProtocolExternalConfig.h`、`App/Protocol/config/LocalConfigProvider.cpp`、`third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h`、`third_party/platform_sdk_port/CommonFile/CommonLib/SipDefs.h`、`third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp`、`third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/client/SipClientImpl.cpp`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/ClientInfoManager.h`、`third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/SipEventManager.cpp`
- **API:** 无对外接口新增，属于已有注册流程行为调整
- **数据:** 无新增持久化结构，仅调整默认值

## 核心场景

### 需求: 设备以 2022 风格发起注册
**模块:** GB28181
设备向平台注册时，应显式带上 `2022` 对应的标准版本头。

#### 场景: 首次注册与鉴权重注册
设备首次 `REGISTER` 以及收到 `401` 后的鉴权 `REGISTER`
- 预期结果: 两次 `REGISTER` 都携带 `X-GB-Ver: 3.0`
- 预期结果: 零配置白皮书头继续保留，不和标准头互斥

#### 场景: 2022 版本下继续复用现有 2016 扩展字段逻辑
设备运行在 `kGB2022Version`
- 预期结果: `Keepalive` 等现有 `2016` 扩展字段逻辑继续可用
- 预期结果: 不引入额外复杂状态机

## 风险评估
- **风险:** 直接切到 `2022` 版本后，SDK 内部对 `2016` 的条件判断可能失效
- **缓解:** 将当前仅有的 `2016` 特性判断改成“`2016` 及以上版本”复用
