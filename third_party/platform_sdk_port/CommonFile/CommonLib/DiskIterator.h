#ifndef DISK_ITERATOR_HEAD
#define DISK_ITERATOR_HEAD

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "DiskManagerSDK.h"
#include "base_type_define.h"
#include <list>

/*
1.未格式化的分区或磁盘,每次启动动态创建GUID;
2.格式化时传入的GUID位置内部识别是分区还是磁盘,并把此GUID写入作为之后的永久标识
3.RAID的GUID即为它所创建的磁盘GUID
*/

class DISK_MANAGER_CLASS CDiskNode
{
public:
	virtual const GUID &GetNodeGUID(void) const = 0;		//节点GUID,即为存储空间的GUID
	virtual tuint32 GetNodeID(void) const = 0;

    virtual bool IsHardDisk(void) const = 0;				//硬盘,直接读写硬盘方式使用,最前面4KB空间保留
    virtual bool IsPartition(void) const = 0;				//分区,文件模块使用

    virtual bool IsOSUsed(void) const = 0;					//操作系统所使用的分区或硬盘

	virtual bool IsFormat(void) const = 0;					//是否已经写标识信息

    virtual const char *GetNodeName(void) const = 0;		//获取节点名称

    virtual const char *GetSerialNO(void) const = 0;		//获取系列号,只对硬盘有效
    virtual const char *GetModel(void) const = 0;           //获取硬盘型号

  	virtual DISK_STATE GetHealthState(void) const = 0;		//获取硬盘健康状态
    virtual tuint32 GetDiskStateDetail(void) const = 0;		//获取硬盘状态

    virtual tuint32 GetPartitionNumber(void) const = 0;		//获取分区数量,只对硬盘有效

	virtual const char *GetPath(void) const = 0;
    virtual tuint64 GetCapacity(void) const = 0;			//获取容量大小,Byte
    virtual tuint64 GetPartitionFreeSpace(void) const = 0;	//获取分区空闲空间大小,Byte,只对分区有效
	virtual bool SetPartAVSpace(tuint64 len) = 0;		//设置分区可用空间,Byte,只对分区有效
	virtual tuint64 GetPartAVSpace(void) const = 0;	//获取分区可用空间，只先设置才能得到,Byte,只对分区有效

    virtual GUID GetRAIDGUID(void) const = 0;		//所属的RAID,只对RAID下的硬盘有效

    virtual const GUID &GetRAIDCardGUID(void) const = 0;	//所属的阵列卡,只对RAID卡下的硬盘有效
    virtual const GUID &GetEnclosureGUID(void) const = 0;	//所属机箱,只对RAID卡下的硬盘有效
    virtual tuint32 GetSlotIndex(void) const = 0;			//机箱前面板槽位索引,从1开始。或主板SATA口丝印序号
    virtual bool IsSpareHardDisk(void) const = 0;			//是否热备盘
    virtual tuint32 GetDiskIndex(void) const = 0;           //盘索引

    virtual GUID GetHardDiskGUID(void) const = 0;			//获取分区所在的硬盘,只对分区有效

    virtual const char *GetFirmwareVersion(void) const = 0;	//固件版本
    virtual const char *GetBusType(void) const = 0;			//总线类型 sata/sas
    virtual tuint32 GetRPM(void) const = 0;					//转速
    virtual bool IsSupportWRCache(void) const = 0;			//写缓存开启
    virtual bool IsSupportRDCache(void) const = 0;			//读缓存开启
    virtual bool IsSupportCMDQ(void) const = 0;				//命令队列开启
    virtual tuint32 GetReadErr(void) const = 0;				//读错误数
    virtual tuint32 GetSpinUp(void) const = 0;				//马达启动时间
    virtual tuint32 GetReallocSectors(void) const = 0;		//重映射块数
    virtual tuint32 GetPendingSectors(void) const = 0;		//不稳定块数
    virtual tuint32 GetUncorrectable(void) const = 0;		//不可修复块数
    virtual tuint32 GetPowerOnHours(void) const = 0;		//上电小时数
    virtual tuint32 GetTemperature(void) const = 0;			//硬盘温度
    virtual const char *GetRWProperty(void) const = 0;		//读写属性
    virtual const char *GetSource(void) const = 0;			//来源,本地还是网络共享
    
    //获取SMART信息，存的是xml格式。 返回的是 xml 的字节长度
    /*
<?xml version="1.0" encoding="UTF-8"?>
<response>
  <status>success</status>
  <types>
    <diskStatus>
      <enum>good</enum>
      <enum>lowHealth</enum>
      <enum>bad</enum>
    </diskStatus>
    <smartStatus>
      <enum>warn</enum>
      <enum>normal</enum>
    </smartStatus>
  </types>
  <content>
  	<id>08095753-fbb2-4bc1-81c3-54de9d0767ac</id>
    <smartItems type="list">
      <itemType>
        <smartStatus type="smartStatus"/>
      </itemType>
      <item id="1">
        <value>100</value>
        <worstValue>60</worstValue>
        <threshold>999</threshold>
        <rawValue>323243</rawValue>
        <smartStatus>normal</smartStatus>
      </item>
    </smartItems>
  </content>
  <diskStatus type="diskStatus">good</diskStatus>
</response>
    */
    virtual int GetSMARTInfo(char *szXmlBuf, int nXmlBufLen) const = 0;
protected:
    CDiskNode(void) {}
    virtual ~CDiskNode(void) {}
};

class CDiskNodeEx;
class DISK_MANAGER_CLASS CDiskIteratorBase
{
public:
	bool HasNext(void);
	CDiskNode *GetNext(void);

protected:
	CDiskIteratorBase(void) ;
	virtual ~CDiskIteratorBase(void);

	std::list<CDiskNodeEx *> m_NodeList;
	std::list<CDiskNodeEx *>::iterator m_Iter;
};

//单节点迭代器,可以是分区节点,硬盘节点,RAID卡下的硬盘节点,RAID组出来的硬盘节点
class DISK_MANAGER_CLASS COneDiskNodeIter
{
public:
	explicit COneDiskNodeIter(tuint32 dwNodeID);
	explicit COneDiskNodeIter(const GUID &NodeGUID);
	~COneDiskNodeIter(void);

	CDiskNode *operator ->(void);
	operator CDiskNode *(void);

private:
	CDiskNodeEx *m_pCurNode;
};

//硬盘迭代器,迭代出操作系统所有可见的硬盘,在RAID卡下空闲的硬盘为不可见的
 class DISK_MANAGER_CLASS CHardDiskNodeIter : public CDiskIteratorBase
 {
 public:
 	CHardDiskNodeIter(void);				//遍历所有的硬盘
 	virtual ~CHardDiskNodeIter(void) {}
 };

 //分区迭代器,迭代分区
 class DISK_MANAGER_CLASS CPartitionNodeIter : public CDiskIteratorBase
 {
 public:
	 CPartitionNodeIter(void);				//遍历所有的分区
	 virtual ~CPartitionNodeIter(void) {}
 };

//硬盘下的分区迭代器
class DISK_MANAGER_CLASS CHardDiskPartitionIter : public CDiskIteratorBase
{
public:
	CHardDiskPartitionIter(const GUID &DiskGUID);	//遍历硬盘下的所有分区节点
	virtual ~CHardDiskPartitionIter(void) {}
};


//迭代RAID下的所有硬盘
class DISK_MANAGER_CLASS CRAIDDiskIter : public CDiskIteratorBase
{
public:
	explicit CRAIDDiskIter(const GUID &RAIDGUID);	//遍历RAID下的所有硬盘
	virtual ~CRAIDDiskIter(void) {}
};

//迭代机箱下的所有空闲盘
class DISK_MANAGER_CLASS CEnclosureFreeDiskIter : public CDiskIteratorBase
{
public:
	CEnclosureFreeDiskIter(void);									//所有机箱下的空闲盘
	explicit CEnclosureFreeDiskIter(const GUID &EnclosureGUID);		//指定机箱下的空闲盘
	virtual ~CEnclosureFreeDiskIter(void) {}
};

//迭代机箱下的全局热备盘
class DISK_MANAGER_CLASS CEnclosureSpareDiskIter : public CDiskIteratorBase
{
public:
	CEnclosureSpareDiskIter(void);									//所有机箱下的全局热备盘
	explicit CEnclosureSpareDiskIter(const GUID &EnclosureGUID);	//指定机箱下的全局热备盘
	virtual ~CEnclosureSpareDiskIter(void) {}
};

//迭代机箱下的硬盘
class DISK_MANAGER_CLASS CEnclosureDiskIter : public CDiskIteratorBase
{
public:
	CEnclosureDiskIter(void);										//所有机箱下的硬盘
	explicit CEnclosureDiskIter(const GUID &EnclosureGUID);			//指定机箱下的硬盘
	virtual ~CEnclosureDiskIter(void) {}
};

#endif //DISK_ITERATOR_HEAD


