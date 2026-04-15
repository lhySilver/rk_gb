# 任务清单: dg_ipc 分支整理回放

目录: `helloagents/plan/202604151948_dg_ipc_replay/`

---

## 1. 分支基线整理
- [√] 1.1 在隔离工作区中新建回放分支，并确认当前本地协议提交与远端协议提交的关系，验证 why.md#需求-在协议基线上重建-dg_ipc-场景-新分支具备协议线与-ipc-适配
- [√] 1.2 将远端协议分支独有提交吸收到回放分支，验证 why.md#需求-在协议基线上重建-dg_ipc-场景-新分支具备协议线与-ipc-适配

## 2. dg_ipc 回放与清理
- [√] 2.1 回放 `0fb527e` 的 IPC 适配改动，并保留项目原本版本化的板级资产，验证 why.md#需求-在协议基线上重建-dg_ipc-场景-新分支具备协议线与-ipc-适配
- [√] 2.2 回退 `cmake-build`、调试输出、压缩包和无关资料等明显垃圾路径，验证 why.md#需求-保持仓库可继续构建与交付-场景-清理后仍保留必要板级资产

## 3. 安全检查
- [√] 3.1 执行安全检查（按G9: 避免破坏性 Git 操作、避免覆盖用户当前工作区）

## 4. 文档更新
- [√] 4.1 更新 `helloagents/wiki/modules/build_runtime.md`
- [√] 4.2 更新 `helloagents/CHANGELOG.md` 与 `helloagents/history/index.md`

## 5. 测试
- [√] 5.1 执行最小验证，确认回放分支状态干净且关键提交顺序符合预期
> 备注: 已执行 `bash -n build.sh`、`bash -n packaging/oem_ipc/usr/bin/RkLunch.sh`、`bash -n packaging/oem_ipc/usr/ko/insmod_ko.sh`、`bash -n packaging/oem_ipc/usr/ko/insmod_wifi.sh`、`bash -n packaging/oem_ipc/usr/bin/dual_rv1106.sh`，并确认垃圾路径未回流。`git diff --check` 因上游 `0fb527e` 自带大量尾随空白/CRLF 风格而失败，未在本次分支整理中额外做全量格式化；当前也未执行完整交叉编译。
