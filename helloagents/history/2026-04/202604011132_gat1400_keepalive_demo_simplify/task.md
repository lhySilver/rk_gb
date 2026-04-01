# 任务清单: 简化 GAT1400 保活 demo

目录: `helloagents/history/2026-04/202604011132_gat1400_keepalive_demo_simplify/`

---

## 1. 现状复核
- [√] 1.1 复核上一提交 `86c91f2` 的改动，拆分“demo 必需部分”和“附带的通用协议改造”

## 2. 代码收敛
- [√] 2.1 将 `GAT1400ClientService` 的保活 demo 收敛为最小实现：首次保活成功后只构造一条人脸和一张图片，直接复用现有 `PostFaces/PostImages`
- [√] 2.2 保留现有 `/Data` Base64 上传语义不变，只收敛 demo 自身实现，避免扩大改动面

## 3. 知识库同步
- [√] 3.1 更新 `helloagents/wiki/modules/gat1400.md`
- [√] 3.2 更新 `helloagents/CHANGELOG.md`
- [√] 3.3 更新 `helloagents/history/index.md`

## 4. 验证
- [√] 4.1 执行 `git diff --check`
- [√] 4.2 执行 `GAT1400ClientService.cpp` 最小语法检查

## 执行结果
- 当前 keepalive demo 已收敛为嵌入式场景下的最小测试调用：读取 `/mnt/sdcard/test.jpeg`，编码成 Base64，直接调用 `PostFaces/PostImages`，只在每次服务启动后的首次保活成功时尝试一次。
- 现有 `/Data` Base64 上传逻辑保持不变：`PostImages/PostVideoSlices/PostFiles` 仍先剥离元数据 JSON 中的 `Data`，再通过 `/VIID/*/{ID}/Data` 发送 Base64 本体；`PostBinaryData()` 继续使用 `application/octet-stream`。
- 已执行 `git diff --check` 与基于现有构建参数映射的 `g++ -std=c++11 -fsyntax-only` 单文件语法检查；本次改动可通过语法检查，仍存在仓库原有的 `GB_ID_LEN` 宏重复定义 warning。
