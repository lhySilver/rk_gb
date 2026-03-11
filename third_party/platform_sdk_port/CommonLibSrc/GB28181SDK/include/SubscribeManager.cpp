#include "SubscribeManager.h"

ExpireMessage g_pExpireMessage = NULL;

void CSubscribeManager::CheckSubscribeStatus(tuint32 dwTimerID, tuint32 dwElapse, LPVOID lpParameter)
{
	CSubscribeManager * manager = (CSubscribeManager*)lpParameter;
	if (manager)
	{

		manager->CheckSubscribeStatusTimer(dwTimerID);
	}
}

CSubscribeManager::CSubscribeManager()
{
	m_user = NULL;
}

CSubscribeManager::~CSubscribeManager()
{
	
}

void CSubscribeManager::CheckSubscribeStatusTimer(tuint32 dwTimerID)
{
	SubscribeHandle handle = NULL;

	m_lock.Lock();
	std::map<int,SubscribeHandle>::iterator iter = m_subscribeManagerMap.find(dwTimerID);
	if (iter != m_subscribeManagerMap.end()){
		handle = iter->second;
	}
	m_lock.Unlock();

	if( g_pExpireMessage  && handle){
		g_pExpireMessage( handle, m_user);
	}
}

void CSubscribeManager::SetMessage(ExpireMessage pExpireMessage,void* user)
{
	g_pExpireMessage = pExpireMessage;
	m_user = user;
}

void CSubscribeManager::InsertSubscribeHandle(SubscribeHandle handle,int exprie)
{
	timer_id = SHARESDK_SetTimer(exprie - 5000, CheckSubscribeStatus, this);
	m_lock.Lock();
	m_subscribeManagerMap.insert(std::make_pair(timer_id,handle));
	m_lock.Unlock();
}

bool CSubscribeManager::DeleteSubscribeHandle(SubscribeHandle handle)
{
	bool flag = false;
	m_lock.Lock();
	for (std::map<int,SubscribeHandle>::iterator iter = m_subscribeManagerMap.begin();iter != m_subscribeManagerMap.end();iter++)
	{
		if( iter->second == handle)
		{
			SHARESDK_KillTimer( iter->first );
			m_subscribeManagerMap.erase(iter);
			flag = true;
			break;
		}
	}	
	m_lock.Unlock();
	return flag;
}

bool CSubscribeManager::FindSubscribeHandle(SubscribeHandle handle)
{
	bool flag = false;
	m_lock.Lock();
	for (std::map<int,SubscribeHandle>::iterator iter = m_subscribeManagerMap.begin();iter != m_subscribeManagerMap.end();iter++)
	{
		if( iter->second == handle)
		{
			flag = true;
			break;
		}
	}
	m_lock.Unlock();
	return flag;
}

SubscribeHandle CSubscribeManager::FindSubscribeHandle(int dialog_id)
{
	SubscribeHandle handle = NULL;
	m_lock.Lock();
	std::map<int,SubscribeHandle>::iterator iter = m_subscribeManagerMap.begin();
	while( iter != m_subscribeManagerMap.end() ) 
	{

		SipDialogKey* subscribeInfo = (SipDialogKey*)( iter->second);
		if(  subscribeInfo->DialogId ==  dialog_id  ){
			handle = iter->second;
			break;
		}
		iter++;
	}
	m_lock.Unlock();
	return handle;
}
