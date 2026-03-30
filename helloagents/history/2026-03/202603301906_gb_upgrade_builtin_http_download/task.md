# 任务清单: GB28181 升级下载去除 curl 命令依赖

目录: `helloagents/history/2026-03/202603301906_gb_upgrade_builtin_http_download/`

---

## 1. 现状复核
- [√] 1.1 复核 `ProtocolManager::HandleGbDeviceUpgradeControl` 的下载、校验和错误码链路，确认 `DownloadFileByCurl()` 的现有约束
- [√] 1.2 盘点工程内可复用的 HTTP/socket helper，确认替换后不再依赖外部 `curl` 命令

## 2. 代码实现
- [√] 2.1 调整 `rk_gb/App/Protocol/ProtocolManager.cpp`，用内置 HTTP 下载实现替换 `DownloadFileByCurl()`
- [√] 2.2 保持升级主流程、超时、重定向和失败语义尽量兼容，并明确当前协议边界

## 3. 验证与知识库同步
- [√] 3.1 执行最小静态验证，确认 `ProtocolManager.cpp` 语法正确且下载 helper 链路自洽
- [√] 3.2 更新 `rk_gb/helloagents/wiki/modules/protocol.md`、`rk_gb/helloagents/CHANGELOG.md`、`rk_gb/helloagents/history/index.md`

## 4. 收尾
- [√] 4.1 更新任务状态与执行结果
- [√] 4.2 将方案包迁移到 `rk_gb/helloagents/history/2026-03/`

## 执行结果
- `ProtocolManager::DownloadFileByCurl()` 已改为进程内 HTTP 下载实现，不再拼接外部 `curl` 命令；下载阶段直接走 URL 解析、TCP 连接、HTTP 请求、响应头解析和流式文件写入。
- 新实现保留了原有升级链路依赖的核心语义：总超时控制、`301/302/303/307/308` 重定向、HTTP `>=400` 失败、以及按 `Content-Length` / `chunked` / 连接关闭三种方式接收响应体。
- 升级包下载改为边收边写，避免把整个固件包一次性读入内存；下载失败时会清理临时文件，继续复用上层现有的文件大小、校验和、重命名与错误码链路。
- 当前内置下载仅支持 `http`；若平台下发 `https` URL，会记录 `scheme_not_supported` 日志并返回下载失败，这个协议边界已同步写入知识库。
- 已完成 `git diff --check` 与定向 `g++ -fsyntax-only` 语法检查，确认 `ProtocolManager.cpp` 当前修改可通过最小静态验证。
