# 任务清单: packaging 目录归档化

目录: `helloagents/plan/202604161016_packaging_archive/`

---

## 1. 归档准备
- [√] 1.1 确认 `packaging/` 当前体量、构建入口依赖和可行的归档格式，验证 why.md#需求-保持现有打包命令可继续使用-场景-构建入口自动恢复-packaging
- [√] 1.2 生成 `packaging.tar.xz`，验证 why.md#需求-减少-packaging-目录的-git-噪声-场景-packaging-以归档形式版本化

## 2. 构建入口调整
- [√] 2.1 修改 `build.sh`，补充按需自动解压 `packaging*.tar.xz` 的逻辑，验证 why.md#需求-保持现有打包命令可继续使用-场景-构建入口自动恢复-packaging
- [√] 2.2 更新 `.gitignore`，忽略解压后的 `packaging/` 工作目录，验证 why.md#需求-减少-packaging-目录的-git-噪声-场景-packaging-以归档形式版本化

## 3. 仓库收口
- [√] 3.1 从版本库中移除展开的 `packaging/` 目录，改为纳入 `packaging.tar.xz`
- [√] 3.2 更新 `helloagents/wiki/modules/build_runtime.md`
- [√] 3.3 更新 `helloagents/CHANGELOG.md` 与 `helloagents/history/index.md`

## 4. 验证
- [√] 4.1 执行最小验证，确认 `build.sh` 语法正确、自动解压可用、工作区状态符合预期
> 备注: 已执行 `bash -n build.sh`；在 `packaging/` 缺失状态下通过 `source ./build.sh` 调用 `ensure_packaging_dir "$ROOT/packaging"` 自动从 `packaging.tar.xz` 恢复目录并验证成功；验证后已重新删除解压目录，确保提交结果仅保留归档文件。
