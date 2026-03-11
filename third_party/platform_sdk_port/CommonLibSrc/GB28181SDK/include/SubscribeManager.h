#ifndef CSUBSCRIBEMANAGER_H
#define CSUBSCRIBEMANAGER_H

#include "GB28181Defs.h"
#include <map>
#include "Lock.h"
#include "ShareSDK.h"
#include "SipDefs.h"
//#pragma once

typedef void (*ExpireMessage)(SubscribeHandle handle,void* user);


class CSubscribeInfo
{
public:
	SipDialogKey key;
	int sn;
	std::string event;
};


class CSubscribeManager
{
public:
	CSubscribeManager();
	~CSubscribeManager();
	void InsertSubscribeHandle(SubscribeHandle handle,int exprie);

	bool DeleteSubscribeHandle(SubscribeHandle handle);

	bool FindSubscribeHandle(SubscribeHandle handle);

	SubscribeHandle  DeleteSubscribeHandle(int dialog_id);

	SubscribeHandle FindSubscribeHandle(int dialog_id);

	static void CheckSubscribeStatus(tuint32 dwTimerID, tuint32 dwElapse, LPVOID lpParameter);

	void CheckSubscribeStatusTimer(tuint32 dwTimerID);

	void SetMessage(ExpireMessage pExpireMessage, void* user);

private:
	CLock                         m_lock;
	std::map<int,SubscribeHandle> m_subscribeManagerMap;
	void*                         m_user;
	tuint32                       timer_id;
};


#endif // CSUBSCRIBEMANAGER_H