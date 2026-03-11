#ifndef USER_ITERATOR_HEAD
#define USER_ITERATOR_HEAD

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "ConfigPack_Data_Item_ID_Def.h"
#include <list>
#include <map>
#include<vector>
#include "UserManagerSDK.h"
using namespace std;

class CConfigPack;
class CUserNodeEx;

class _EXPORT_USERMANAGER_CLASS CUserNode
{
public:
	////////////////////获取帐户名,密码
	virtual bool GetAccountName(char *szName, int nSize) const = 0;
	virtual bool GetAccountPSW(char *szPSW, int nSize) const = 0;

	///////////获取帐户ID
	virtual tuint32 GetID(void) const = 0;
	virtual const GUID &GetGUID(void) const = 0;

	virtual const GUID &GetBelongGroupID(void) const = 0;//获取帐户的组ID
	virtual const GUID &GetCreateID(void) const = 0;//获取创建者，用户——用户 角色——角色
	virtual bool IsUser(void) const = 0;
	virtual bool IsUserGroup(void) const = 0;

	virtual bool GetPhoneNum(char *szPhone, int nSize) const = 0;	//获取帐户的电话号码

	virtual bool GetEmail(char *szEmail, int nSize) const = 0;		//获取帐户的Email地址

	virtual bool IsLock(void) const = 0;							//获取帐户的锁定状态

	virtual bool IsLimited(void) const = 0;							//获取帐户是否绑定mac

	virtual bool GetDescription(char *szDescription, int nSize) const = 0;//获取帐户的描述

	virtual bool IsSuperAdmin(void) const = 0;	//超级管理员
	virtual bool IsAdmin(void) const = 0;		//包括超级管理员和普通管理员
	virtual bool IsOperator(void) const = 0;	//操作员

	virtual bool IsDefaultGroup(void) const = 0;	//是否默认用户组,只对组节点有效

	virtual bool IsEnableRightCtrl(void) const = 0;//获取帐户的权限开启或关闭的状态

	virtual bool GetLimitedMAC(char *szMAC, int nSize) const = 0;

	//是否对节点有权限，有实际的权限，比如设备配置，通道预览等
	virtual bool HasNodeRight(tuint32 dwNodeID) const = 0;

	//是否需要此节点，需要此节点不一定代表有它的权限，比如有通道权限就需要设备显示在树上，可以传入存储、转发节点
	virtual bool NeedNode(const GUID &NodeGUID) const = 0;
	virtual bool CheckNodeRight(tuint32 dwNodeID, const tuint64 &ullRight) const = 0;
	virtual bool CheckNodeRight(const GUID &NodeGUID, const tuint64 &ullRight) const = 0;
	virtual tuint64 GetNodeRight(const GUID &NodeGUID) const = 0;

	virtual bool GetUserInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperatorType) const = 0;
	virtual bool GetUserRight(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperatorType) const = 0;

	virtual bool SetUserGroupSourceArea(const GUID AreaGUID) = 0;
	virtual bool GetUserGroupSourceArea(GUID &AreaGUID) const = 0;//获取之前一定要判断是否有源区域，没有源区域，表示没有任何区域权限
	virtual bool IsHaveUserGroupSourceArea() const  = 0;
	virtual bool DeleteUserGroupSourceArea()  = 0 ;
	virtual std::map<GUID, tuint64> GetUserGroupRightMap() const = 0;

protected:
	CUserNode(void) {}
	virtual ~CUserNode(void) {}
};

class _EXPORT_USERMANAGER_CLASS CUserIteratorBase
{
public:
	virtual ~CUserIteratorBase(void);

	bool HasUser(void);
	CUserNode *GetNext(void);

protected:
	CUserIteratorBase(void) {}
	CUserIteratorBase(bool bUser);

protected:
	std::list<CUserNodeEx *> m_UserNodeList;
	std::list<CUserNodeEx *>::iterator m_Iter;

protected:
	//判断角色1的等级是否能够有权限查看角色2
	bool IsHighLevelRight(GUID group1,GUID group2);
	//获取父区域
	GUID GetAreaParentNodeGUID(GUID areaGUID);
	//获取区域的直系区域 下坐标越小区域越大
	std::vector<GUID> GetDirectLineArea(GUID areaGUID);
};

//指定或所有用户或组迭代器
class _EXPORT_USERMANAGER_CLASS CUserIterator : public CUserIteratorBase
{
public:
	CUserIterator(void);					//所有用户和组
	CUserIterator(tuint32 dwNodeID);		//指定用户或组ID
	CUserIterator(const GUID &NodeGUID);	//指定用户或组GUID
	CUserIterator(const char *szName);		//指定名称,不区分是用户还是组
	CUserIterator(const char *szName, bool bUser);	//指定类型(用户true/组false)名称

	virtual ~CUserIterator(void) {}
};

//指定用户组下所有用户迭代器
class _EXPORT_USERMANAGER_CLASS CGroupUserIterator : public CUserIteratorBase
{
public:
	CGroupUserIterator(const GUID &GroupGUID);
	virtual ~CGroupUserIterator(void) {}
};

//所有用户迭代器
class _EXPORT_USERMANAGER_CLASS CAllUserIterator : public CUserIteratorBase
{
public:
	CAllUserIterator(void);
	virtual ~CAllUserIterator(void) {}
};

//所有组迭代器
class _EXPORT_USERMANAGER_CLASS CAllUserGroupIterator : public CUserIteratorBase
{
public:
	CAllUserGroupIterator(void);
	virtual ~CAllUserGroupIterator(void) {}
};

//根据用户或者角色GUID迭代出其有权限查看到的角色(权限组)
class _EXPORT_USERMANAGER_CLASS CHaveRightUserGroupIterator : public CUserIteratorBase
{
public:
	CHaveRightUserGroupIterator(const GUID &UserORGroupGUID);
	virtual ~CHaveRightUserGroupIterator(void) {}
};

//根据用户或者角色GUID迭代出其有权限查看到的用户
class _EXPORT_USERMANAGER_CLASS CHaveRightUserIterator : public CUserIteratorBase
{
public:
	CHaveRightUserIterator(const GUID &UserORGroupGUID);
	virtual ~CHaveRightUserIterator(void) {}
};

#endif 
