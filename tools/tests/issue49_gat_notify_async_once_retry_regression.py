#!/usr/bin/env python3
from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[2]
HEADER = (ROOT / 'App/Protocol/gat1400/GAT1400ClientService.h').read_text(encoding='utf-8', errors='ignore')
SOURCE = (ROOT / 'App/Protocol/gat1400/GAT1400ClientService.cpp').read_text(encoding='utf-8', errors='ignore')


def require(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is None:
        raise AssertionError(message)


def reject(text: str, pattern: str, message: str) -> None:
    if re.search(pattern, text, re.MULTILINE | re.DOTALL) is not None:
        raise AssertionError(message)


def extract_function(name: str) -> str:
    marker = f"int GAT1400ClientService::{name}"
    start = SOURCE.find(marker)
    if start < 0:
        raise AssertionError(f'缺少 {name} 定义')
    brace = SOURCE.find('{', start)
    if brace < 0:
        raise AssertionError(f'{name} 函数体起始缺失')
    depth = 0
    for idx in range(brace, len(SOURCE)):
        ch = SOURCE[idx]
        if ch == '{':
            depth += 1
        elif ch == '}':
            depth -= 1
            if depth == 0:
                return SOURCE[brace:idx + 1]
    raise AssertionError(f'{name} 函数体结束缺失')


def main() -> int:
    require(HEADER, r'int\s+max_attempt_count\s*;', 'PendingUploadItem 需要记录最大总尝试次数')
    require(SOURCE, r'max_attempt_count=', '持久化队列项需要序列化最大总尝试次数')
    require(SOURCE, r'key == "max_attempt_count"', '持久化队列项需要反序列化最大总尝试次数')
    require(SOURCE, r'PendingReplayLoop\(', 'issue49 需要后台异步回放 worker')
    require(SOURCE, r'RequestPendingReplay\(', 'issue49 需要入队后唤醒后台回放')
    require(SOURCE, r'max_attempt_count\s*>\s*0\s*&&\s*current->attempt_count\s*>?=\s*current->max_attempt_count', '达到最大总尝试次数后应停止继续补传')

    checks = [
        ('NotifyFaces', r'PostFaces\s*\(', 'NotifyFaces 不应再同步直发，且必须限制为最多 2 次总发送'),
        ('NotifyMotorVehicles', r'PostMotorVehicles\s*\(', 'NotifyMotorVehicles 不应再同步直发，且必须限制为最多 2 次总发送'),
        ('NotifyNonMotorVehicles', r'PostNonMotorVehicles\s*\(', 'NotifyNonMotorVehicles 不应再同步直发，且必须限制为最多 2 次总发送'),
    ]
    for name, direct_pattern, message in checks:
        body = extract_function(name)
        reject(body, direct_pattern, message)
        require(body, r'EnqueuePendingUpload\s*\(', message)
        require(body, r'kNotifyAsyncMaxAttemptCount', message)
        require(body, r'RequestPendingReplay\s*\(', f'{name} 在已注册时需要唤醒后台异步回放')

    print('PASS: issue49 gat notify uses async queue with at most two total attempts')
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except AssertionError as exc:
        print(f'FAIL: {exc}')
        raise SystemExit(1)
