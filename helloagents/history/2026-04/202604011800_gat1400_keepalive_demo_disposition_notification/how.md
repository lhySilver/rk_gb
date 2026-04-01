# 实现方案

## 代码调整
- 将 `GAT1400ClientService::SendKeepaliveDemoCaptureEvent()` 重命名为 `SendKeepaliveDemoDispositionNotification()`。
- 保留保活成功后的触发时机不变，但不再构造 `GAT1400CaptureEvent`。
- 直接构造一条 `GAT_1400_Disposition_Notification`，并调用 `PostDispositionNotifications()` 上送平台。

## 告警对象选择
- `DispositionNotification` 当前结构只支持 `PersonObject` / `MotorVehicleObject`，不支持 `FaceObject`。
- 为了保留现有人脸图片 demo，本次使用最小 `PersonObject`，并在 `SubImageList` 中携带 `IMAGE_FACE + FACE_DETECT_EVENT + Base64 Data`。
- demo 布控标识固定按 `DISPDEMO_<DeviceID>` 生成，便于联调时识别来源。
