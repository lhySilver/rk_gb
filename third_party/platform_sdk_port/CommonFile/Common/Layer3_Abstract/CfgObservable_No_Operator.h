
#ifndef __CFG_OBSERVABLE_NO_OPERATOR_
#define __CFG_OBSERVABLE_NO_OPERATOR_

//////////包含头文件 
#include <list>
#include "Lock.h"


/************************************************************************/
/*  配置信息主题，如果是需要了解配置信息的具体更新内容可以注册后
会收到更新的动作和信息*/
/************************************************************************/
class CUpdateCfgPack_No_Operator;

class CCfgObservable_No_Operator  
{
public:
	CCfgObservable_No_Operator();
	virtual ~CCfgObservable_No_Operator();

	/////////////加入需要观察的对象
	virtual void AddObserver(CUpdateCfgPack_No_Operator *pObserver);

	/////////////移除对象，并不销毁对象
	void DeleteObserver(CUpdateCfgPack_No_Operator *pDelObserver);

	/////////移除全部的观察者
	void ClearAllOberver();

	virtual void UpdateCompleted(void);

	virtual void UpdateUserInfoChange(GUID userGUID);//客户端需要判断当前登录用户是不是此用户，如果是需要重登录
	virtual void UpdateUserGroupRightChange(GUID userGroupGUID);//客户端需要判断当前登录用户是不是属于此权限组，如果属于需要重登录

protected:
private:

	//////////////通知对象的列表
	std::list<CUpdateCfgPack_No_Operator *>   m_ObserverList;
	CLock									  m_ObserverListLock;

};


#endif 