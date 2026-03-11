#ifndef AVP_STORE_ALONE_SDK_HEAD
#define AVP_STORE_ALONE_SDK_HEAD

#include "AVPStoreSDK.h"

typedef bool (ScheduleCBFun)(const GUID &ScheduleGUID);			//排程判断回调函数,当前时间在此排程时间段内返回true,否则返回false


AVP_STORE_SDK bool AVPSTORE_SetScheduleCBFun(ScheduleCBFun *pCBFun);	//设置排程判断回调函数


//添加直接读写音视频存储位置: GroupGUID,组ID,非分组模式为GUID_NULL; szPath,完整路径; ullBeginPos,开始位置; ullCapacity,空间大小; dwSectorSize,扇区大小;
AVP_STORE_SDK GUID AVPSTORE_AddRECPath(const GUID &GroupGUID, const char *szPath, const tuint64 &ullBeginPos, const tuint64 &ullCapacity, tuint32 dwSectorSize);

//添加直接读写图片存储位置
AVP_STORE_SDK GUID AVPSTORE_AddPicturePath(const GUID &GroupGUID, const char *szPath, const tuint64 &ullBeginPos, const tuint64 &ullCapacity, tuint32 dwSectorSize);

AVP_STORE_SDK bool AVPSTORE_DelRECPath(const GUID &PathGUID);			//删除存储位置,PathGUID为Add返回GUID


//添加录像通道
AVP_STORE_SDK bool AVPSTORE_AddRECChannel(const GUID &ChGUID);

//删除存储通道
AVP_STORE_SDK bool AVPSTORE_DelRECChannel(const GUID &ChGUID);

//绑定通道存储位置,在哪个组下存储,如非分组模式GroupGUID为GUID_NULL
AVP_STORE_SDK bool AVPSTORE_BindChannelRECGroup(const GUID &ChGUID, const GUID &GroupGUID);
AVP_STORE_SDK bool AVPSTORE_UnbindChannelRECGroup(const GUID &ChGUID);


//设置通道录像码流:ChGUID通道GUID, VideoRes分辨率, dwRate帧率, dwQuality质量, bAudio声音, isMainStream主码流; 当isMainStream为false时前面参数无效(录子码流)

//一直高质量录像码流,一定是主码流
AVP_STORE_SDK bool AVPSTORE_SetChannelMaxStream(const GUID &ChGUID, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool bAudio);
//自动模式平常录像码流
AVP_STORE_SDK bool AVPSTORE_SetChannelAutoNormalStream(const GUID &ChGUID, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool bAudio, bool isMainStream);
//自动模式事件录像码流
AVP_STORE_SDK bool AVPSTORE_SetChannelAutoEventStream(const GUID &ChGUID, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool bAudio, bool isMainStream);
//手动模式排程录像码流
AVP_STORE_SDK bool AVPSTORE_SetChannelManualNormalStream(const GUID &ChGUID, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool bAudio, bool isMainStream);
//手动模式事件录像码流
AVP_STORE_SDK bool AVPSTORE_SetChannelManualEvnetStream(const GUID &ChGUID, tuint32 VideoRes, tuint32 dwRate, tuint32 dwQuality, bool bAudio, bool isMainStream);
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//设置通道预录像时长
AVP_STORE_SDK bool AVPSTORE_SetChannelPerrecordTime(const GUID &ChGUID, tuint16 usTime);

//设置通道延时录像时长
AVP_STORE_SDK bool AVPSTORE_SetChannelPostRecordTime(const GUID &ChGUID, tuint16 usTime);

//绑定通道录像排程,一个时刻只能绑定一个排程
AVP_STORE_SDK bool AVPSTORE_BindChannelRECSchedule(const GUID &ChGUID, const GUID &ScheduleGUID);

//解绑定通道录像排程
AVP_STORE_SDK bool AVPSTORE_UnbindChannelRECSchedule(const GUID &ChGUID);

//绑定通道Motion录像排程,一个时刻只能绑定一个排程
AVP_STORE_SDK bool AVPSTORE_BindChannelMotionRECSchedule(const GUID &ChGUID, const GUID &ScheduleGUID);

//解绑定通道Motion录像排程
AVP_STORE_SDK bool AVPSTORE_UnbindChannelMotionRECSchedule(const GUID &ChGUID);

//绑定通道Sensor录像排程,一个时刻只能绑定一个排程
AVP_STORE_SDK bool AVPSTORE_BindChannelSensorRECSchedule(const GUID &ChGUID, const GUID &ScheduleGUID);

//解绑定通道Sensor录像排程
AVP_STORE_SDK bool AVPSTORE_UnbindChannelSensorRECSchedule(const GUID &ChGUID);


#endif //AVP_STORE_ALONE_SDK_HEAD