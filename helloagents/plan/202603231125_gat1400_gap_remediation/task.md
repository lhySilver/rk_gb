# 任务清单: GAT1400完成度补齐

目录: `helloagents/plan/202603231125_gat1400_gap_remediation/`

---

## 1. 配置与传输模型补齐
- [√] 1.1 在 `rk_gb/App/Protocol/config/ProtocolExternalConfig.h` 中扩展 `GatRegisterParam` / `GatUploadParam`，补入协议、路径、超时和待发送队列相关参数，验证 `why.md#需求-传输与接入模型补齐`
- [√] 1.2 在 `rk_gb/App/Protocol/config/HttpConfigProvider.cpp` 与 `rk_gb/App/Protocol/config/LocalConfigProvider.cpp` 中补齐上述字段的读取、回写和校验，验证 `why.md#需求-传输与接入模型补齐-场景-平台要求-https-或完整-url-接入`，依赖任务1.1
- [√] 1.3 在 `rk_gb/App/Protocol/gat1400/GAT1400ClientService.cpp` 中重构 URL/请求拼装逻辑，支持统一地址模型并保留当前 HTTP 兼容路径，验证 `why.md#需求-传输与接入模型补齐`，依赖任务1.2

## 2. 注册恢复与补传闭环
- [√] 2.1 在 `rk_gb/App/Protocol/gat1400/` 下新增统一待发送队列抽象，用于保存失败的 JSON/Data 上传请求，验证 `why.md#需求-注册与断链恢复闭环`
- [√] 2.2 在 `rk_gb/App/Protocol/gat1400/GAT1400ClientService.cpp` 中把 `PostJsonWithResponseList` / `PostJsonWithResponseStatus` / `PostBinaryData` 接入待发送队列和重放逻辑，验证 `why.md#需求-注册与断链恢复闭环-场景-保活失败后重新注册`，依赖任务2.1
- [√] 2.3 在 `rk_gb/App/Protocol/gat1400/GAT1400ClientService.cpp` 中把 `HeartbeatLoop` 和重新注册成功事件与队列回放打通，验证 `why.md#需求-注册与断链恢复闭环-场景-保活失败后重新注册`，依赖任务2.2

## 3. 业务接线与 SDK 适配
- [√] 3.1 确认 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/LowerGAT1400SDK.h` 的导出接口是否需要由本仓补实现；若需要，新增对应适配实现文件并映射到 `GAT1400ClientService`，验证 `why.md#需求-结构化对象可真实上报`
- [√] 3.2 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 或相关业务事件入口中接入至少一种真实上报路径，使 `Faces/Images` 或等价对象能够调用到 `Post*` 主链路，验证 `why.md#需求-结构化对象可真实上报-场景-业务侧触发人脸和图片上报`
- [√] 3.3 梳理订阅观察者注册与通知发送链路，确保 `/VIID/Subscribes` 的变化能被业务层消费并触发 `PostSubscribeNotifications`，验证 `why.md#需求-订阅接收与通知闭环-场景-平台新增订阅并收到通知`

## 4. 平台兼容增强
- [ ] 4.1 基于平台要求评估并实现 HTTPS/TLS 支持，至少明确当前是否支持、如何配置和失败时的日志提示，验证 `why.md#需求-传输与接入模型补齐`
- [√] 4.2 复核 `PostApes` 与标准 `GET/PUT /VIID/APEs` 的差异，确定是否保留为平台兼容扩展或改为条件启用，验证 GAT1400 标准基线与平台要求
- [√] 4.3 评估是否需要补 `/VIAS/*`；若平台不要求，则在知识库和配置能力中明确“当前不支持”，依赖任务4.1

## 5. 验证与知识库同步
- [ ] 5.1 建立最小 mock 联调用例，覆盖 `注册/401鉴权/保活失败/重注册/补传/Faces+Images/订阅新增与通知`
- [ ] 5.2 实施完成后更新 `helloagents/wiki/modules/gat1400.md`、`helloagents/wiki/api.md`、`helloagents/wiki/data.md`、`helloagents/CHANGELOG.md`、`helloagents/history/index.md`
- [ ] 5.3 将本方案包迁移至 `helloagents/history/YYYY-MM/` 并补充最终验证结论，依赖任务1.1-5.2
