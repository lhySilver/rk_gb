#ifndef __LOGIN_STATE_OBSERVER__
#define __LOGIN_STATE_OBSERVER__

#include "NetProtocolDefine_ForNVMS.h"

//////////包含头文件 
using namespace NVMS_NET_PROTOCOL;

class CLoginStateObserver
{
public:
	enum LOGIN_STATE_DEF
	{
		LOGIN_STATE_DEF_RANGE_BEGIN,
		NO_STARTUP_LOGIN = 2,////没有启动登陆
		CONNECTING,////正在连接
		CONNECT_FAIL,//////连接失败
		CONNECT_SUCCESS,//////连接成功
		LOGINING,//////////正在登陆
		LOGIN_FAIL,////////登陆失败
		LOGIN_SUCCESS,///////登陆成功
		LOGIN_STATE_DEF_RANGE_END
	};
	CLoginStateObserver(void){};
public:
	virtual ~CLoginStateObserver(void){};

	virtual void UpdateLoginState(tint32 state,tuint32 errcode,ECMS_LOGIN_SUCCESS_INFO *pLoginSuccInfo,const char *pExtInfo,size_t extInfoLen)=0;
};


#endif //__LOGIN_STATE_OBSERVER__


