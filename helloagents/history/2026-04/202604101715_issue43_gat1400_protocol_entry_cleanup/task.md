# 任务清单: issue 43 GAT1400 对外接口收口

目录: `helloagents/history/2026-04/202604101715_issue43_gat1400_protocol_entry_cleanup/`

---

## 1. 代码调整
- [√] 1.1 在 `rk_gb/App/Protocol/ProtocolManager.*` 中新增 `NotifyGatFaces()`、`NotifyGatMotorVehicles()`、`NotifyGatNonMotorVehicles()` 3 个 1400 结构化对象上报入口
- [√] 1.2 在 `rk_gb/App/Protocol/gat1400/GAT1400ClientService.*` 中新增对应的发送逻辑：已注册直接发送，未注册直接进入现有失败补传队列
- [√] 1.3 删除 `rk_gb/App/Protocol/ProtocolManager.*` 中的 `NotifyGatAlarm()` 对外入口，避免继续暴露错误的抓拍通知调用面
- [√] 1.4 删除 `rk_gb/App/Protocol/gat1400/GAT1400ClientService.*` 中 keepalive demo 函数、状态变量和 demo 专用补传过滤分支
- [√] 1.5 删除 `rk_gb/App/Protocol/gat1400/GAT1400CaptureControl.*`、`GAT1400CaptureEvent` 与 `GAT1400ClientService::NotifyCaptureEvent()/PostCaptureEvent()/DrainPendingCaptureEvents()` 这条已无调用方的抓拍桥接链路，并同步收口构建脚本

## 2. 知识库同步
- [√] 2.1 更新 `helloagents/wiki/modules/gat1400.md`
- [√] 2.2 更新 `helloagents/wiki/api.md`
- [√] 2.3 更新 `helloagents/wiki/data.md`
- [√] 2.4 更新 `helloagents/wiki/modules/protocol.md`
- [√] 2.5 更新 `helloagents/CHANGELOG.md`
- [√] 2.6 更新 `helloagents/history/index.md`

## 3. 验证
- [√] 3.1 执行 `git -C /home/jerry/silver/rk_gb diff --check`
- [√] 3.2 基于现有 `flags.make` 映射当前工作区路径后，执行 `g++ -std=gnu++11 -fsyntax-only`，覆盖 `ProtocolManager.cpp` 与 `GAT1400ClientService.cpp`
- [√] 3.3 执行符号级回归检查，确认工作区中不再存在 keepalive demo、`NotifyGatAlarm()`、`GAT1400CaptureControl` 与 `NotifyCaptureEvent()` 当前态引用

## 执行结果
- `ProtocolManager` 已提供给外部模块直接调用的人脸、机动车、非机动车 3 个 1400 上报接口。
- `ProtocolManager::NotifyGatAlarm()` 已删除；当前对外只保留 3 个结构化对象接口。
- `GAT1400ClientService` 现区分“已注册直发”和“未注册直接落失败补传队列”两种处理，避免外部模块再去碰底层 `Post*` 发送细节。
- keepalive demo 相关函数、状态变量和 demo 专用补传过滤逻辑已删除，保活链路不再夹带联调样例上传。
- `GAT1400CaptureControl`、`GAT1400CaptureEvent` 与 `NotifyCaptureEvent()` 抓拍桥接链路已整体删除；当前运行态不再保留一个仅供 `GAT1400ClientService` 自用、但仍暴露在模块外部的假接口。
- `cmake --build /home/jerry/silver/rk_gb/cmake-build --target dgiot -j2` 未执行成功；当前 `cmake-build/CMakeCache.txt` 仍指向旧路径 `/home/lhy/share/ipc/RK/temp/lhy/0310/rk/rc0240-yuanshi`，因此本次改动改用单文件语法检查完成最小验证。
