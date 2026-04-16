# 变更提案: packaging 目录归档化

## 需求背景
`feature/dg_ipc_replay_20260415` 分支中的 `packaging/` 目录长期存放板级镜像、rootfs 资源、驱动与工具，目录层级深、二进制文件多，但日常变更频率很低。继续以展开目录形式直接纳入 Git，会让仓库差异浏览和历史记录噪声过大，因此需要把 `packaging/` 收敛为单个归档文件，同时保留现有打包流程可用性。

## 变更内容
1. 将当前版本化的 `packaging/` 目录压缩为单个 `packaging.tar.xz` 归档并纳入 Git。
2. 调整构建入口，在 `packaging/` 目录缺失但归档存在时自动解压恢复。
3. 将解压后的 `packaging/` 目录改为忽略项，避免本地产生大量未跟踪文件。

## 影响范围
- **模块:** build、packaging、helloagents
- **文件:** `build.sh`、`.gitignore`、`packaging.tar.xz`、知识库文档
- **API:** 无新增外部 API，仅调整本地打包资源的获取方式
- **数据:** 无业务数据模型变更

## 核心场景

### 需求: 减少 packaging 目录的 Git 噪声
**模块:** build / packaging
把基本不变的板级打包资源改为单归档管理，降低仓库中文件数量和差异噪声。

#### 场景: packaging 以归档形式版本化
在归档化完成后：
- Git 中只保留一个 `packaging.tar.xz`
- 原 `packaging/` 展开目录不再直接进入版本库
- 日常查看提交时不再被大量二进制路径淹没

### 需求: 保持现有打包命令可继续使用
**模块:** build
用户继续执行现有 `build.sh` 的 `image/all/debug` 等路径时，不需要手工先解压归档。

#### 场景: 构建入口自动恢复 packaging
当 `build.sh` 检测到目标打包目录不存在、但对应 `tar.xz` 归档存在时：
- 自动在仓库根目录完成解压
- 后续 `make -C $PACKAGING` 保持原行为
- 已解压目录被 `.gitignore` 忽略，不污染工作区

## 风险评估
- **风险:** 直接删除 `packaging/` 可能导致现有 `build.sh` 打包流程中断。
- **缓解:** 在 `build.sh` 中补充通用自动解压入口，并保留归档命名与目录命名一一对应。
