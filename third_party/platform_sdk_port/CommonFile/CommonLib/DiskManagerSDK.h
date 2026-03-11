#ifndef DISK_MANAGER_SDK_HEAD
#define DISK_MANAGER_SDK_HEAD

#ifdef WIN32

#ifdef DISK_MANAGER_SDK_EXPORT_API//动态库编译
#define DISK_MANAGER_SDK  __declspec(dllexport)
#define DISK_MANAGER_CLASS  __declspec(dllexport)
#else
#ifdef DISK_MANAGER_SDK_EXPORT_STATIC//静态库编译和使用
#define DISK_MANAGER_SDK
#define DISK_MANAGER_CLASS
#else//使用者编译
#define DISK_MANAGER_SDK  __declspec(dllimport)
#define DISK_MANAGER_CLASS  __declspec(dllimport)
#endif
#endif

#else
#define DISK_MANAGER_SDK __attribute__ ((visibility("default")))
#define DISK_MANAGER_CLASS __attribute__ ((visibility("default")))
#endif

//阵列类型，暂时只支持 RAID0/RAID1/RAID5/RAID10
typedef enum _tag_RAID_type
{
	RAID_TYPE_0 = 0,
	RAID_TYPE_1 = 1,
	RAID_TYPE_5 = 5,
	RAID_TYPE_6 = 6,
// 	RAID_TYPE_01 = 9,
// 	RAID_TYPE_10 = 10,
// 	RAID_TYPE_50 = 50,
// 	RAID_TYPE_60 = 60,
	RAID_TYPE_Unknown = 100,	//未知的阵列类型
}RAID_TYPE;

//阵列卡支持的条带大小
typedef enum _tag_RAID_stripe_size
{
//     RAID_Stripe_Size_4KB = 4,		//4KB
//     RAID_Stripe_Size_8KB = 8,		//8KB
//     RAID_Stripe_Size_16KB = 16,		//16KB
//     RAID_Stripe_Size_32KB = 32,		//32KB
//     RAID_Stripe_Size_64KB = 64,		//64KB
    RAID_Stripe_Size_128KB = 128,	//128KB
    RAID_Stripe_Size_256KB = 256,	//256KB
    RAID_Stripe_Size_512KB = 512,	//512KB
    RAID_Stripe_Size_1024KB = 1024,	//1024KB
    RAID_Stripe_Size_Unknown = ~0,
}RAID_STRIPE_SIZE;

//阵列状态
typedef enum _tag_RAID_state
{
	RAID_STATE_BEGIN = 0,
    RAID_STATE_DOWNGRADE,		//降级, 黄(255, 255, 0)
    RAID_STATE_REBUILD,			//重建, 黄(255, 255, 0)
    RAID_STATE_INITIAL,			//初始化, 绿(0, 0, 255)
    RAID_STATE_EXPANSION,		//扩容, 黄(255, 255, 0)
    RAID_STATE_NORMAL,			//正常状态, 绿(0, 255, 0)
    RAID_STATE_DISABLED,		//不可用(可被遍历到,但是不能被使用), 红(255, 0, 0)
	RAID_STATE_END
}RAID_STATE;

//硬盘状态
typedef enum _tag_disk_state
{
	DISK_STATE_BEGIN = 0,
	DISK_STATE_NORMAL,		//正常, 绿(0, 255, 0)
	DISK_STATE_UNNORMAL,	//异常, 黄(255, 255, 0)
	DISK_STATE_DISABLED,	//不可用, 红(255, 0, 0)
	DISK_STATE_END
}DISK_STATE;


//所存储的数据的类型，
typedef enum _tag_stored_data_type
{
	STORED_DATA_TYPE_BEGIN = 0,
	STORED_DATA_TYPE_PICTURE,			//图片
	STORED_DATA_TYPE_VIDEO_AND_AUDIO,	//音视频
	STORED_DATA_TYPE_GPS,				//GPS
	STORED_DATA_TYPE_END
}STORED_DATA_TYPE;

//所需存储数据的内容
typedef enum _tag_stored_data_content
{
	STORED_DATA_CONTENT_BEGIN = 0,
	STORED_DATA_CONTENT_VIDEO_AUDIO,				//存储音视频
	STORED_DATA_CONTENT_VIDEO_AUDIO_AND_PICTURE,	//存储音视频和图片
}STORED_DATA_CONTENT;

enum DISKM_ERROR_CODE
{
    DISKM_ERROR_NONE = 0,
	DISKM_ERROR_DELETE_PRESERVED_CACHE, //必须重启（比如delete PreservedCache时一直没返回
};

enum DISKM_STORAGE_TYPE
{
	DISKM_TYPE_PARTION, //分区
	DISKM_TYPE_DISK, //磁盘
	DISKM_TYPE_RAID, //Raid
};

#include "base_type_define.h"
#include "DiskIterator.h"
#include "RAIDIterator.h"
#include "HDSpaceIterator.h"

class Interlocked;
class CDiskNode;
class CEnclosureNode;
class CRAIDNode;
class CHDSpaceNode;

class CDiskObserver
{
public:
	CDiskObserver(void) {}
	virtual ~CDiskObserver(void) {}

	virtual void OnAddObserver(void) = 0;

	virtual void AddHDSpaceNode(const CHDSpaceNode *pHDSpaceNode) {}				//增加一段可以用于存储的硬盘空间,即分区或硬盘的一段
	virtual void RemoveHDSpaceNode(const CHDSpaceNode *pHDSpaceNode) {}				//移除一段可以用于存储的硬盘空间,即分区或硬盘的一段

	virtual void AddDiskNode(const CDiskNode *pDiskNode) {}							//增加一块硬盘或是一个分区,这里是逻辑硬盘的通知,不包括通过第三方(RAID卡)接入的硬盘
	virtual void RemoveDiskNode(const CDiskNode *pDiskNode) {}						//移除一块硬盘或是一个分区

	virtual void OnDiskEvent(const CDiskNode *pDiskNode, const char *szEventXML) {}	//硬盘事件,RAID卡下的硬盘插入和拔出从这里通知上来
	virtual void OnRAIDEvent(const CRAIDNode *pRAIDNode, const char *szEventXML) {}	//RAID事件

	virtual void EnclosurePowerOn(const CEnclosureNode *pEnclosureNode) {}			//机箱开机
	virtual void EnclosurePowerOff(const CEnclosureNode *pEnclosureNode) {}			//机箱关机
};

/* szSataMapXml 为硬盘硬件地址 与 主板SATA丝印 的映射关系，内容为
<?xml version="1.0" encoding="utf-8" ?>
<root>
	<diskInfo>	
		<diskCountIdentify>disk8</diskCountIdentify>
  		<disk8>
    			<item id="0:4:0:0" index="1">sata1</item>
    			<item id="0:3:0:0" index="2">sata2</item>
    			<item id="0:0:0:0" index="3">sata3</item>
    			<item id="0:2:0:0" index="4">sata4</item>
    			<item id="0:1:0:0" index="5">sata5</item>
    			<item id="1:0:0:0" index="6">sata6</item>
    			<item id="1:1:0:0" index="7">sata7</item>
    			<item id="1:2:0:0" index="8">sata8</item>
  		</disk8>
  		<disk2>
    			<item id="0:0:0:0" index="1">sata1</item>
    			<item id="1:0:0:0" index="2">sata2</item>
   		</disk2>
  		<disk1>
    			<item id="0:0:0:0" index="1">sata1</item>
  		</disk1>
	</diskInfo>
</root>
*/
DISK_MANAGER_SDK bool DISKM_Initial(Interlocked *pNodeID, STORED_DATA_CONTENT StoreType = STORED_DATA_CONTENT_VIDEO_AUDIO_AND_PICTURE, const char *szSataMapXml = NULL);
DISK_MANAGER_SDK void DISKM_Quit(void);

DISK_MANAGER_SDK bool DISKM_RefreshInfo(void);//只获取信息时调用，在初始化后不需要调用DISKM_Start的情况下，如果业务调用了DISKM_Start那就不调用此函数

DISK_MANAGER_SDK bool DISKM_Start(void);
DISK_MANAGER_SDK void DISKM_Stop(void);

DISK_MANAGER_SDK bool DISKM_AddObserver(CDiskObserver *pDiskObserver);
DISK_MANAGER_SDK void DISKM_DeleteObserver(CDiskObserver *pDiskObserver);

DISK_MANAGER_SDK void DISKM_ClearFile(const GUID &PathGUID);			//如果要独占分区进行录像 在格式化GUID(分区)之前需要先清除文件,只对分区有效

DISK_MANAGER_SDK GUID DISKM_Format(const GUID &NodeGUID);				//格式化节点,会重新产生新的GUID
//DISK_MANAGER_SDK bool DISKM_UnFormat(const GUID &SpaceGUID);			//把GUID对应的存储位置变为未格式化的
//DISK_MANAGER_SDK bool DISKM_Format(const GUID &SpaceGUID, STORED_DATA_TYPE StoredDataType, const tuint64 &ullBytes);	//硬盘空间段格式化,单位Byte,只对无分区硬盘有效

DISK_MANAGER_SDK bool DISKM_CreateFile(const GUID &PathGUID, const char *szFileName, const tuint64 &ullFileLen,const tuint64 &ullOSLen);	//创建文件并占用空间,只对分区有效,单位Byte

//RAID相关函数
//创建RAID, 指定RAID类型,nStripeSize为条带大小(单位KB)
DISK_MANAGER_SDK GUID DISKM_CreateRAID(const char *szName, const GUID *pDiskGUID, tuint32 nDiskNumber, RAID_TYPE RaidType, RAID_STRIPE_SIZE StripeSizeKB, tuint32& APIRet,bool bUseSpare=false);
DISK_MANAGER_SDK bool DISKM_DestoryRAID(const GUID &GroupGUID);															//销毁RAID
DISK_MANAGER_SDK bool DISKM_IncrementalRAID(const GUID &GroupGUID, const GUID *pDiskGUID, tuint32 nDiskNumber,tuint32& APIRet);			//扩容RAID

//创建热备盘，如果GroupGUID为GUID_NULL表示创建全局热备盘，否则为专有热备盘
DISK_MANAGER_SDK bool DISKM_SetSpare(const GUID &GroupGUID, const GUID *pDiskGUID, tuint32 nDiskNumber);
DISK_MANAGER_SDK bool DISKM_FreeSpare(const GUID *pSpareGUID, tuint32 nSpareNumber);

DISK_MANAGER_SDK DISKM_ERROR_CODE DISKM_GetLastError(void);

//停止阵列卡报警声音
DISK_MANAGER_SDK bool DISKM_ClearAlarm(void);

//得到存储类型
DISK_MANAGER_SDK DISKM_STORAGE_TYPE DISKM_GetStorageType(void);

#endif //DISK_MANAGER_SDK_HEAD






