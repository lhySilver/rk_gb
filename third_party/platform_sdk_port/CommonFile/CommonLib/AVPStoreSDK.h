#ifndef AVP_STORE_SDK_HEAD
#define AVP_STORE_SDK_HEAD

#ifdef WIN32

#ifdef AVP_STORE_SDK_EXPORT_STATIC//静态库编译
#define AVP_STORE_SDK extern "C"
#else
#ifdef AVP_STORE_SDK_EXPORT_API//动态库编译
#define AVP_STORE_SDK extern "C" __declspec(dllexport)
#else//使用者编译
#define AVP_STORE_SDK extern "C" __declspec(dllimport)
#endif
#endif

#else
#define AVP_STORE_SDK __attribute__ ((visibility("default")))
#endif

#include "NVMS_Define.h"

//状态观察者
class CAVPStateObserver
{
public:
	CAVPStateObserver(void) {}
	virtual ~CAVPStateObserver(void) {}

	virtual void UpdateRecordState(const GUID &ChannelGUID, tuint32 dwRecState) = 0;	//录像状态改变通知, dwRecState为几种录像类型的组合,为0时关闭录像

	virtual void NotifyIOError(const GUID &DiskGUID) {}						//I/O错误通知
	virtual void NotifyOpenFail(const GUID &DiskGUID) {}					//文件(段)打开失败通知
	virtual void NotifyBadTrack(const GUID &DiskGUID) {}					//坏道通知
	virtual void NotifyDiskFull(const GUID &GroupGUID, bool bFull) {}		//磁盘空间用完通知,当满时bFull=true,在覆盖模式下没有这个通知
	virtual void NotifyRecordCover(const GUID &GroupGUID, bool bCover) {}	//覆盖模式下磁盘组GroupGUID开始时覆盖通知
	virtual void NotifyNoDisk(const GUID &GroupGUID, bool bHaveNot) {}		//当磁盘组GroupGUID下没有任何有效存储位置时通知
	virtual void NotifyRepairHD(const GUID &DiskGUID, bool bBegin) {}		//磁盘修复,bBegin==true时开始否则结束
};

//现场音视频请求控制
class CAVPLiveStreamCtrl
{
public:
	CAVPLiveStreamCtrl(void) {}
	virtual ~CAVPLiveStreamCtrl(void) {}

	virtual tuint32 OpenMainStream(const GUID &ChannelGUID, tuint32 VideoRes, tuint32 dwRate,tuint32 dwBitRate, tuint32 dwQuality, bool bAudio) = 0;
	virtual bool SetMainStream(tuint32 dwStreamID, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality) = 0;	//当所录主码流参数改变时调用
	virtual bool SetStreamRate(tuint32 dwStreamID, tuint32 nStreamType,tuint32 dwRate,tuint32 dwBitRate) = 0;

	virtual tuint32 OpenSubStream(const GUID &ChannelGUID,tuint32 dwRate,tuint32 dwBitRate, bool bAudio, bool bSubSecondStream) = 0;//bSubSecondStream第二子码流(即第三)

	virtual bool OpenAudio(tuint32 dwStreamID) = 0;				//dwStreamID为Open的主码流或子码流返回值,主码流和子码流分别打开音频,外面应分别传入打开码流的音频			
	virtual bool CloseAudio(tuint32 dwStreamID) = 0;

	virtual bool CloseStream(tuint32 dwStreamID) = 0;			//关闭码流,如打开了音频则音频同时关闭

	virtual bool isNeedAddNode(const GUID &ChannelGUID) = 0;	//通道是否需要添加到录像处理(通道配置信息刷来后是否加入处理)
};

//存储模式,配置手动模式或是自动模式中的自动录像类型
typedef enum _tag_storage_mode
{
	STORAGE_MODE_NULL = 0,
	STORAGE_MODE_MANUAL,					//手动模式,每个通道按各自配置录像
	STORAGE_MODE_ALWAYS_HIGH,				//自动模式,全天最高码流录像
	STORAGE_MODE_MOTION,					//自动模式,移动时录像
	STORAGE_MODE_ALARM,						//自动模式,报警时录像
	STORAGE_MODE_MOTION_ALARM,				//自动模式,移动或报警时录像
	STORAGE_MODE_INTENSIVE_MOTION,			//自动模式,平时和移动时录像,两个时机录像码流可以不同
	STORAGE_MODE_INTENSIVE_ALARM,			//自动模式,平时和报警时录像,两个时机录像码流可以不同
	STORAGE_MODE_INTENSIVE_MOTION_ALARM,	//自动模式,平时和移动或报警时录像,两个时机录像码流可以不同
}STORAGE_MODE;


//szXMLConfig录像相关配置信息
AVP_STORE_SDK tuint32 AVPSTORE_GetLastError(void);						//返回_ErrorCodeDef.h里的定义

AVP_STORE_SDK bool AVPSTORE_Initial(CAVPLiveStreamCtrl *pAVPStreamCtrl, const char *szXMLCfg = NULL, tuint32 dwXMLLen = 0);
AVP_STORE_SDK void AVPSTORE_Quit(void);

AVP_STORE_SDK bool AVPSTORE_Start(void);
AVP_STORE_SDK void AVPSTORE_Stop(tuint32 dwEndTime = 0);//指定结束时间，不指定时内部会以获取当前时间为结束时间，0没有指定，非0指定

AVP_STORE_SDK bool AVPSTORE_AddAVPStoreStateObserver(CAVPStateObserver *pObserver);
AVP_STORE_SDK void AVPSTORE_DelAVPStoreStateObserver(CAVPStateObserver *pObserver);


////////////录像参数设置//////////////////////////////////////////////////
//启动时,在AVPSTORE_Initial之后AVPSTORE_Start之前设置,运行过程中可以设置
AVP_STORE_SDK bool AVPSTORE_SetStorageMode(STORAGE_MODE StorageMode);	//设置录像模式,默认为手动模式

AVP_STORE_SDK bool AVPSTORE_SetDoubleStream(bool isDouble);				//设置双码流模式,默认单码流

AVP_STORE_SDK bool AVPSTORE_SetExpirationTime(tuint32 dwTimeLen);		//录像过期时间,单位秒

AVP_STORE_SDK bool AVPSTORE_SetMainStreamTime(tuint32 dwTimeLen);		//主码流录像时长,单位秒

AVP_STORE_SDK bool AVPSTORE_SetRecordCover(bool isCover);				//录像是否覆盖

//AVP_STORE_SDK bool AVPSTORE_SetPictureSpace(const tuint64 &ullSpaceSize);//抓图存储空间大小,单位Byte

AVP_STORE_SDK bool AVPSTORE_SetPictureCover(bool isCover);				//图片存储是否覆盖
///////////////////////////////////////////////////////////////////////////


//事件通知
AVP_STORE_SDK void AVPSTORE_StreamClosed(tuint32 dwStreamID);			//现场流关闭

//控制手动录像和报警联动录像
AVP_STORE_SDK bool AVPSTORE_StartEventRecord(const GUID &ChannelGUID, NVMS_DEFINE::RECORD_TYPE RecordType);
AVP_STORE_SDK bool AVPSTORE_StopEventRecord(const GUID &ChannelGUID, NVMS_DEFINE::RECORD_TYPE RecordType);
///////////////////////////////////////////////////////////////////////////
//清除报警类型的录像
AVP_STORE_SDK bool AVPSTORE_StopAlarmRecord();

//业务调用
AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchRecordLog(const GUID &ChannelGUID, tuint32 dwStartTime, tuint32 dwEndTime, bool bWithDataLen = false);	//创建查找录像日志任务(XML描述),单位秒, bWithDataLen:true日志带数据长度,否则不带数据长度
AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchRECLog(const GUID *pChGUID, tuint32 dwChNumber, tuint32 dwMask, tuint32 dwStartTime, tuint32 dwEndTime);
AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchRECDataLenLog(const GUID *pChGUID, tuint32 dwChNumber, tuint32 dwMask, tuint32 dwStartTime, tuint32 dwEndTime);

AVP_STORE_SDK tuint32 AVPSTORE_CreatePlayback(const GUID &ChannelGUID, bool bMainStream, tuint32 dwStartTime, tuint32 dwEndTime);	//创建回放任务,单位秒

AVP_STORE_SDK bool AVPSTORE_Keyframe(tuint32 dwTaskID, const FILETIME &ftTime);				//关键帧回放,此任务此时间点开始,单位100纳秒

AVP_STORE_SDK bool AVPSTORE_Rewind(tuint32 dwTaskID, const FILETIME &ftTime);				//倒放,单位100纳秒

AVP_STORE_SDK bool AVPSTORE_Previous(tuint32 dwTaskID, const FILETIME &ftTime);				//前一帧,单位100纳秒

AVP_STORE_SDK bool AVPSTORE_PreviousKeyframe(tuint32 dwTaskID, const FILETIME &ftTime);		//前一关键帧,单位100纳秒

AVP_STORE_SDK bool AVPSTORE_Playback(tuint32 dwTaskID, const FILETIME &ftTime);				//正常回放,单位100纳秒

AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchKeyframes(const GUID &ChannelGUID, tuint32 dwStartTime, tuint32 dwEndTime, tuint32 nFrame);	//查找时间段内平均时间点对应的关键帧,单位秒

AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchRecordDate(const GUID &ChannelGUID = GUID_NULL);		//查询有录像数据日期,获取XML的描述,ChannelGUID==GUID_NULL时查询所有通道
AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchRECDate(const GUID *pChGUID, tuint32 dwChNumber);	//查询有录像数据日期,获取XML的描述,pChGUID==NULL时查询所有通道

AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchRecordChannel(tuint32 dwDate = 0);					//查询指定日期(==0时所有日期)有录像数据的通道(XML描述),单位秒

//读取一帧数据,返回0表示需要等一下才能读取到数据,返回-1表示读取结束,其他为实际长度
AVP_STORE_SDK tint32 AVPSTORE_GetTaskNext(tuint32 dwTaskID, const BYTE *&pBuf);		//pBuf由内部一次分配,任务销毁时内部释放

AVP_STORE_SDK bool AVPSTORE_DestroyTask(tuint32 dwTaskID);						//销毁任务

AVP_STORE_SDK bool AVPSTORE_AddRecordTag(const GUID &ChannelGUID, tuint32 dwStartTime, const char *szTagName);	//添加标签,已存在则覆盖,单位秒

AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchTag(tuint32 dwStartTime, tuint32 dwEndTime,const GUID &ChannelGUID=GUID_NULL);	//搜索通道时间段内的标签(XML描述),单位秒

AVP_STORE_SDK bool AVPSTORE_DeleteRecordTag(const GUID &ChannelGUID, tuint32 dwStartTime);							//删除标签,单位秒

AVP_STORE_SDK void AVPSTORE_LiveStreamInput(tuint32 dwStreamID, const char *pBuf, tuint32 nLen);				//输入现场流数据

AVP_STORE_SDK bool AVPSTORE_PictureInput(const GUID &ChGUID, const FILETIME &ftTime, NVMS_DEFINE::CAPTURE_PICTURE_TYPE CapType, const char *szUserName, const char *pBuf, tuint32 nLen, tuint32 alarmType=0,tuint32 nPmsPassRecordIndex/*通行记录索引*/=0);	//保存一张图片

AVP_STORE_SDK tuint32 AVPSTORE_CreateSearchPictureLog(tuint32 dwStartTime, tuint32 dwEndTime, const GUID &chGuid = GUID_NULL, tuint32 alarmType=0);			//查看当前存储有哪些图片(XML描述),单位秒

AVP_STORE_SDK tuint32 AVPSTORE_CreateGetPicture(const GUID &ChannelGUID, const FILETIME &ftTime);		//获取指定的图片,单位100纳秒
//根据通行记录查询图片
AVP_STORE_SDK tuint32 AVPSTORE_CreateGetPmsPassRecordPicture(tuint32 nPmsPassRecordIndex/*通行记录索引*/);

AVP_STORE_SDK bool AVPSTORE_DeletePicture(const GUID &ChannelGUID, const FILETIME &ftTime);				//删除一张图片,单位100纳秒

AVP_STORE_SDK bool AVPSTORE_DeletePassPicture(const GUID &ChannelGUID, tuint32 nPmsPassRecordIndex, bool bMoreThanAll=false);	//删除图片,bMoreThanAll为true时删除所有>nPmsPassRecordIndex的图片

AVP_STORE_SDK tuint32 AVPSTORE_GetChRecordDataLen(const GUID &ChannelGUID, tuint32 dwStartTime, tuint32 dwEndTime);		//获取通道录像数据长度,单位MB
//////////////////////////////////////////////////////////////////////////

//格式化接口,格式化的节点一定是未加入存储空间的
AVP_STORE_SDK bool AVPSTORE_FormatPath(const GUID &PathGUID);		//格式化存储空间
///////////////////////////////////////////////


//备份回放支持新增函数

//增加备份数据目录
//szDirPath:文件夹或裸盘目录
//pDirInfo:返回的目录录像有关节点等信息,pDirInfo:外面传入的空指针,该函数返回所需信息,外面不需要销毁
AVP_STORE_SDK tuint32 AVPSTORE_AddPlayBackDir(const char *szDirPath, const BYTE *&pDirInfo);

AVP_STORE_SDK bool AVPSTORE_RemovePlayBackDir(tuint32 hDirHandel);	//移除备份数据目录

AVP_STORE_SDK bool AVPSTORE_ConnectDirHandelAndNode(tuint32 hDirHandel, const GUID &NewGUID, const GUID &SrcGUID);	//设置目录,节点映射关系
//////////////////////////////////////////////////////////////////////////

//PathGUID==GUID_NULL时返回所有空闲空间大小,PathGUID可以也指定为GroupGUID,DiskGUID,StoreSpaceGUID
AVP_STORE_SDK bool AVPSTORE_GetStoreSpaceInfo(const GUID &PathGUID, tuint64 &ullAVFreeSpace, tuint64 &ullAVTotalSpace, tuint64 &ullPFreeSpace, tuint64 &ullPTotalSpace);
#endif //AVP_STORE_SDK_HEAD
