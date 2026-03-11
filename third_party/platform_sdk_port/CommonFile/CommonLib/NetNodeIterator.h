#ifndef __NETNODEITERATOR_H__
#define __NETNODEITERATOR_H__

#include <list>
#include "NetCommSDK.h"
#include "TypeCheck.h"

class CNetNode;

//******************************************************************************/
//迭代器基类，实现了列表迭代器基本方法，列表迭代器继承时只需在构造时构造出m_NodeList
class _EXPORT_NETCOMM_CLASS CNetIteratorBase
{
public:
	virtual ~CNetIteratorBase(void);

	//根据数据集实现相应的取当前项同时移动到下一项函数
	virtual CNetNode *GetNext(void);
	virtual bool HasNext(void);

protected:
	static void GetAll(CNetNode* pNetNode, void* pParam1, const void* pParam2);
	static void GetLogined(CNetNode* pNetNode, void* pParam1, const void* pParam2);
	static void GetType(CNetNode* pNetNode, void* pParam1, const void* pParam2);
	static void GetGUID(CNetNode* pNetNode, void* pParam1, const void* pParam2);
	static void GetID(CNetNode* pNetNode, void*pParam1, const void *pParam2);
	static void GetNotDevType(CNetNode* pNetNode, void* pParam1, const void* pParam2);

protected:
	CNetIteratorBase(void) {;}
	std::list<CNetNode*> m_NodeList;		//用来存储全部
	std::list<CNetNode*>::iterator m_iter;//这个用来记录当前迭代位置
};

////////////////////////////////////////////////////单节点，连接上
class _EXPORT_NETCOMM_CLASS COneNetNodeIterator : public CNetIteratorBase
{
public:
	explicit COneNetNodeIterator(GUID nodeID);
	explicit COneNetNodeIterator(tuint32 localID);////单个系统内部的网络节点标识
	virtual ~COneNetNodeIterator(void) {;}

	CNetNode* GetNode();
	CNetNode* operator-> () { return GetNode(); }
	operator CNetNode*();

protected:
	COneNetNodeIterator() {;}
};

////////////////////////////////////////////////////单节点,存在的
class _EXPORT_NETCOMM_CLASS COneExistNetNodeIterator : public CNetIteratorBase
{
public:
	explicit COneExistNetNodeIterator(GUID nodeID);
	explicit COneExistNetNodeIterator(tuint32 localID);////单个系统内部的网络节点标识
	virtual ~COneExistNetNodeIterator(void) {;}

protected:
	COneExistNetNodeIterator() {;}
};

////////////////////////////////////////////////////登录网络节点，成功登陆
class _EXPORT_NETCOMM_CLASS CLoginNetNodeIterator : public CNetIteratorBase
{
public:
	CLoginNetNodeIterator();//
	explicit CLoginNetNodeIterator(CTypeCheck::ECMS_NODE_TYPE_DEF nodeType);
	virtual ~CLoginNetNodeIterator(void) {;}
};

////////////////////////////////////////////////////全部网络节点，连接成功节点
class _EXPORT_NETCOMM_CLASS CAllNetNodeIterator : public CNetIteratorBase
{
public:
	CAllNetNodeIterator();//
	virtual ~CAllNetNodeIterator(void) {;}
};

//单类型迭代器，可以迭代出系统某种类型的全部节点，连接成功
class _EXPORT_NETCOMM_CLASS COneTypeNetNodeIterator : public CNetIteratorBase
{
public:
	explicit COneTypeNetNodeIterator(CTypeCheck::ECMS_NODE_TYPE_DEF nodeType);
	virtual ~COneTypeNetNodeIterator(void){;}
private:
	COneTypeNetNodeIterator(){;}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//迭代器，可以迭代出系统中非设备节点（编码设备和解码设备，目前就这两种设备有独立的网络节点类，以后如果增加则修改实现），连接成功
class _EXPORT_NETCOMM_CLASS CNotDevNetNodeIterator : public CNetIteratorBase
{
public:
	CNotDevNetNodeIterator();
	virtual ~CNotDevNetNodeIterator(void){;}
};


#endif    /////////#ifndef __NETNODEITERATOR_H__