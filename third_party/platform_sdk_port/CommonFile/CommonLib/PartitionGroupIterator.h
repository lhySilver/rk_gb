
#ifndef PARTITION_GROUP_ITERATOR_HEAD
#define PARTITION_GROUP_ITERATOR_HEAD

#include "PartitionGroupSDK.h"
#include "ConfigPack_Data_Item_ID_Def.h"
#include <list>

class CStoragePathOrGroupNodeEx;

class PARTITION_GROUP_CLASS CStoragePathOrGroupNode
{
public:
	virtual const GUID &GetNodeGUID(void) const = 0;	//本节点GUID
	//virtual tuint32 GetNodeID(void) = 0;			//本节点ID

	virtual bool GetNodeName(char *szName, int nLen) const = 0;		//获取存储位置组名称, 存储位置组节点有效

	virtual bool IsDefaultGroup(void) const = 0;					//判断是否默认分组,只对分组有效

	virtual const GUID &GetPartitionGroupGUID(void) const = 0;		//获取存储位置所在存储位置组GUID,存储位置节点有效

	virtual const GUID &GetMSUGUID(void) const = 0;	//存储位置或存储位置组所在的流媒体服务器GUID

	virtual bool GetNodeInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperateType) const = 0;

protected:
	CStoragePathOrGroupNode(void) {}
	~CStoragePathOrGroupNode(void) {}
};

class PARTITION_GROUP_CLASS CStoragePathOrGroupIteratorBase
{
public:
	bool HasNext(void);
	CStoragePathOrGroupNode *GetNext(void);

protected:
	CStoragePathOrGroupIteratorBase(void);
	virtual ~CStoragePathOrGroupIteratorBase(void);

	std::list<CStoragePathOrGroupNodeEx *> m_NodeList;		//用来存储已经使用的节点，在析构时Release
	std::list<CStoragePathOrGroupNodeEx *>::iterator m_Iter;
};

//存储位置迭代器,配置里要用到
class PARTITION_GROUP_CLASS CStoragePathIterator : public CStoragePathOrGroupIteratorBase
{
public:
	CStoragePathIterator(void);									//迭代所有存储位置
	explicit CStoragePathIterator(const GUID &PartitionGUID);		//迭代指定存储位置
	virtual ~CStoragePathIterator(void) {}
};

//迭代所有存储位置组
class PARTITION_GROUP_CLASS CStoragePathGroupIterator : public CStoragePathOrGroupIteratorBase
{
public:
	CStoragePathGroupIterator(void);								//迭代所有存储位置组
	CStoragePathGroupIterator(const GUID &GroupGUID);		//迭代指定存储位置组
	virtual ~CStoragePathGroupIterator(void) {}
};

//迭代分组下的所有存储位置分区
class PARTITION_GROUP_CLASS CGroupStoragePathIterator : public CStoragePathOrGroupIteratorBase
{
public:
	CGroupStoragePathIterator(const GUID &GroupGUID);		//迭代指定GUID组下的所有存储位置
	virtual ~CGroupStoragePathIterator(void) {}
};

//迭代流媒体下的所有存储位置组
class PARTITION_GROUP_CLASS CServerStoragePathGroupIterator : public CStoragePathOrGroupIteratorBase
{
public:
	CServerStoragePathGroupIterator(const GUID &SMUNodeGUID);		//迭代指定SMU的所有存储组
	virtual ~CServerStoragePathGroupIterator(void) {}
};

//迭代流媒体下的所有存储位置
class PARTITION_GROUP_CLASS CServerStoragePathIterator : public CStoragePathOrGroupIteratorBase
{
public:
	CServerStoragePathIterator(const GUID &SMUNodeGUID);		//迭代指定SMU的所有存储位置
	virtual ~CServerStoragePathIterator(void) {}
};

#endif //PARTITION_GROUP_ITERATOR_HEAD