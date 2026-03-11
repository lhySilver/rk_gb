#ifndef USER_ITERATOR_HEAD
#define USER_ITERATOR_HEAD

#include "NVRServerSDKEx.h"
#include <string>

/************************************************************************/
/* 管理员不需要配置权限 拥有所有权限
    只有操作员才需要配置权限  ，而且操作员不能登录配置客户端  不能进行配置*/
/************************************************************************/
//////////////////////在线用户迭代器/////////////////////////
//class _EXPORT_NVRSERVERSDK_API COnlineUserIterator
//{
//public:
//	COnlineUserIterator(void);		//所有在线用户
//
//	COnlineUserIterator(tuint32 loginID);		//
//	COnlineUserIterator(const char *szUserName);
//
//	~COnlineUserIterator(void);
//
//	bool HasNext(void);
//	tuint32 GetNext(void);					//返回值得将作为以下两上函数的参数
//
//	tuint32 GetNodeID(tuint32 item);
//	GUID GetNodeGUID(tuint32 item);
//	std::string GetNodeName(tuint32 item);
//private:
// 	std::map<tuint32, tuint32> m_VirtualUserIDMap;
// 	std::map<tuint32, tuint32>::const_iterator m_Iter;
// 
// 	std::list<CUserNode *> m_UserNodeList;
//};

/////////////////////////////////////////////////////////////
#endif 
