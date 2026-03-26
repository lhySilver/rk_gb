# 任务清单: GB28181 零配置宏开关与注册重定向闭环

目录: `helloagents/history/2026-03/202603261707_gb_zero_config_macro_switch/`

---

## 1. 方案开关与配置模型
- [√] 1.1 在 `rk_gb/App/CMakeLists.txt`、`rk_gb/App/Protocol/gb28181/sdk_port/CMakeLists.txt` 和公共头文件中落下 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 编译期开关，验证 `why.md#需求-编译期切换-gb-标准零配置流程`
- [√] 1.2 在 `rk_gb/App/Protocol/config/ProtocolExternalConfig.h`、`rk_gb/App/Protocol/config/LocalConfigProvider.*` 中扩展 `StringCode/Mac/Line/redirect_domain/redirect_server_id/CustomProtocolVersion/manufacturer/model` 的本地配置读写，验证 `why.md#需求-首次-register-携带零配置身份和扩展头`

## 2. SIP 与 GB 注册状态机
- [√] 2.1 在 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/SipDefs.h`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/SipSDK/common/*`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/SipSDK/client/SipClientImpl.cpp` 中补齐 REGISTER 扩展头承载和 `302` 响应字段解析，验证 `why.md#需求-首次-register-携带零配置身份和扩展头` 与 `why.md#需求-收到-302-后切换正式平台完成注册`
- [√] 2.2 在 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` 与 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.*` 中实现零配置单次注册事务：`StringCode -> 401 -> 302 -> 正式平台注册 -> 401 -> 200`，验证 `why.md#需求-收到-302-后切换正式平台完成注册-场景-重定向服务器返回正式平台信息`

## 3. ProtocolManager 生命周期接线
- [√] 3.1 在 `rk_gb/App/Protocol/ProtocolManager.*` 中用宏切标准/零配置参数装配，并在心跳线程补 30 秒重试、3 次失败后 1 分钟重取重定向地址的节奏，验证 `why.md#需求-编译期切换-gb-标准零配置流程-场景-宏开启` 与 `why.md#需求-零配置模式下的失败重试`

## 4. 安全检查
- [√] 4.1 检查零配置新增日志和配置持久化，确保密码继续打码且未引入非法端口/空地址写入

## 5. 文档更新
- [√] 5.1 更新 `rk_gb/helloagents/wiki/modules/zero_config.md`
- [√] 5.2 更新 `rk_gb/helloagents/wiki/modules/gb28181.md`
- [√] 5.3 更新 `rk_gb/helloagents/wiki/modules/terminal_requirements.md`
- [√] 5.4 更新 `rk_gb/helloagents/CHANGELOG.md`

## 6. 测试
- [√] 6.1 进行最小静态验证或编译验证，确认宏关闭/开启两条路径至少能通过构建
- [√] 6.2 记录本轮仍未实现或待联调确认的零配置缺口，并在知识库中显式标注

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
