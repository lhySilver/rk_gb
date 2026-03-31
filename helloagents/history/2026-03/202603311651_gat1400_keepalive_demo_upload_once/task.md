# 任务清单: GAT1400 保活后一次性 demo 抓拍上传

目录: `helloagents/history/2026-03/202603311651_gat1400_keepalive_demo_upload_once/`

---

## 1. 现状复核
- [√] 1.1 复核 `GAT1400ClientService` 的注册、保活和抓拍上传链路，确认 demo 挂点与一次性约束
- [√] 1.2 复核图片/视频上传字段与本体 `Data` 发送方式，确认修正元数据与 `/Data` 分离语义

## 2. 代码实现
- [√] 2.1 调整 `rk_gb/App/Protocol/gat1400/GAT1400ClientService.*`，修正图片/视频/文件元数据与 `/Data` 上传语义，并新增一次性 demo 上传状态
- [√] 2.2 在首次保活成功后触发 demo 人脸事件上传，只读取 `/mnt/sdcard/test.jpeg`

## 3. 验证与知识库同步
- [√] 3.1 执行最小静态验证，确认 `GAT1400ClientService` 语法正确
- [√] 3.2 更新 `rk_gb/helloagents/wiki/modules/gat1400.md`、`rk_gb/helloagents/CHANGELOG.md`、`rk_gb/helloagents/history/index.md`

## 4. 收尾
- [√] 4.1 更新任务状态与执行结果
- [√] 4.2 将方案包迁移到 `rk_gb/helloagents/history/2026-03/`

## 执行结果
- `GAT1400ClientService::SendKeepaliveNow()` 现会在首次保活成功后触发一次本地 demo 上传；demo 只构造人脸事件和关联图片，读取 `/mnt/sdcard/test.jpeg`，单次尝试后不再随保活重复。
- `PostImages/PostVideoSlices/PostFiles` 现会先清空元数据批次里的 `Data` 字段，再分别通过 `/VIID/Images/{ID}/Data`、`/VIID/VideoSlices/{ID}/Data`、`/VIID/Files/{ID}/Data` 发送 Base64 本体，避免把大块 Base64 混进元数据 JSON。
- `PostBinaryData()` 现统一使用 `application/octet-stream` 发送 `/Data` 请求，并保留原有失败入队与后续补传语义。
- 已执行 `git diff --check` 与基于构建目录 include/define 的 `g++ -std=c++11 -fsyntax-only` 单文件语法检查，确认本次修改无格式问题、`GAT1400ClientService.cpp` 语法可过。
- 当前无法在现有 `cmake-build` 目录完成整目标编译：其 `Makefile` 依赖的私有 `cmake` 路径 `/opt/cmake/cmake-3.16.3-Linux-x86_64/bin/cmake` 不存在；该问题属于本地构建环境残留，不是本次代码改动引入。
