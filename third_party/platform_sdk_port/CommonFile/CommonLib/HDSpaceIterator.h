#ifndef HD_SPACE_ITERATOR_HEAD
#define HD_SPACE_ITERATOR_HEAD

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "DiskManagerSDK.h"
#include <list>
#include "base_type_define.h"

class DISK_MANAGER_CLASS CHDSpaceNode
{
public:
	virtual const GUID &GetNodeGUID(void) const = 0;		//节点GUID
	virtual tuint32 GetNodeID(void) const = 0;

    virtual bool IsHardDisk(void) const = 0;				//硬盘,直接读写硬盘方式使用
    virtual bool IsPartition(void) const = 0;				//分区,文件模块使用
	virtual bool IsOSUsed(void) const = 0;					//操作系统所使用的

	virtual bool IsFormat(void) const = 0;					//空间是否已经格式化过

    virtual STORED_DATA_TYPE GetStoredDataType(void) const = 0;	//获取被存储数据的类型

    virtual const char *GetPath(void) const = 0;			//获取存储空间的路径,只有已格式化的存储空间有效

    virtual tuint64 GetBeginPosition(void) const = 0;		//获取开始位置,只能硬盘段有效
    virtual tuint64 GetCapacity(void) const = 0;			//获取容量大小,Byte
    virtual tuint64 GetPartitionFreeSpace(void) const = 0;	//获取分区空闲空间大小,Byte,只对分区有效
    virtual tuint32 GetSectorSize(void) const = 0;			//获取扇区大小,Byte

    virtual GUID GetHardDiskGUID(void) const = 0;			//获取所在的硬盘GUID

protected:
    CHDSpaceNode(void) {}
    virtual ~CHDSpaceNode(void) {}
};

class CHDSpaceNodeEx;
class DISK_MANAGER_CLASS CHDSpaceIteratorBase
{
public:
	bool HasNext(void);
	CHDSpaceNode *GetNext(void);

protected:
	CHDSpaceIteratorBase(void);
	virtual ~CHDSpaceIteratorBase(void);

	std::list<CHDSpaceNodeEx *> m_NodeList;
	std::list<CHDSpaceNodeEx *>::iterator m_Iter;
};

//单结点迭代器,可以是分区节点,硬盘空间段节点
class DISK_MANAGER_CLASS COneHDSpaceNodeIter
{
public:
	explicit COneHDSpaceNodeIter(const GUID &SpaceGUID);
	~COneHDSpaceNodeIter(void);

	CHDSpaceNode *operator ->(void);
	operator CHDSpaceNode *(void);

private:
	CHDSpaceNodeEx *m_pCurNode;
};

//迭代所有的硬盘空间,包括分区和硬盘空间段节点
 class DISK_MANAGER_CLASS CHDSpaceNodeIter : public CHDSpaceIteratorBase
 {
 public:
 	CHDSpaceNodeIter(void);				//遍历所有的硬盘空间
 	virtual ~CHDSpaceNodeIter(void) {}
 };

 //迭代指定硬盘下的所有硬盘空间
 class DISK_MANAGER_CLASS CDiskHDSpaceNodeIter : public CHDSpaceIteratorBase
 {
 public:
	 CDiskHDSpaceNodeIter(const GUID &DiskGUID);
	 virtual ~CDiskHDSpaceNodeIter(void) {}
 };

#endif //HD_SPACE_ITERATOR_HEAD


