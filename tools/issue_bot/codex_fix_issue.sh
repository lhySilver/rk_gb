#!/usr/bin/env bash
set -euo pipefail

: "${WORKTREE_DIR:?WORKTREE_DIR is required}"
: "${ISSUE_NUMBER:?ISSUE_NUMBER is required}"
: "${ISSUE_TITLE:?ISSUE_TITLE is required}"
: "${ISSUE_BODY_FILE:?ISSUE_BODY_FILE is required}"

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
worktree_dir="$(cd "${WORKTREE_DIR}" && pwd)"
state_dir="${ISSUE_BOT_STATE_DIR:-${TMPDIR:-/tmp}/rk_gb-issue-bot-local}"
prompt_file="${state_dir}/codex-issue-${ISSUE_NUMBER}-prompt.md"
last_message_file="${state_dir}/codex-issue-${ISSUE_NUMBER}-last-message.txt"
issue_body_preview_file="${state_dir}/codex-issue-${ISSUE_NUMBER}-body.txt"
base_branch="${ISSUE_BOT_BASE_BRANCH:-silver}"
codex_bin="${CODEX_BIN:-codex}"

mkdir -p "${state_dir}"

if ! command -v "${codex_bin}" >/dev/null 2>&1; then
    printf '[issue-bot][codex-fix] codex executable not found: %s\n' "${codex_bin}" >&2
    exit 20
fi

python3 - "${ISSUE_BODY_FILE}" "${issue_body_preview_file}" <<'PY'
from pathlib import Path
import sys

src = Path(sys.argv[1]).read_text(encoding="utf-8", errors="replace").strip()
if len(src) > 12000:
    src = src[:12000] + "\n\n[truncated]"
Path(sys.argv[2]).write_text(src + ("\n" if src else ""), encoding="utf-8")
PY

cat > "${prompt_file}" <<EOF
你现在在处理 rk_gb 仓库的 GitHub issue 自动修复任务。

约束:
- 当前工作目录是独立隔离 worktree，只能在这个 worktree 内修改代码。
- 不要执行破坏性 git 命令，不要 push，不要创建远端分支。
- 不要污染其他 SoC 的编译环境；如果需要构建，只能把输出放在 worktree 内或 /tmp。
- 本次修复默认基线分支是 \`${base_branch}\`。
- 优先修复 issue 本身；若需要少量文档同步，可以一起做。
- 保持修改尽量小且可验证。
- 如果 issue 信息不足以安全修复，可以留下最小必要诊断改动，但不要伪造“已修复”。

当前 issue:
- 编号: #${ISSUE_NUMBER}
- 标题: ${ISSUE_TITLE}
- 修复分支: ${REPAIR_BRANCH:-unknown}

Issue 正文:

$(cat "${issue_body_preview_file}")

执行要求:
1. 先阅读仓库中与问题直接相关的代码和文档。
2. 直接实现修复，不要只输出分析。
3. 如能本地做轻量验证，则执行；若不能验证，说明原因并保持修改可审查。
4. 最终用简洁中文总结:
   - 改了什么
   - 改动文件
   - 验证结果或未验证原因
EOF

printf '[issue-bot][codex-fix] running codex in %s for issue #%s\n' "${worktree_dir}" "${ISSUE_NUMBER}"

"${codex_bin}" exec \
    --full-auto \
    --cd "${worktree_dir}" \
    --output-last-message "${last_message_file}" \
    - < "${prompt_file}"

printf '[issue-bot][codex-fix] codex completed, summary saved to %s\n' "${last_message_file}"
