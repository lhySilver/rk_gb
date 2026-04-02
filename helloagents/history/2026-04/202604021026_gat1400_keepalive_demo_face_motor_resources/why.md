# 需求说明

## 背景
- 对方研发已明确 keepalive demo 的资源上报路径和 JSON 样例：人脸应走 `/VIID/Faces`，机动车应走 `/VIID/MotorVehicles`。
- 现有 demo 仍走 `POST /VIID/DispositionNotifications`，与当前联调口径不一致。

## 目标
- 将 keepalive demo 改为保活成功后直接上报一条人脸资源和一条机动车资源。
- `Data` 字段继续沿用当前 Base64 语义，不改 `/Data` 上传逻辑。
- JSON 尽量贴近平台给出的 `2.png` 和 `1.txt` 样例，同时保持嵌入式 demo 的实现简单。

## 影响范围
- `App/Protocol/gat1400/GAT1400ClientService.cpp/.h`
- `third_party/platform_sdk_port/CommonLibSrc/GAT1400SDK/include/GAT1400Json.cpp`
- `helloagents/wiki/modules/gat1400.md`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
