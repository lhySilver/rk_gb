
#ifndef __UPDATE_CFGPACK_NO_OP__
#define __UPDATE_CFGPACK_NO_OP__

#include "base_type_define.h"

class CCfgObservable_No_Operator;
class CUpdateCfgPack_No_Operator
{
public:
	CUpdateCfgPack_No_Operator()
	{
	};
	virtual ~CUpdateCfgPack_No_Operator(void)
	{
	};

	///////////观察者第一次被添加进去的时候被调用的函数
	virtual void OnAddObserver(CCfgObservable_No_Operator *pObservable) = 0;						//添加观察者后的通知,观察者可以在此时初始化

	////////////一个包更新完成   提供一个发送给窗口消息的函数
	virtual void UpdateCompleted(CCfgObservable_No_Operator *pObservable){};

	virtual void UpdateUserInfoChange(GUID userGUID){};//客户端需要判断当前登录用户是不是此用户，如果是需要重登录
	virtual void UpdateUserGroupRightChange(GUID userGroupGUID){};//客户端需要判断当前登录用户是不是属于此权限组，如果属于需要重登录
};

#endif //__UPDATE_CFGPACK_NO_OP__