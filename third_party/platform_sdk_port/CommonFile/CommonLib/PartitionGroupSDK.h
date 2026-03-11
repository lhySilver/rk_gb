
///////////////////////////PartitionGroupSDK.h

#ifndef PARTITION_GROUP_SDK_HEAD
#define PARTITION_GROUP_SDK_HEAD

#ifdef WIN32
#ifdef PARTITION_GROUP_SDK_EXPORT_API//动态库编译
#define PARTITION_GROUP_CLASS __declspec(dllexport)
#define PARTITION_GROUP_SDK  __declspec(dllexport)
#else
#ifdef PARTITION_GROUP_SDK_EXPORT_STATIC//静态库编译和引用
#define PARTITION_GROUP_CLASS
#define PARTITION_GROUP_SDK
#else//使用者编译
#define PARTITION_GROUP_CLASS __declspec(dllimport)
#define PARTITION_GROUP_SDK  __declspec(dllimport)
#endif
#endif

#else
#define PARTITION_GROUP_CLASS __attribute__ ((visibility("default")))
#define PARTITION_GROUP_SDK __attribute__ ((visibility("default")))
#endif 

class CConfigPack;
class Interlocked;

#include "PartitionGroupIterator.h"

class CStoragePathOrGroupNode;
class CPartitionGroupObserver
{
public:
	CPartitionGroupObserver(void) {}
	virtual ~CPartitionGroupObserver(void) {}

	virtual void OnAddObserver(void) {}

	virtual void CreatePartitionGroup(const CStoragePathOrGroupNode *pGroupNode) {}			//创建盘组通知
	virtual void DeletePartitionGroup(const CStoragePathOrGroupNode *pGroupNode) {}			//删除盘组通知

	virtual void AddPartition(const CStoragePathOrGroupNode *pPartitionNode) {}				//添加磁盘空间配置通知
	virtual void RemovePartition(const CStoragePathOrGroupNode *pPartitionNode) {}			//移除磁盘空间配置通知
};

PARTITION_GROUP_SDK bool PARGROUP_Initial(Interlocked *pNodeID);
PARTITION_GROUP_SDK void PARGROUP_Quit(void);

PARTITION_GROUP_SDK bool PARGROUP_AddObserver(CPartitionGroupObserver *pObserver);
PARTITION_GROUP_SDK bool PARGROUP_DeleteObserver(CPartitionGroupObserver *pObserver);

PARTITION_GROUP_SDK void PARGROUP_ClearAllInfo(void);
PARTITION_GROUP_SDK void PARGROUP_UpdateInfo(const CConfigPack &ConfigPack);
PARTITION_GROUP_SDK bool PARGROUP_GetAllInfo(CConfigPack &ConfigPack);

PARTITION_GROUP_SDK GUID PARGROUP_GetDefaultDisDiskGroup(const GUID &ServerGUID);

#endif //PARTITION_GROUP_SDK_HEAD








