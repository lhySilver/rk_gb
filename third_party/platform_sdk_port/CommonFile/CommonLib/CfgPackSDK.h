
///////////////////////////CfgPackSDK.h
/************************************************************************/
/* 说明：提供对下面操作的封装，产生关联的数据包保证操作后数据的一致性。
１、设备增、删、改　（设备包括解码设备、编码设备、各种服务器）
２、排程增、删、改
３、区域增、删、改
４、用户增、删、改
５、用户权限增、删、改
６、报警联动增、删、改
７、电子地图文件增、删、改
８、电子地图上的热区或热点增、删、改*/
/************************************************************************/


#ifndef _CFG_PACK_SDK_H
#define _CFG_PACK_SDK_H

#ifdef WIN32
#ifdef CFGPACK_SDK_EXPORT_API//动态库编译
#define _EXPORT_CFGPACK_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_CFGPACK_SDK  __declspec(dllimport)
#endif

#else
#define _EXPORT_CFGPACK_SDK __attribute__ ((visibility("default")))
#endif

#include "TypeCheck.h"
#include "NVMS_Define.h"
#include "ConfigPack_Struct_Def.h"
#include "GB28181Def_Base.h"
#include<set>
class CConfigPack;
class Interlocked;

class CCfgPackInfoHelp
{
public:
	virtual CTypeCheck::ECMS_NODE_TYPE_DEF GetCurrAppNodeType(void) = 0;	//当前应用程序节点类型
	virtual bool UserHasDefaultRightForCreateResource(void) = 0;			//是否给所有用户设置新创建资源的默认权限
	virtual GUID GetCurrAppLoginUserID(void) = 0;							//获取当前操作配置的用户ID
	virtual bool GetChannelGUIDFile(char *szFile, tuint32 dwLen) {return false;}	//获取通道对应GUID文件完整路径及名称
};

_EXPORT_CFGPACK_SDK bool CFGPACK_Initial(CCfgPackInfoHelp *pCfgPackInfoHelp);
_EXPORT_CFGPACK_SDK void CFGPACK_Quit(void);

_EXPORT_CFGPACK_SDK bool CFGPACK_Begin(void);								//所有调用CFGPACK配置操作之前设置
_EXPORT_CFGPACK_SDK const char *CFGPACK_GetConfigData(int &nLen);			//获得配置数据
_EXPORT_CFGPACK_SDK bool CFGPACK_End(const char *pConfigData);				//当前配置操作完成时调用此函数,pConfigData为获得的缓冲区

//设备相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateDevice(const char *szName, tuint32 devType, const GUID &ParentAreaGUID);			//创建设备
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateDevice(const char *szName, tuint32 devType, const GUID &ParentAreaGUID,const GUID &MDUServerGUID,const GUID &MSUServerGUID, const GUID &sopGUID);			//创建设备
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceProperty(const GUID &DeviceGUID, int nChannelNum, int nSensorNum, int nAlarmOutNum);	//设置设备属性
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceProperty(const GUID &DeviceGUID, int nChannelNum, int nSensorNum, int nAlarmOutNum,const GUID &MDUServerGUID,const GUID &AreaGUID,const GUID &MSUServerGUID,std::set<GUID> N9KChlGUIDList ,bool bIsGBDevice =false);	//设置设备属性
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceIPPort(const GUID &DeviceGUID, const char *szIP, tuint16 nPort,const char *szUrl);	//设置登录设备的IP地址和端口,szIP可以是IP或域名,如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceManufacturer(const GUID &DeviceGUID, tuint32 nManufacturer);/*0为默认(无定制及旧版本)，1支持定制且设备已定制，2支持定制设备未定制*/	//
//_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceRtspUrl(const GUID &DeviceGUID, const char *szUrl);	//设置登录设备的RTSP Url
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceUserNamePSW(const GUID &DeviceGUID, const char *szUserName, const char *szPSW);	//设置登录设备的用户名和密码,如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceSN(const GUID &DeviceGUID, const char *szSN);							//设置主动上报设备的SN
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceHighestLiveStream(const GUID &DeviceGUID, NVMS_DEFINE::ECMS_STREAM_TYPE StreamType);	//设置现场预览最高码流
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceType(const GUID &DeviceGUID, tuint32 devType);						//设置设备类型
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceName(const GUID &DeviceGUID, const char *szName);						//设置设备名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceGB28181Info(const GUID &DeviceGUID, const char *szGB28181ID, const char *szGAT1400ID, const char *szUpperGB28181ID,const char *szUpperGAT1400ID, const char *szGB28181AppendInfo,const GUID &CascadePlatformGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDeviceModelVersion(const GUID &DeviceGUID, const char *szModel, const char *szVersion); //设置设备型号和版本信息
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDevicTransfer(const GUID &DeviceGUID,const GUID &TransferGUID);	//设置设备所属转发服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDevicTransfer(const GUID &DeviceGUID,const GUID &TransferGUID);	//修改设备所属转发服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDeviceArea(const GUID &DeviceGUID,const GUID &AreaGUID);	//修改设备所属区域
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteDevice(const GUID &DeviceGUID);			//删除设备, 自动删除其下通道及相关信息,包括逻辑和物理关系
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDevicStorageServer(const GUID &DeviceGUID,const GUID &StroageServerGUID);	//修改设备所属存储服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteDevicStorageServer(const GUID &DeviceGUID);	//设备所属存储服务器设置成空
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteDevicTransfer(const GUID &DeviceGUID);	//设备所属转发服务器设置成空
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteDevicCGU(const GUID &DeviceGUID);	//设备所属级联服务器设置成空
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDevicCGU(const GUID &DeviceGUID,const GUID &CguGUID);	//修改设备所属级联服务器，仅国标设备用到
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDeviceSop(const GUID &DeviceGUID,const GUID &SopGUID);	//修改设备处警操作
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDeviceUpperGB28181ID(const GUID &DeviceGUID, const char *szUpperGB28181ID);	//修改设备UpperGBID（在客户端手动修改用)
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDeviceUpperGAT1400ID(const GUID &DeviceGUID, const char *szUpperGAT1400ID);	//修改设备UpperGAT1400ID（在客户端手动修改用)
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDeviceGAT1400ID(const GUID &DeviceGUID, const char *szGAT1400ID);

	//////////////////////////////////////////////////////////////////////
//解码器相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateDecoder(const char *szName, tuint32 devType,const GUID &WallGUID);	//创建设备
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDecoderProperty(const GUID &DecoderGUID, int nOutPutNum);	//设置设备属性
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDecoderIPPort(const GUID &DecoderGUID, const char *szIP, tuint16 nPort);	//设置登录设备的IP地址和端口,szIP可以是IP或域名,如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDecoderUserNamePSW(const GUID &DecoderGUID, const char *szUserName, const char *szPSW);	//设置登录设备的用户名和密码,如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDecoderName(const GUID &DeviceGUID, const char *szName);	
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDecoderType(const GUID &DecoderGUID, tuint32 devType);						//设置类型
_EXPORT_CFGPACK_SDK bool CFGPACK_SetDecoderWall(const GUID &DecoderGUID,const GUID &WallGUID);	//设置设备所属电视墙
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyDecoderWall(const GUID &DecoderGUID,const GUID &WallGUID);	//修改设备所属电视墙
_EXPORT_CFGPACK_SDK bool CFGPACK_DelDecoderWall(const GUID &DecoderGUID,const GUID &WallGUID);//解绑解码器和电视墙的关系
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteDecoder(const GUID &DecoderGUID);

//通道相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateChannel(const char *szName, const GUID &DeviceGUID, tuint8 nChIndex);		//创建通道
_EXPORT_CFGPACK_SDK bool CFGPACK_CreateChannel(const GUID &ChannelGUID, const char *szName, const GUID &DeviceGUID, tuint8 nChIndex);	//带ID创建通道
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelName(const GUID &ChannelGUID, const char *szName);						//更改通道名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelNo(const GUID &ChannelGUID, tuint8 nChIndex);							//更改通道序号
_EXPORT_CFGPACK_SDK bool CFGPACK_SetRecordSchedule(const GUID &ChannelGUID, const GUID &ScheduleGUID);				//设置录像排程,如果ScheduleGUID为GUID_NULL清除当前设置的排程
//_EXPORT_CFGPACK_SDK bool CFGPACK_SetMotionRecSchedule(const GUID &ChannelGUID, const GUID &ScheduleGUID);			//设置Motion录像排程,如果ScheduleGUID为GUID_NULL清除当前设置的排程
//_EXPORT_CFGPACK_SDK bool CFGPACK_SetSensorRecSchedule(const GUID &ChannelGUID, const GUID &ScheduleGUID);			//设置Sensor录像排程,如果ScheduleGUID为GUID_NULL清除当前设置的排程
//_EXPORT_CFGPACK_SDK bool CFGPACK_SetMotionSchedule(const GUID &ChannelGUID, const GUID &ScheduleGUID);				//设置移动侦测排程,如果ScheduleGUID为GUID_NULL清除当前设置的排程
_EXPORT_CFGPACK_SDK bool CFGPACK_SetRecStream(const GUID &ChannelGUID, NVMS_DEFINE::ECMS_STREAM_TYPE StreamType);	//设置通道录像码流
_EXPORT_CFGPACK_SDK bool CFGPACK_SetStoragePath(const GUID &ChannelGUID, const GUID &PathGUID);						//设置通道录像存储位置
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteChannel(const GUID &ChannelGUID);											//删除通道
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelKeyboardIndex(const GUID &ChannelGUID, const tuint32 nKeyboardIndex);	//更改通道键盘编号
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelFaceCaptureSource(const GUID &ChannelGUID, const tuint32 nFaceCaptureSource);	//更改通道人脸抓拍源

//包括设置视频移动侦测，丢失，异常，物品丢失，越界，入侵
//_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelAlarmSchedule(const GUID &ChannelGUID, const GUID &ScheduleGUID , int nChannelAlarmType);	//通用接口，设置通道产生的报警排程，移动侦测也放到这里
////////////////////////////////////////////////////////////////////////


//IP通道相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateIPChannel(const char *szName, const char *szIP, tuint16 nPort, tuint32 Manufacturer, tuint32 ProtocolType, tuint16 nChIndex, bool isIDBindIP = false);	//创建IP通道,isIDBindIP==true时根据IP和端口生成固定GUID
 
_EXPORT_CFGPACK_SDK bool CFGPACK_SetIPChProperty(const GUID &IPChGUID, int nSensorNum, int nAlarmOutNum);	//设置IP通道属性
_EXPORT_CFGPACK_SDK bool CFGPACK_SetIPChIPPort(const GUID &IPChGUID, const char *szIP, tuint16 nPort);						//设置登录IP通道的IP地址和端口,szIP可以是IP或域名,如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetIPChUserNamePSW(const GUID &IPChGUID, const char *szUserName, const char *szPSW);		//设置登录IP通道的用户名和密码,如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetIPChHighestLiveStream(const GUID &IPChGUID, NVMS_DEFINE::ECMS_STREAM_TYPE StreamType);	//设置现场预览最高码流
_EXPORT_CFGPACK_SDK bool CFGPACK_SetIPChName(const GUID &IPChGUID, const char *szName);										//设置IP通道名称
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteIPChannel(const GUID &IPChGUID);	
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelGB28181id(const GUID &ChannelGUID, const char *szGB28181Id);   //设置国标ID
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelUpperGB28181id(const GUID &ChannelGUID, const char *szUpperGB28181Id);   //设置发送给上级国标ID
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelUpperGAT1400id(const GUID &ChannelGUID, const char *szUpperGAT1400Id); 
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyChannelGAT1400ID(const GUID &ChannelGUID, const char *szGAT1400Id); 

//删除IP通道
///////////////////////////////////////////////////////////////////////////////

//报警输入相关接口
_EXPORT_CFGPACK_SDK bool CFGPACK_SetSensorName(const GUID &SensorGUID, const char *szName);					//更改Sensor名称
//_EXPORT_CFGPACK_SDK bool CFGPACK_SetSensorSchedule(const GUID &SensorGUID, const GUID &ScheduleGUID,tuint32 nAlarmType);		//设置Sensor排程,如果ScheduleGUID为GUID_NULL清除当前设置的排程
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmInProperty(const GUID &AlarmInGUID, bool bNormallyOpen, bool bEnable);	//设置报警输出属性

_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAlarmIn(const GUID &DeviceGUID, unsigned char nIndex);			//创建报警输入
//////////////////////////////////
//报警输出相关接口
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmOutName(const GUID &SensorGUID, const char *szName);					//更改Alarmout名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmOutHoldingTime(const GUID &AlarmOutGUID, tuint32 dwTime);			//设置报警输出延时时间
//_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmOutSchedule(const GUID &AlarmOutGUID, const GUID &ScheduleGUID,tuint32 nAlarmType);	//设置报警输出排程,如果ScheduleGUID为GUID_NULL清除当前设置的排程
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAlarmOut(const GUID &DeviceGUID, unsigned char nIndex);			//创建报警输出
/////////////////////////////////

//设置节点附加信息
_EXPORT_CFGPACK_SDK bool CFGPACK_SetNodeAppendInfo(const GUID &NodeGUID, const char *szAppendInfo);


//区域相关接口
_EXPORT_CFGPACK_SDK bool CFGPACK_CreateArea(const char *szName, const GUID &ParentGUID, bool isNeedModifyUpperGBID = false);	//创建区域,ParentGUID为GUID_NULL时为根节点
//返回值不同，用于添加设备的时候自动添加区域
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateArea_Ex(const char *szName, const GUID &ParentGUID/*, bool isNeedModifyUpperGBID = false*/);	//创建区域,ParentGUID为GUID_NULL时为根节点
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteArea(const GUID &AreaGUID);							//删除区域
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAreaName(const GUID &AreaGUID, const char *szName);		//更改区域名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAreaType(const GUID &AreaGUID, const tuint32 type);		//设置区域类型
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAreaGBid(const GUID &AreaGUID, const GUID &GBid);		//如果是国标区域，使用此接口设置国标GUID
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAreaGB28181id(const GUID &AreaGUID, const char *szGB28181Id);   //设置国标ID
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAreaUpperGB28181id(const GUID &AreaGUID, const char *szUpperGB28181Id);   //设置Upper国标ID
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAreaUpperGAT1400id(const GUID &AreaGUID, const char *szUpperGAT1400Id);   //设置Upper1400ID
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAreaGB28181Info(const GUID &AreaGUID, const GUID GBid, const char *szGB28181ID);   //设置国标info


_EXPORT_CFGPACK_SDK bool CFGPACK_CreateAreaRelation(const GUID &ParentGUID, const GUID &ChildGUID);		//创建区域关系
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAreaRelation(const GUID &ParentGUID, const GUID &ChildGUID);		//修改区域关系,修改子节点的父节点,并删除子节点与原来父节点的关系
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAreaRelation(const GUID &ParentGUID, const GUID &ChildGUID);		//删除区域关系
///////////////////////////////////////////////////////////////////////

//排程相关接口
struct SCHE_TIME_SECTION
{
	tuint32	Weekday;			//周几
	tuint32	dwStartTime;		//开始时间,单位为秒
	tuint32	dwEndTime;			//结束时间,单位为秒
};
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateSchedule(const char *szName, const SCHE_TIME_SECTION *pTimeSect, int nTimeSectNum);	//创建排程
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteSchedule(const GUID &ScheduleGUID);													//删除排程
_EXPORT_CFGPACK_SDK bool CFGPACK_SetScheduleName(const GUID &ScheduleGUID, const char *szName);								//更改排程名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetScheduleTimeSect(const GUID &ScheduleGUID, const SCHE_TIME_SECTION *pTimeSect, int nTimeSectNum);//更改排程时间段

_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateTimeSecRate(const char *szName, const char *szTimeSection);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteTimeSecRate(const GUID &TimeSecRateGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyTimeSecRate(const GUID &TimeSecRateGUID, const char *szName, const char *szTimeSection);

////////////////////////////////////////////////////


//用户相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateOperatorUser(const char *szName, const char *szPSW);//创建操作员
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAdminUser(const char *szName, const char *szPSW);//创建管理员
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateSuperAdminUser(const char *szName, const char *szPSW);//创建超级管理员
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateUserGroup(const char *szName, bool bDefault = false);//创建用户组,bDefault是否默认用户组
_EXPORT_CFGPACK_SDK bool CFGPACK_SetUserNamePSW(const GUID &UserGUID, const char *szName, const char *szPSW);	//名称和密码如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetUserPhone(const GUID &UserGUID, const char *szPhone);				//设置用户电话
_EXPORT_CFGPACK_SDK bool CFGPACK_SetUserEmail(const GUID &UserGUID, const char *szEmail);				//设置用户Email
_EXPORT_CFGPACK_SDK bool CFGPACK_SetUserDescription(const GUID &UserGUID, const char *szDesc);			//设置用户描述
_EXPORT_CFGPACK_SDK bool CFGPACK_EnableUserRightCtrl(const GUID &UserGUID, bool bEnable);				//开启关闭权限控制
_EXPORT_CFGPACK_SDK bool CFGPACK_LockUser(const GUID &UserGUID, bool bLock);							//锁定解除锁定用户
_EXPORT_CFGPACK_SDK bool CFGPACK_SetMACLimit(const GUID &UserGUID, const BYTE byMAC[8], bool bLimited);	//设置MAC限制
_EXPORT_CFGPACK_SDK bool CFGPACK_SetUserDefaultRight(const GUID &UserGUID);								//设置用户拥有所有资源的默认权限
_EXPORT_CFGPACK_SDK bool CFGPACK_SetUserGroup(const GUID &UserGUID, const GUID &UserGroupGUID);			//设置用户组UserGroupGUID==GUID_NULL时,用户将不属于任何一个组
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteUser(const GUID &UserGUID);										//删除一个用户
_EXPORT_CFGPACK_SDK bool CFGPACK_AddUserRight(const GUID &UserGUID, const GUID &DestNodeGUID, const tuint64 &ullRight);		//增加用户权限
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyUserRight(const GUID &UserGUID, const GUID &DestNodeGUID, const tuint64 &ullRight);	//修改用户权限
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteUserRight(const GUID &UserGUID, const GUID &DestNodeGUID);								//删除用户权限
_EXPORT_CFGPACK_SDK bool CFGPACK_ClearUserRight(const GUID &UserGUID);									//清除权限
_EXPORT_CFGPACK_SDK bool CFGPACK_AddUserGroupSourceArea(const GUID &UserGUID,const GUID SourceAreaGUID);//在二元物理中存角色对应的源区域
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyUserGroupSourceArea(const GUID &UserGUID,const GUID SourceAreaGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteUserGroupSourceArea(const GUID &UserGUID,const GUID &SourceAreaGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetCreateID(const GUID &UserGUID, const GUID &CreateGUID);			//设置用户创建者，用户——用户   角色——角色
////////////////////////////////////////////////////////



//服务器相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateServer(const char *szServerName, tuint32 dwType ,tuint32 dwIsDefault = 0, tuint8 dwIsBackupServer = 0 );//0不是缺省，1缺省									//创建服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_SetServerUserNamePSW(const GUID &ServerGUID, const char *szUserName, const char *szPSW);	//设置登录服务器的用户名和密码如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetServerName(const GUID &ServerGUID, const char *szServerName);							//设置服务器名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetServerIPPort(const GUID &ServerGUID, const char *szIP, tuint16 nPort);					//设置登录服务器的IP地址和端口
_EXPORT_CFGPACK_SDK bool CFGPACK_SetGB28181Info(const GUID &ServerGUID, const char *szGB28181ID, const char *szGB28181Password);//设置服务器的国标编号和国标密码，仅级联网关用到
_EXPORT_CFGPACK_SDK bool CFGPACK_SetIsBackupServer(const GUID &ServerGUID, tuint8 dwIsBackupServer);//设置服务器是否是热备服务

//服务器关系
_EXPORT_CFGPACK_SDK bool CFGPACK_CreateServerRelation(const GUID &ParentGUID, const GUID &ChildGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyServerRelation(const GUID &ParentGUID, const GUID &ChildGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteServerRelation(const GUID &ParentGUID, const GUID &ChildGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteServer(const GUID &ServerGUID);														//删除服务器
/////////////////////////////////////////////////


//报警联动相关
_EXPORT_CFGPACK_SDK bool CFGPACK_CreateAlarmTrigger(const GUID &AlarmNodeGUID, tuint32 AlarmType, tuint32 TriggerType, const GUID &TriggerNodeGUID, const char* TriggerInfoEx = "");	//创建报警联动
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAlarmTrigger(const GUID &AlarmTriggerGUID);//删除报警联动


//语音播报文件相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateVoiceBroadcastFile(const char *szName, const char *szFileExtensionName, const char * pFileBuf,tuint32 nFileLen);						//创建语音文件
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteVoiceBroadcastFile(const GUID &FileGUID);											//删除语音文件
/////////////////////////////////////////////


//分区组相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreatePartitionGroup(const GUID &ServerGUID, const char *szName, bool isDefault);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeletePartitionGroup(const GUID &GroupGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_AddPartition(const GUID &ServerGUID, const GUID &GroupGUID, const GUID &PartitionGUID);//添加存储位置
_EXPORT_CFGPACK_SDK bool CFGPACK_RemovePartition(const GUID &PartitionGUID);//存储移除
////////////////////////////////////////////////


//电子地图相关接口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateEMap(const char *szName,tuint32 nType, const char *szFileExtensionName,const char* XPos,const char* YPos,tuint32 nZoomRate,tuint32 nAttribute, const GUID &ParentGUID,const char * pFileBuf,tuint32 nFileLen);						//创建地图
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteEMap(const GUID &EMapGUID);											//删除地图
_EXPORT_CFGPACK_SDK bool CFGPACK_SetEMapName(const GUID &EMapGUID, const char *szName);						//更改地图名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetEMapProperty(const GUID &EMapGUID, const char* XPos,const char* YPos,tuint32 nZoomRate);						//更改地图属性
_EXPORT_CFGPACK_SDK bool CFGPACK_SetParentEMap(const GUID &EMapGUID, const GUID &ParentGUID);				//设置地图树关系
_EXPORT_CFGPACK_SDK GUID CFGPACK_AddHotSpotOrArea(const GUID &EMapGUID, const char *szSpotOrAreaName, const GUID &SpotOrAreaResGUID);	//在地图上增加一个热点热区
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteHotSpotOrArea(const GUID &HotSpotOrAreaGUID);						//删除热点或热区
_EXPORT_CFGPACK_SDK bool CFGPACK_SetHotSpotOrAreaProperty(const GUID &HotSpotOrAreaGUID, const char * color, unsigned char imageIndex, const char * xPos, const char * yPos,tuint32 nZoomRate);
_EXPORT_CFGPACK_SDK bool CFGPACK_AddHotSpotEvent(const GUID &HotSpotGUID, tuint32 dwEvent);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteHotSpotEvent(const GUID &HotSpotGUID, tuint32 dwEvent);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetHotSpotOrAreaName(const GUID &HotSpotOrAreaGUID, const char *szName);	//更改热点或热区名称
/////////////////////////////////////////////////////////////

//输出配置相关接口
// _EXPORT_CFGPACK_SDK GUID CFGPACK_CreateOutput(const GUID &ServerGUID, const char *szName, tuint8 ucIndex, tuint8 OutputType);    //创建一个输出
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetOutputSplitScreenNumber(const GUID &OutputGUID, tuint8 ucSplitMode); //设置输出的分割模式
// _EXPORT_CFGPACK_SDK bool CFGPACK_AlarmBindOutput(const GUID &OutputGUID, bool bAlarmBind);  //设置报警绑定输出
// _EXPORT_CFGPACK_SDK bool CFGPACK_DeleteOutput(const GUID &OutputGUID); //删除输出
// _EXPORT_CFGPACK_SDK GUID CFGPACK_CreateOutputFrame(const GUID &OutputGUID, const char *szName, tuint8 ucFrameIndex); //创建输出的一个输出框
// _EXPORT_CFGPACK_SDK bool CFGPACK_DeleteOutputFrame(const GUID &OutputFrameGUID); //删除输出框,同时也删除输出框绑定的通道
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetOutputFrameChannel(const GUID &OutputFrameGUID, const GUID &ChannelGUID);//设置输出框绑定通道,如果ChannelGUID为GUID_NULL清除当前绑定的通道
// _EXPORT_CFGPACK_SDK GUID CFGPACK_CreateChannelGroup(const char *szName, tuint8 ChannelGroupType);//创建一个通道组，ChannelGroupType: 1为通道组，其他为方案
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelGroupDwellTime(const GUID &ChannelGroupGUID, tuint32 dwDwellTime);//设置通道组轮询时间
// _EXPORT_CFGPACK_SDK bool CFGPACK_DeleteChannelGroup(const GUID &ChannelGroupGUID);//删除通道组
// _EXPORT_CFGPACK_SDK bool CFGPACK_PushChannelGroupItem(const GUID &ChannelGroupGUID, const GUID &ChannelGUID, tuint8 ucPageIndex, tuint8 ucFrameIndex);//添加预案的一个条目
// _EXPORT_CFGPACK_SDK bool CFGPACK_PopChannelGroupItem(const GUID &ChannelGroupGUID, const GUID &ChannelGUID, tuint8 ucPageIndex, tuint8 ucFrameIndex);//移除预案的一个条目
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetOutputChannelGroup(const GUID &OutputGUID, const GUID &ChannelGroupGUID);//设置输出绑定方案,如果PreplanGUID为GUID_NULL清除当前绑定的通道组

//电视墙相关,墙，不是服务
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateTVWall(const char *szName,const char *szDescription,const tuint32 nKeyboardIndex ,const GUID &TVWServerGUID,GUID wallguid = GUID_NULL);	//创建电视墙
_EXPORT_CFGPACK_SDK bool CFGPACK_SetTVWallName(const GUID &WallGUID, const char *szName);	//设置名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetTVWallDescription(const GUID &WallGUID, const char *szDescription);	//设置描述
_EXPORT_CFGPACK_SDK bool CFGPACK_SetTVWallKeyboardIndex(const GUID &WallGUID, tuint32 keyboardIndex);						//设置键盘编号
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyTVWallRelationTVWServer(const GUID &WallGUID,const GUID &TVWServerGUID);	//修改所属电视墙服务
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteTVWall(const GUID &WallGUID);

//通道组相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateChannelGroupForN5000(const char *szName,const char *szDescription,const tuint32 nKeyboardIndex,const tuint32 DwellTime,GUID parentGUID);	//创建通道组
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelGroupNameForN5000(const GUID &ChannelGroupGUID, const char *szName);	//设置名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelGroupDescriptionForN5000(const GUID &ChannelGroupGUID, const char *szDescription);	//设置描述
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelGroupKeyboardIndexForN5000(const GUID &ChannelGroupGUID, tuint32 keyboardIndex);		
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelGroupDwellTimeForN5000(const GUID &ChannelGroupGUID, tuint32 dwellTime);	//设置轮循间隔时间
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteChannelGroupForN5000(const GUID &ChannelGroupGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DelChannelFromGroupForN5000(const GUID &ChannelGroupGUID, const GUID &ChannelGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_AddChannelToGroupForN5000(const GUID &ChannelGroupGUID, const GUID &ChannelGUID, tuint32 ChannelIndexInGroup);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetChannelGroupParentGUIDForN5000(const GUID &ChannelGroupGUID, const GUID &parentChannelGroupGUID);

//报警输出组相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAlarmOutGroupForN5000(const char *szName,const char *szDescription,GUID parentGUID);	//创建通道组
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmOutGroupNameForN5000(const GUID &ChannelGroupGUID, const char *szName);	//设置名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmOutGroupDescriptionForN5000(const GUID &ChannelGroupGUID, const char *szDescription);	//设置描述
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAlarmOutGroupForN5000(const GUID &ChannelGroupGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DelAlarmOutFromGroupForN5000(const GUID &ChannelGroupGUID, const GUID &ChannelGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_AddAlarmOutToGroupForN5000(const GUID &ChannelGroupGUID, const GUID &ChannelGUID, tuint32 ChannelIndexInGroup);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmOutGroupParentGUIDForN5000(const GUID &ChannelGroupGUID, const GUID &parentChannelGroupGUID);

//处警标准操作指导相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateSop(const char *szName);	//创建处警标准操作指导
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateSopAction(const char *szName);	//创建处警动作
_EXPORT_CFGPACK_SDK bool CFGPACK_SetSopName(const GUID &SopGUID, const char *szName);	//设置名称
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteSop(const GUID &SopGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DelSopActionFromSop(const GUID &SopGUID, const GUID &SopActionGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_AddSopActionToSop(const GUID &SopGUID, const GUID &SopActionGUID, const char *szName, tuint32 SopActionIndexInSop);

//告警任务相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAlarmTask(const char *szName, const char *szDescription, const tuint32 alarmType, const GUID &scheduleID, const tuint32 triggerTime);
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAlarmTask(const char *szName, const char *szDescription, const tuint32 alarmType, const GUID &scheduleID, tuint32 targetType, tint32 maxCount);

_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTaskName(const GUID &AlarmTaskGUID, const char *szName);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTaskDescription(const GUID &AlarmTaskGUID, const char *szDescription);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTaskAlarmType(const GUID &AlarmTaskGUID, const tuint32 alarmType);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTaskScheduleID(const GUID &AlarmTaskGUID, const GUID &scheduleID);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTaskTriggerTime(const GUID &AlarmTaskGUID, const tuint32 triggerTime);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTaskTargetType(const GUID &AlarmTaskGUID, const tuint32 targetType);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTaskMaxCount(const GUID &AlarmTaskGUID, const tint32 maxCount);

_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAlarmTask(const GUID &AlarmTaskGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAlarmTaskFromGroup(const GUID &AlarmTaskGUID, const GUID &ResourceGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_AddAlarmTaskToGroup(const GUID &AlarmTaskGUID, const GUID &ResourceGUID);

//邮件信息
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateEmail(const char *szName, const char *szAddr, const char *senderInfo, tuint32 type);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetEmailProperty(const GUID &EmailGUID, const char *szName, const char *szAddr, const char *senderInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteEmail(const GUID &EmailGUID);

//报警主机相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAlarmHost(const char *szName, tuint32 ProtocalType/*协议类型*/, tuint32 HostModel/*主机型号*/,const char *szRes, const GUID &ParentAreaGUID,const GUID &JSUServerGUID/*接入服务器*/);//创建报警主机
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostProperty(const GUID &AlarmHostGUID, tuint32 ProtocalType, int nSubSystemNum, int nZoneNum,int nALarmOutNum,const GUID &JSUServerGUID,const GUID &AreaGUID);	//设置设备属性
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostIPPort(const GUID &AlarmHostGUID, const char *szIP, tuint16 nPort);	//设置登录设备的IP地址和端口,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostUserNamePSW(const GUID &AlarmHostGUID, const char *szUserName, const char *szPSW);	//设置登录设备的用户名和密码,如果为NULL则不修改,如果改为空则传空字符串
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostSerial(const GUID &AlarmHostGUID, const char *szSerial);							//设置主动上报设备的序列号
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostProtocalType(const GUID &AlarmHostGUID, tuint32 protocalType);						//设置协议类型
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostModel(const GUID &AlarmHostGUID, tuint32 model);						//设置主机型号
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostName(const GUID &AlarmHostGUID, const char *szName);						//设置设备名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmHostJSU(const GUID &AlarmHostGUID,const GUID &JsuGUID);	//设置设备所属接入服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAlarmHostJSU(const GUID &AlarmHostGUID,const GUID &JsuGUID);	//修改设备所属接入服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAlarmHostArea(const GUID &AlarmHostGUID,const GUID &AreaGUID);	//修改设备所属区域
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAlarmHostRes(const GUID &AlarmHostGUID,const char *szRes);	//修改预留信息
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAlarmHost(const GUID &AlarmHostGUID);			//删除报警主机, 自动删除其下子系统防区及相关信息,包括逻辑和物理关系

//子系统相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateSubSystem(const char *szName,const char *szRes, const GUID &AlarmHostGUID, tuint8 nSubSystemIndex);	
_EXPORT_CFGPACK_SDK bool CFGPACK_SetSubSystemName(const GUID &SubSystemGUID, const char *szName);	//更改名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetSubSystemNo(const GUID &SubSystemGUID, tuint8 nSubSystemIndex);//更改序号
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifySubSystemRes(const GUID &SubSystemGUID,const char *szRes);	//修改预留信息
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteSubSystem(const GUID &SubSystemGUID);	
//防区相关
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateZone(const char *szName,const char *szRes, const GUID &AlarmHostGUID,const GUID &SubSystemGUID,  tuint8 nZoneIndex);	
_EXPORT_CFGPACK_SDK bool CFGPACK_SetZoneName(const GUID &ZoneGUID, const char *szName);	//更改名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetZoneNo(const GUID &ZoneGUID, tuint8 nZoneIndex);//更改序号
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyZoneSubSystem(const GUID &ZoneGUID,const GUID &SubSystemGUID);	//修改子系统
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyZoneRes(const GUID &ZoneGUID,const char *szRes);	//修改预留信息
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyZoneType(const GUID &ZoneGUID,tuint8 nZoneType);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyZoneImageType(const GUID &ZoneGUID,tuint8 nZoneImageType);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteZone(const GUID &ZoneGUID);	
// 包括设置子系统布防，撤防，消警
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetSubsystemAlarmSchedule(const GUID &nodeGUID, const GUID &ScheduleGUID , int nAlarmType);
// 包括设置防区旁路，旁路恢复，防区报警
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetZoneAlarmSchedule(const GUID &nodeGUID, const GUID &ScheduleGUID , int nAlarmType);

//门禁考勤系统
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAcsSystem(const char *szName, tuint32 ProtocalType/*协议类型*/, const char *szRes,const GUID &JSUServerGUID/*接入服务器*/);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAcsSystemIPPort(const GUID &AcsSystemGUID, const char *szIP, tuint16 nPort);	
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAcsSystemUserNamePSW(const GUID &AcsSystemGUID, const char *szUserName, const char *szPSW,const char *szKey);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAcsSystemProtocalType(const GUID &AcsSystemGUID, tuint32 protocalType);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAcsSystemName(const GUID &AcsSystemGUID, const char *szName);						//设置设备名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAcsSystemJSU(const GUID &AcsSystemGUID,const GUID &JsuGUID);	//设置设备所属接入服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAcsSystemJSU(const GUID &AcsSystemGUID,const GUID &JsuGUID);	
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAcsSystemRes(const GUID &AcsSystemGUID,const char *szRes);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAcsSystem(const GUID &AcsSystemGUID);		

//门禁考勤设备
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAcsDevice(const char *szName, tuint32 nStatus, const char *szBelongModule,\
	const char* szDevModelName,const char* szIdEx,const char* szSerialNo,const char *szRes,const GUID &AcsSystemGUID/*门禁系统*/,const GUID &JSUServerGUID/*接入服务器*/);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAcsDeviceInfo(const GUID &AcsDeviceGUID,const char *szName, tuint32 nStatus, const char *szBelongModule,\
	const char* szDevModelName,const char* szIdEx,const char* szSerialNo,const char *szRes,const GUID &AcsSystemGUID/*门禁系统*/);	
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAcsDevice(const GUID &AcsDeviceGUID);	

//门
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAcsDoor(const char *szName,const char* szIdEx,const GUID &AcsDeviceGUID/*门禁设备*/,const char *szRes,const GUID &JSUServerGUID/*接入服务器*/);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAcsDoorInfo(const GUID &AcsDoorGUID,const char *szName,const char* szIdEx,const GUID &AcsDeviceGUID/*门禁设备*/,const char *szRes);	
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAcsDoor(const GUID &AcsDoorGUID);	
//门报警联动排程
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetAcsDoorAlarmSchedule(const GUID &nodeGUID, const GUID &ScheduleGUID , int nAlarmType);
// _EXPORT_CFGPACK_SDK bool CFGPACK_SetAcsDeviceAlarmSchedule(const GUID &nodeGUID, const GUID &ScheduleGUID , int nAlarmType);

//读头
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAcsReader(const char *szName,const char* szIdEx,const char *szReaderNo,tuint32 nReaderState,const GUID &AcsDoorGUID/*门*/,const char *szRes,const GUID &JSUServerGUID/*接入服务器*/);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAcsReaderInfo(const GUID &AcsReaderGUID,const char *szName,const char* szIdEx,const char *szReaderNo,tuint32 nReaderState,const GUID &AcsDoorGUID/*门*/,const char *szRes);	
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteAcsReader(const GUID &AcsReaderGUID);	

//目标库
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateAlbum(GUID AlbumGUID,const char *szName,const char *szRes1,const char *szRes2,const GUID &ANSServerGUID/*智能服务器*/,const GUID &FatherGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyAlbum(GUID AlbumGUID,const char *szName,const char *szRes1,const char *szRes2,const GUID &ANSServerGUID/*智能服务器*/,const GUID &FatherGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DelAlbum(GUID AlbumGUID);

//设置任务排程
_EXPORT_CFGPACK_SDK bool CFGPACK_SetTaskTypeSchedule(const GUID &nodeGUID, const GUID &ScheduleGUID, tuint32 nTaskType);	

//设置报警排程
_EXPORT_CFGPACK_SDK bool CFGPACK_SetAlarmTypeSchedule(const GUID &nodeGUID, const GUID &ScheduleGUID , tuint32 nAlarmType);	

//PMS相关接口
/*
_EXPORT_CFGPACK_SDK bool CFGPACK_CreateDefaultPark(const char *szMainName, const char *szSubName, tuint32 subNum, tuint32 parkSpace, tuint32 parkRemainSpace);
_EXPORT_CFGPACK_SDK bool CFGPACK_CreateDefaultGateway(const char *szGateName, tuint32 nGateNum1[], tuint32 num1, tuint32 nLaneNum2[], tuint32 num2);
_EXPORT_CFGPACK_SDK bool CFGPACK_CreateDefaultLane(const char *szLaneName, tuint32 nGateNum1[], tuint32 num1, tuint32 nLaneNum2[], tuint32 num2);
*/
//创建停车场
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreatePMSPark(const char *szName/*名称*/,tuint32 nEnable/*启用状态*/, GUID parentParkID/*父停车场ID*/, tuint32 nGatewayNum/*出入口数量*/,
	const char * other/*备注*/,tuint32 nParkingSpace/*车位数量*/,tuint32 nRemainParkingSpace/*车位剩余数量*/,tuint32 nIndex/*序号*/,GUID ParkChargeRuleGUID/*收费规则GUID*/);

//创建出入口
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreatePMSGateway(const char *szName/*名称*/,tuint32 nEnable/*启用状态*/, GUID parentParkID/*父停车场ID*/, tuint32 nLaneNum/*关联车道数量*/,
	tuint32 nIndex/*序号*/,const char * other/*备注*/);
//创建车道
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreatePMSLane(const char *szName/*名称*/,tuint32 nEnable/*启用状态*/,tuint32 nTempCarIn/*临时车可否通行*/,tuint32 nLaneType/*车道类型*/,
	tuint32 nAutoReleaseRule/*是否自动放行*/, GUID GatewayGUID/*父出入口GUID*/,tuint32 nIndex/*序号*/,const char * other/*备注*/);

//固定车
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsAddFixedCar(CFGPACK_DEFINE::PMS_FIXED_VEHICLE_INFO *pFixedCarInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveFixedCar(CFGPACK_DEFINE::PMS_FIXED_VEHICLE_INFO *pFixedCarInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveGateway(CFGPACK_DEFINE::PMS_GATEWAY_INFO* pGateInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveLane(CFGPACK_DEFINE::PMS_LANE_INFO *pLaneInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelFixedCar(const GUID &PmsFixedCarGUID);
//黑名单车
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsAddBlacklistCar(CFGPACK_DEFINE::PMS_BLACKLIST_VEHICLE_INFO *pBlacklistCarInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveBlacklistCar(CFGPACK_DEFINE::PMS_BLACKLIST_VEHICLE_INFO *pBlacklistCarInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelBlacklistCar(const GUID &PmsBlacklistCarGUID);

//车辆组
_EXPORT_CFGPACK_SDK GUID CFGPACK_PmsAddVehicleGroup(CFGPACK_DEFINE::PMS_VEHICLE_GROUP_INFO *pVehicleGroupInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveVehicleGroup(CFGPACK_DEFINE::PMS_VEHICLE_GROUP_INFO *pVehicleGroupInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelVehicleGroup(const GUID &PmsVehicleGroupGUID);

//收费规则
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsAddChargeRule(CFGPACK_DEFINE::PMS_CHARGE_RULE_INFO *chargeRuleInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveChargeRule(CFGPACK_DEFINE::PMS_CHARGE_RULE_INFO *chargeRuleInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelChargeRule(const GUID &PmsDelChargeRuleGUID);
//包期规则
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsAddBagPeriodRule(CFGPACK_DEFINE::PMS_BAG_PERIOD_RULE_INFO *bagPeriodRuleInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveBagPeriodRule(CFGPACK_DEFINE::PMS_BAG_PERIOD_RULE_INFO *bagPeriodRuleInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelBagPeriodRule(CFGPACK_DEFINE::PMS_BAG_PERIOD_RULE_INFO *bagPeriodRuleInfo);
//保存停车场信息
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveParkInfo(CFGPACK_DEFINE::PMS_PARK_INFO *parkInfo);
//保存出入口
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSavePMSGateway(CFGPACK_DEFINE::PMS_GATEWAY_INFO *gatewayInfo);
//保存车道
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSavePMSLane(CFGPACK_DEFINE::PMS_LANE_INFO *laneInfo);

//删除 1:停车场/2:出入口/3:车道
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelParkGatewayLane(const GUID &PmsDelGUID, tuint32 nType);

//自定义时间段的收费规则
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsAddTimeSectionChargeRuleInfo(CFGPACK_DEFINE::PMS_TIME_SECTION_CHARGE_RULE_INFO *timeSectionChargeRuleInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelTimeSectionChargeRuleInfo(CFGPACK_DEFINE::PMS_TIME_SECTION_CHARGE_RULE_INFO *timeSectionChargeRuleInfo);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveTimeSectionChargeRuleInfo(CFGPACK_DEFINE::PMS_TIME_SECTION_CHARGE_RULE_INFO *timeSectionChargeRuleInfo);
//LED屏
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsAddLED(const char *szName, tuint32 nodeType, const char *szIP, tuint32 port, const char *szAppend);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsSaveLED(const GUID guidLed, const char *szName, tuint32 nodeType, const char *szIP, tuint32 port, const char *szAppend);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelLED(const GUID guidLED);
//车道和摄像机或显示屏的绑定表
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsAddLaneDev(CFGPACK_DEFINE::BINARY_LANE_DEVICE *laneDevice);
_EXPORT_CFGPACK_SDK bool CFGPACK_PmsDelLaneDev(CFGPACK_DEFINE::BINARY_LANE_DEVICE *laneDevice);

//级联平台
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateCascadePlatform(const char *szName, tuint32 ProtocalType/*协议类型*/,const char * gb28181ID,const GUID &CGUServerGUID/*级联服务器*/,const GUID &areaGUID);
_EXPORT_CFGPACK_SDK bool CFGPACK_SetCascadePlatformName(const GUID &CascadePlatformGUID, const char *szName);						//设置名称
_EXPORT_CFGPACK_SDK bool CFGPACK_SetCascadePlatformCGU(const GUID &CascadePlatformGUID,const GUID &CguGUID);	//设置平台所属网关服务器
_EXPORT_CFGPACK_SDK bool CFGPACK_SetCascadePlatformIPPort(const GUID &CascadePlatformGUID, const char *szIP, tuint16 nPort);	//设置下级平台的ip和端口
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyCascadePlatformArea(const GUID &CascadePlatformGUID, const GUID &AreaGUID);	
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyCascadePlatformCGU(const GUID &CascadePlatformGUID,const GUID &CguGUID);	
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyCascadePlatformGB28181ID(const GUID &CascadePlatformGUID,const char *szGB28181ID);
_EXPORT_CFGPACK_SDK bool CFGPACK_DeleteCascadePlatform(const GUID &CascadePlatformGUID);	
_EXPORT_CFGPACK_SDK void CFGPACK_SetGB28181XML(const tagXmlConfig &GbXmlConfig);	

//配置
_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateConfig(tuint32 configKey, const char *szConfigValue);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyConfig(const GUID &configGUID, tuint32 configKey, const char *szConfigValue);

_EXPORT_CFGPACK_SDK GUID CFGPACK_CreateCustom(tuint32 customIdx, const char *szConfigValue);
_EXPORT_CFGPACK_SDK bool CFGPACK_ModifyCustom(const GUID &configGUID, tuint32 customIdx, const char *szConfigValue);


#endif //_CFG_PACK_SDK_H
