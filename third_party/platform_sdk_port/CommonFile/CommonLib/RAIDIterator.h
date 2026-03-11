#ifndef RAID_ITERATOR_HEAD
#define RAID_ITERATOR_HEAD

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "DiskManagerSDK.h"
#include <list>
#include "base_type_define.h"

/************************************************************************/
/*                         RAID                                         */
/************************************************************************/
class DISK_MANAGER_CLASS CRAIDNode
{
public:
    virtual const GUID &GetNodeGUID(void) const = 0;			//节点GUID
    virtual tuint32 GetNodeID(void) const = 0;

    virtual const char *GetNodeName(void) const = 0;			//RAID名称

    virtual const GUID &GetRAIDCardGUID(void) const = 0;		//所在RAID卡GUID

    virtual  tuint32 getEnclosureID(void) const = 0;        //RAID所在Enclosure

    virtual tuint32 GetDiskNumber(void) const = 0;				//接入的硬盘个数
    virtual tuint32 GetSpareDiskNumber(void) const = 0;			//热备盘个数

    virtual RAID_TYPE GetRAIDType(void) const = 0;				//RAID类型
    virtual RAID_STRIPE_SIZE GetRAIDStripeSize(void) const = 0;	//RAID条带大小,单位Byte

    virtual tuint64 GetRealSize(void) const = 0;				//组出来的实际可用大小,Byte
    virtual tuint64 GetAllDiskSize(void) const = 0;				//组内所有硬盘大小总和,Byte

    virtual RAID_STATE GetRAIDState(void) const = 0;			//RAID状态
    virtual tuint32 GetStateProgress(void) const = 0;			//RAID状态进度,初始化,重建,扩容状态支持,范围:0-10000

    virtual bool GetSupportExpansionFlag(void) const = 0;       //RAID当前状态是否支持扩容

protected:
	CRAIDNode(void) {}
	virtual ~CRAIDNode(void) {}
};

//RAID迭代器
class CRAIDNodeEx;
class DISK_MANAGER_CLASS CRAIDIterator
{
public:
	CRAIDIterator(void);									//所有RAID
	CRAIDIterator(const GUID &RAIDGUID);					//指定RAID

	~CRAIDIterator(void);

	bool HasNext(void);
	CRAIDNode *GetNext(void);

private:
	std::list<CRAIDNodeEx *> m_NodeList;
	std::list<CRAIDNodeEx *>::iterator m_Iter;
};



/************************************************************************/
/*                         RAID卡                                       */
/************************************************************************/
class DISK_MANAGER_CLASS CRAIDCardNode
{
public:
	virtual const GUID &GetNodeGUID(void) const = 0;		//节点GUID
	virtual tuint32 GetNodeID(void) const = 0;

	virtual bool IsSupportedRAIDType(RAID_TYPE RAIDType) const = 0;						//是否支持的RAID类型
    virtual bool IsSupportedRAIDStripeSize(RAID_STRIPE_SIZE RAIDStripeSize) const = 0;	//是否支持的条带大小

protected:
	CRAIDCardNode(void) {}
	virtual ~CRAIDCardNode(void) {}
};

class CRAIDCardNodeEx;
class DISK_MANAGER_CLASS CRAIDCardIterator
{
public:
	CRAIDCardIterator(void);							//所有RAID卡
	CRAIDCardIterator(const GUID &CardGUID);			//指定RAID卡

	~CRAIDCardIterator(void);

	bool HasNext(void);
	CRAIDCardNode *GetNext(void);

private:
	std::list<CRAIDCardNodeEx *> m_NodeList;
	std::list<CRAIDCardNodeEx *>::iterator m_Iter;
};


/************************************************************************/
/*                         箱子                                         */
/************************************************************************/
class DISK_MANAGER_CLASS CEnclosureNode
{
public:
	virtual const GUID &GetNodeGUID(void) const = 0;			//节点GUID
	virtual tuint32 GetNodeID(void) const = 0;

	virtual const char *GetSlotHomePosition(void) const = 0;	//基准位置(起始索引的位置), "leftTop", "leftBottom","rightTop","rightBottom"
    virtual const char *GetIncreaseDirection(void) const = 0;	//获取序号方向相对于home位置 "up","down","left","right"

	virtual tuint32 GetRowNumber(void) const = 0;				//行数
	virtual tuint32 GetColumnNumber(void) const = 0;			//列数

    virtual tuint32 GetEnclosureID(void)const = 0;
protected:
	CEnclosureNode(void) {}
	virtual ~CEnclosureNode(void) {}
};

class CEnclosureNodeEx;
class DISK_MANAGER_CLASS CEnclosureIterator
{
public:
	CEnclosureIterator(void);							//所有机箱
	CEnclosureIterator(const GUID &EnclosureGUID);		//指定机箱

	~CEnclosureIterator(void);

	bool HasNext(void);
	CEnclosureNode *GetNext(void);

private:
	std::list<CEnclosureNodeEx *> m_NodeList;
	std::list<CEnclosureNodeEx *>::iterator m_Iter;
};

#endif //RAID_ITERATOR_HEAD
