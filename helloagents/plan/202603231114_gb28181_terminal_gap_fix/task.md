# 任务清单: GB28181终端白皮书差距收敛

目录: `helloagents/plan/202603231114_gb28181_terminal_gap_fix/`

---

## 1. 配置与数据模型扩展
- [ ] 1.1 在 `rk_gb/App/Protocol/config/ProtocolExternalConfig.h` 中补充 `VoiceFlowMode/LocalPort/Domain/SipIp/SipPort/SipId/Password/ChannelList` 模型，验证 `why.md#需求-基础参数查询与配置扩展`
- [ ] 1.2 在 `rk_gb/App/Protocol/config/HttpConfigProvider.cpp` 与 `rk_gb/App/Protocol/config/LocalConfigProvider.cpp` 中接入上述字段读取与默认值策略，验证 `why.md#需求-基础参数查询与配置扩展-场景-平台查询或下发-basicparam`，依赖任务1.1
- [ ] 1.3 在 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` 中扩展 `DeviceInfo` / `CfgBasicParam` / `BasicSetting` 等共享结构，验证 `why.md#需求-设备信息查询扩展` 与 `why.md#需求-基础参数查询与配置扩展`，依赖任务1.1

## 2. 广播链路与公网 TCP active
- [ ] 2.1 在 `rk_gb/App/Protocol/gb28181/GB28181BroadcastBridge.cpp` 中统一 `VoiceFlowMode=1 -> TCP active` 的协商映射，并明确回退策略，验证 `why.md#需求-公网语音广播闭环`
- [ ] 2.2 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中梳理广播建立流程，确保发流目标取自平台 SDP 应答并在 `ACK` 合法后启动推流，验证 `why.md#需求-公网语音广播闭环-场景-平台公网下发广播`，依赖任务2.1
- [ ] 2.3 对广播 `ACK`、SDP 方向属性和媒体目标地址补充日志关键字，便于抓包和日志对照排障，验证 `why.md#需求-公网语音广播闭环-场景-平台公网下发广播`，依赖任务2.2

## 3. 注册重定向与设备信息应答
- [ ] 3.1 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp` 中定位并补齐首个重定向 `REGISTER` 的扩展身份字段与身份切换逻辑，验证 `why.md#需求-注册重定向扩展闭环`
- [ ] 3.2 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/device_info.*` 与 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp` 中补齐 `DeviceInfo` 扩展字段与最小能力清单编码，验证 `why.md#需求-设备信息查询扩展-场景-平台查询-deviceinfo`，依赖任务1.3
- [ ] 3.3 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中补齐 `DeviceInfo` / `ConfigDownload` 的运行时取值装配，验证 `why.md#需求-设备信息查询扩展` 与 `why.md#需求-基础参数查询与配置扩展-场景-平台查询或下发-basicparam`，依赖任务1.2, 3.2

## 4. 第二阶段能力补齐
- [ ] 4.1 在 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` 与 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/alarm_notify.*` 中补齐扩展告警结构承载，验证白皮书 `A.11` 路径，依赖任务1.3
- [ ] 4.2 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 及相关业务事件模块中接入升级失败、文件上传失败、断电等告警事件源，并区分是否需要报警复位，验证白皮书 `A.11` 路径，依赖任务4.1
- [ ] 4.3 在 `rk_gb/App/Protocol/config/*` 和相关码流能力声明链路中把多码流能力提升到“三码流及以上”，验证白皮书 `4.7.9` 和附录 `G`，依赖任务1.2

## 5. 验证与知识库同步
- [ ] 5.1 准备最小抓包用例：重定向注册、广播 `INVITE/200/ACK/发流`、`DeviceInfo`、`BasicParam`、扩展告警，并明确每个检查点的关键字段
- [ ] 5.2 实施完成后更新 `helloagents/wiki/modules/gb28181.md`、`helloagents/wiki/modules/terminal_requirements.md`、`helloagents/CHANGELOG.md`、`helloagents/history/index.md`
- [ ] 5.3 将本方案包迁移到 `helloagents/history/YYYY-MM/` 并补充最终验证结论，依赖任务1.1-5.2
