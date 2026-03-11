
#include "CfgObservable_No_Operator.h"
#include "UpdateCfgPack_No_Operator.h"
#include "MySingleLock.h"

#define new DEBUG_NEW

/************************************************************************/
/* 构造                                                                     */
/************************************************************************/
CCfgObservable_No_Operator::CCfgObservable_No_Operator()
{

}

/************************************************************************/
/*析构                                                                     */
/************************************************************************/
CCfgObservable_No_Operator::~CCfgObservable_No_Operator()
{

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCfgObservable_No_Operator::AddObserver(CUpdateCfgPack_No_Operator *pObserver)
{
	if(NULL == pObserver)
	{
		return ;
	}

	/////////如果主题已经存在就先删除，防止重复加入后的重复调用
	DeleteObserver(pObserver);

	CMySingleLock  LockAction(m_ObserverListLock);
	m_ObserverList.push_back(pObserver);
	pObserver->OnAddObserver(this);
}

void CCfgObservable_No_Operator::DeleteObserver(CUpdateCfgPack_No_Operator *pDelObserver)
{
	CMySingleLock  LockAction(m_ObserverListLock);
	std::list<CUpdateCfgPack_No_Operator *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CUpdateCfgPack_No_Operator *pObserver = *iter;
		if(pObserver == pDelObserver)
		{
			m_ObserverList.erase(iter);
			break;
		}		
		iter++;
	}	
}

void CCfgObservable_No_Operator::ClearAllOberver()
{
	m_ObserverList.clear();
}


void CCfgObservable_No_Operator::UpdateCompleted(void)
{
	CMySingleLock  LockAction(m_ObserverListLock);
	std::list<CUpdateCfgPack_No_Operator *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CUpdateCfgPack_No_Operator *pObserver =*iter;
		iter++;

		pObserver->UpdateCompleted(this);
	}	
}

//客户端需要判断当前登录用户是不是此用户，如果是需要重登录
void CCfgObservable_No_Operator::UpdateUserInfoChange(GUID userGUID)
{
	CMySingleLock  LockAction(m_ObserverListLock);
	std::list<CUpdateCfgPack_No_Operator *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CUpdateCfgPack_No_Operator *pObserver =*iter;
		iter++;

		pObserver->UpdateUserInfoChange(userGUID);
	}	
}
//客户端需要判断当前登录用户是不是属于此权限组，如果属于需要重登录
void CCfgObservable_No_Operator::UpdateUserGroupRightChange(GUID userGroupGUID)
{
	CMySingleLock  LockAction(m_ObserverListLock);
	std::list<CUpdateCfgPack_No_Operator *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CUpdateCfgPack_No_Operator *pObserver =*iter;
		iter++;

		pObserver->UpdateUserGroupRightChange(userGroupGUID);
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

