#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[2]
errors = []

protocol_header = (root / 'App/Protocol/ProtocolManager.h').read_text(encoding='utf-8', errors='ignore')
protocol_manager = (root / 'App/Protocol/ProtocolManager.cpp').read_text(encoding='utf-8', errors='ignore')
gat_header = (root / 'App/Protocol/gat1400/GAT1400ClientService.h').read_text(encoding='utf-8', errors='ignore')
gat_service = (root / 'App/Protocol/gat1400/GAT1400ClientService.cpp').read_text(encoding='utf-8', errors='ignore')

if 'bool GetGatOnlineStatus() const;' not in protocol_header:
    errors.append('ProtocolManager.h 缺少 GetGatOnlineStatus 声明')

if 'bool ProtocolManager::GetGatOnlineStatus() const' not in protocol_manager:
    errors.append('ProtocolManager.cpp 缺少 GetGatOnlineStatus 定义')

if 'return m_gat_client.get() != NULL && m_gat_client->IsRegistered();' not in protocol_manager:
    errors.append('ProtocolManager::GetGatOnlineStatus 未通过 GAT1400ClientService::IsRegistered 透传在线态')

if 'bool IsRegistered() const;' not in gat_header:
    errors.append('GAT1400ClientService.h 缺少 IsRegistered 声明')

if 'bool GAT1400ClientService::IsRegistered() const' not in gat_service:
    errors.append('GAT1400ClientService.cpp 缺少 IsRegistered 定义')

if 'return m_registered;' not in gat_service.split('bool GAT1400ClientService::IsRegistered() const', 1)[1].split('}', 1)[0] if 'bool GAT1400ClientService::IsRegistered() const' in gat_service else True:
    errors.append('GAT1400ClientService::IsRegistered 未返回 m_registered')

if errors:
    for error in errors:
        print(f'FAIL: {error}')
    sys.exit(1)

print('PASS: issue45 gat online status regression checks')
