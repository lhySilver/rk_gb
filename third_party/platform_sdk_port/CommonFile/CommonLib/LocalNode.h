#ifndef __LOCALNODE_H__
#define __LOCALNODE_H__

#include "NodeManagerDll.h"
#include "TypeCheck.h"
#include <list>
#include <map>
#include "ConfigPack_Data_Item_ID_Def.h"
#include "ConfigPack_Struct_Def.h"
#include "NetProtocolDefine_ForNVMS.h" //added by yushulin 20150806
#include "Lock.h"
#include "WarningDisable.h"
using namespace NVMS_NET_PROTOCOL;

#pragma pack(push, 4)			//结构体定义为4字节对齐,在gcc里只能是1,2,4
struct CHANNEL_IN_GROUP
{
	GUID channelGUID;
	tuint32 indexInGroup;
	CHANNEL_IN_GROUP()
	{
		channelGUID = GUID_NULL;
		indexInGroup = 0;
	}
};

struct SOPACTION_IN_SOP
{
	GUID sopActionGUID;
	char szSopActionName[256];
	tuint32 indexInSop;
	SOPACTION_IN_SOP()
	{
		sopActionGUID = GUID_NULL;
		memset(szSopActionName, 0, sizeof(szSopActionName));
		indexInSop = 0;
	}
};

typedef struct ALARMTASK_IN_GROUP_
{
	GUID alarmTaskGUID;
	ALARMTASK_IN_GROUP_()
	{
		alarmTaskGUID = GUID_NULL;
	}
}ALARMTASK_IN_GROUP;

struct ALARMOUT_IN_GROUP
{
	GUID alarmOutGUID;
	tuint32 indexInGroup;
	ALARMOUT_IN_GROUP()
	{
		alarmOutGUID = GUID_NULL;
		indexInGroup = 0;
	}
};

class CConfigPack;
class _EXPORT_NODEMANAGER_CLASS CLocalNode : public CTypeCheck
{
protected:
	explicit CLocalNode(tuint32 dwTypeValue);
	virtual ~CLocalNode(void);

public:
	virtual tuint32 GetNodeID(void) const = 0;			//获取节点ID
	virtual const GUID &GetNodeGUID(void) const = 0;	//获取节点GUID

	virtual bool GetNodeInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperateType) const = 0;		//把节点信息打成配置包
	virtual bool GetNodeInfo_Lite(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperateType) ;		//把节点信息打成配置包

	tuint32 GetNodeType(void) const;					//获取节点类型,返回值参见ECMS_NODE_TYPE_DEF

	virtual bool GetConnectIP(char *pBuf, int buflen) const;		//获取节点IP,对有IP的节点有效:对于Sensor,通道,报警输出节点返回所在设备IP
	virtual bool GetConnectSecondIP(char *pBuf, int buflen) const;

	virtual bool GetPort(unsigned short &usPort) const;				//获取端口,对有端口的节点有效:对于Sensor,通道,报警输出节点返回所在设备端口

	virtual bool GetUserName(char *buf, int buflen) const;			//获取用户名:对于Sensor,通道,报警输出节点返回所在设备用户名
	virtual bool GetPassword(char *buf, int buflen) const;			//获取密码:对于Sensor,通道,报警输出节点返回所在设备密码

	virtual bool GetNodeName(char *buf, int buflen) const = 0;		//获取节点名称
	virtual bool GetNodeFullName(char *buf, int buflen) const;		//获取节点的全名,对通道和Sensor,报警输出节点,节点全名为设备名+节点名,其余为节点名称

	virtual bool GetAreaParentNodeID(tuint32 &dwParentID) const = 0;	//获取父区域节点ID
	virtual bool GetAreaParentNodeGUID(GUID &ParentGUID) const = 0;

	virtual bool GetChannelGroupParentNodeID(tuint32 &dwParentID) const = 0;	//获取父区域节点ID
	virtual bool GetChannelGroupParentNodeGUID(GUID &ParentGUID) const = 0;

	virtual bool GetAlarmOutGroupParentNodeID(tuint32 &dwParentID) const = 0;	//获取父报警输出组节点ID
	virtual bool GetAlarmOutGroupParentNodeGUID(GUID &ParentGUID) const = 0;

	virtual tuint32 GetMDUID(void) const = 0;		//获取它的转发服务器ID
	virtual GUID GetMDUGUID(void) const = 0;		//获取它的转发服务器ID

	virtual tuint32 GetMSUID(void) const = 0;		//获取它的存储服务器ID
	virtual GUID GetMSUGUID(void) const = 0;		//获取它的存储服务器ID

	virtual tuint32 GetTVWallServerID(void) const = 0;		//获取它的电视墙服务器ID
	virtual GUID GetTVWallServerGUID(void) const = 0;		//获取它的电视墙服务器ID

	virtual tuint32 GetChannelNum(void) const;		//对于通道节点则为通道号, 对于设备节点为通道数量,其余无效 遍历出来的 对通道组表示通道数量

	virtual tuint32 GetSopActionNum(void) const;		//处警动作数量

	virtual bool GetRecordPath(GUID &PathGUID) const;				//获取通道所属流媒体服务器存储的位置,即分区组,对通道节点有效

	//virtual bool GetSensorSchedule(GUID &ScheduleGUID) const;		//获取传感器报警排程信息,对传感器节点有效

	virtual tuint32 GetSensorInNum(void) const;						//对于Sensor节点则为Sensor号, 对于设备节点为Sensor数量,其余无效
	virtual tuint32 GetAlarmOutNum(void) const;						//对于报警节点则为报警输出序号,对于设备节点为报警输出数量,其余无效

	virtual bool GetRecordSchedule(GUID &ScheduleGUID) const;		//获取录像排程信息,对通道节点有效
	//virtual bool GetMotionRecSchedule(GUID &ScheduleGUID) const;	//获取Motion录像排程信息,对通道节点有效
	//virtual bool GetSensorRecSchedule(GUID &ScheduleGUID) const;	//获取Sensor录像排程信息,对通道节点有效
	
	//virtual bool GetMotionSchedule(GUID &ScheduleGUID) const;		//获取移动报警排程信息,对通道节点有效
	virtual BYTE GetRecordStream(void) const;							//对通道节点有效,获取录像流类型
	virtual int  GetChStreamCount(void) const;							//对通道节点有效
	virtual bool GetStreamResolution(int streamIndex, int &nWidth, int &nHeight) const;		//索引从0开始
	virtual bool GetChStreamInfo(int streamIndex, char *pRetStream, int bufSize) const;		//索引从0开始
	virtual tuint32 GetFaceCaptureSource(void) const;//对通道节点有效,获取人脸抓图源
	//新增报警排程类型，对通道节点有效
// 	virtual bool GetAvdSchedule(GUID &ScheduleGUID) const;
// 	virtual bool GetOscSchedule(GUID &ScheduleGUID) const;
// 	virtual bool GetPeaTripwireSchedule(GUID &ScheduleGUID) const;
// 	virtual bool GetPeaPerimeterSchedule(GUID &ScheduleGUID) const;
// 	virtual bool GetVLossSchedule(GUID &ScheduleGUID) const;//获取视频丢失排程方案

	virtual bool GetNodeAppendInfo(char *szBuf, int bufSize) const;		//获取节点附加信息
	virtual bool GetNodeCreateTimeStr(char *szBuf, int bufSize) const;		//获取节点创建时间，暂时只有编码设备
	virtual bool SetCreateTimeIsNotify() ;//设置是否已经上报过添加设备设备的通知了
	virtual bool GetCreateTimeIsNotify() ;

	virtual tuint32 GetDevID(void) const;								//GetDevID\GetDevGUID对于Sensor,报警输出和通道,IP通道节点,子系统，防区有效，用于获取所属的编码设备或报警主机的ID
	virtual const GUID &GetDevGUID(void) const;
	virtual tuint32 GetHighestLiveStream(void) const;					//获取限制的最高码流,对于IP通道,通道,设备节点有效

	virtual tuint32 GetProtocolType(void) const;					//获取协议类型,对于IP通道和设备节点有效
	/*0为默认(无定制及旧版本)，1支持定制且设备已定制，2支持定制设备未定制*/
    virtual tuint32 GetManufacturerType(void) const;					//获取
	virtual tuint32 GetAlarmOutHoldingTime(void) const;				//获取报警输出延迟时间

	virtual bool IsAlarmInNormallyOpen(void) const;					//报警输入是否常开
	virtual bool IsAlarmInEnable(void) const;						//报警输入是否启用

	//virtual bool GetAlarmOutSchedule(GUID &ScheduleGUID) const;		//获取报警输出排程信息,对报警输出节点有效

	//获取存储服务器分区信息,对存储服务器有效 added by yushulin 20150806
	virtual bool GetPartitionInfo(std::list<NET_PARTITION_INFO> &partInfoList);
	//设置存储服务器分区状态,只有注册服务器需要调用
	virtual bool SetPartitionState(const std::list<NET_PARTITION_INFO> &partInfoList); 

	//add
	virtual GUID GetDevArea() const;//设备节点的区域在设备表中，逻辑关系没有，所以要单在设备信息中查询 暂时是编码设备，解码设备和报警主机用到
	virtual bool SetFormalLoginFailTime(time_t t);//设置正式登录失败的时间
	virtual time_t GetFormalLoginFailTime();//正式登录失败的时间

	//add 20190705
	virtual GUID GetDevSop() const;   //在设备信息中获取SopGUID

	virtual tuint32 GetKeyboardIndex() const;//为监控点的键盘编号
	virtual tuint32 GetOutputNum(void) const;//解码器的输出通道个数
	virtual bool GetWall(GUID &WallGUID) const;		//获取解码器对应的墙,对解码器节点有效
	virtual GUID GetCreateUser() const;		//获取创建的用户
	virtual tuint32 GetDwellTime() const;//获取轮循间隔时间
	virtual GUID GetScheduleGUID() const; //获取告警排程GUID
	virtual tuint32 GetAlarmType() const;  //获取告警任务类型
	virtual tuint32 GetTriggerTime() const;  //获取告警触发时间
	virtual tuint32 GetTargetType() const;   //获取目标类型:人车非 枚举值
	virtual tint32 GetMaxCount() const;      //获取最大保有量阈值
	virtual tuint32 GetAlarmTaskNum() const;   //获取告警任务数量


	//通道组用到
	virtual bool GetChannelListFromGroup(std::list<CHANNEL_IN_GROUP> &ChannelList);//设置通道组的通道列表
	virtual bool AddChannelToGroup(GUID ChannelGUID,tuint32 nIndexInGroup);
	virtual bool DelChannelToGroup(GUID ChannelGUID);
	//处警标准操作用到
	virtual bool GetSopActionListFromSop(std::list<SOPACTION_IN_SOP> &SopActionList);//设置处警标准操作的处警动作列表
	virtual bool AddSopActionToSop(GUID SopActionGUID,const char* pSopActionName, tuint32 nIndexInSop);
	virtual bool DelSopActionToSop(GUID SopActionGUID);

	//报警输出组用到
	virtual bool GetAlarmOutListFromGroup(std::list<ALARMOUT_IN_GROUP> &ChannelList);//设置通道组的通道列表
	virtual bool AddAlarmOutToGroup(GUID AlarmOutGUID,tuint32 nIndexInGroup);
	virtual bool DelAlarmOutToGroup(GUID AlarmOutGUID);

	virtual bool GetNodeEmailAddr(char *buf, int buflen); //获取邮件邮箱地址
	virtual bool GetNodeEmailType(); //获取邮件还是发件人 收0，发1
	virtual bool GetNodeEmailExInfo(char *buf, int buflen); //寄件人的相关信息，服务地址，端口等

	//告警任务组用到
	virtual bool GetAlarmTaskListFromGroup(std::list<ALARMTASK_IN_GROUP> &AlarmTaskList);//设置告警任务组的通道列表
	virtual bool AddAlarmTaskToGroup(GUID AlarmTaskGUID);
	virtual bool DelAlarmTaskToGroup(GUID AlarmTaskGUID);


	//报警主机用到
	virtual bool GetSerial(char *pBuf, int buflen) const;//门禁也用序列号
	virtual tuint32 GetJSUID(void) const = 0;		//获取它的接入服务器ID
	virtual GUID GetJSUGUID(void) const = 0;		//获取它的接入服务器ID
	virtual tuint32 GetCGUID(void) const = 0;		//获取它的级联网关服务器ID
	virtual GUID GetCGUGUID(void) const = 0;		//获取它的级联网关服务器ID

	virtual tuint32 GetSubSystemNum(void) const;//对于子系统节点则为子系统号, 对于设备节点为子系统数量,其余无效 遍历出来的
	virtual tuint32 GetZoneNum(void) const;//对于防区节点则为防区号, 对于设备节点为防区数量,其余无效 遍历出来的
	virtual tuint32 GetZoneDetectorType(void) const;//对于防区节点为探测器类型,其余无效
	virtual tuint32 GetHostModel(void) const;					//获取主机型号,对报警主机有效
	virtual const GUID &GetZoneSubSystemGUID(void) const;//获取防区对应的子系统，只对防区有效
	virtual tuint32 GetImageIndex(void) const;

	//门禁
	virtual bool GetNodeIdEx(char *buf, int buflen) const;
	virtual GUID GetNodeAcsSystemGUID() const;
	virtual GUID GetNodeAcsDeviceGUID() const;
	virtual GUID GetNodeAcsDoorGUID() const;
	virtual tuint32 GetNodeStatus(void) const;//状态
	virtual bool GetNodeBelongModuleName(char *buf, int buflen) const;//所属模块
	virtual bool GetNodeDevModelName(char *buf, int buflen) const;//设备型号名称
	virtual bool GetNodeReaderNo(char *buf, int buflen) const;//读头编号
	virtual tuint32 GetNodeReaderState(void) const;//读头状态


	//是否是智能IPC
	virtual bool IsSmartIPC() const;

	//新增通用的报警排程设置
	virtual bool SetAlarmTypeSchedule(tuint32 nAlarmType, const GUID &ScheduleGUID) ;
	virtual bool GetAlarmTypeSchedule(tuint32 nAlarmType,GUID &ScheduleGUID) ;
	virtual void GetAlarmTypeScheduleMap(std::map<tuint32 , GUID> & mapAlarmType_Schedule) ;
	virtual void SetAlarmTypeScheduleMap(const std::map<tuint32 , GUID> & mapAlarmType_Schedule) ;

	//新增通用的任务排程设置
	virtual bool SetTaskTypeSchedule(tuint32 nType, const GUID &ScheduleGUID) ;
	virtual bool GetTaskTypeSchedule(tuint32 nType,GUID &ScheduleGUID) ;
	virtual void GetTaskTypeScheduleMap(std::map<tuint32 , GUID> & mapTaskType_Schedule) ;
	virtual void SetTaskTypeScheduleMap(const std::map<tuint32 , GUID> & mapTaskType_Schedule) ;

	//联动开关
	virtual bool SetTriggerSwitch(tuint32 AlarmType,tuint32 nSwitch);	
	virtual bool IsSwitchEnable(tuint32 AlarmType,tuint32 nTriggerSwitchType);
	virtual void GetTriggerSwitchMap(std::map<tuint32 , GUID> & mapAlarmTypeToTriggerSwitch);
	virtual bool SetTriggerSwitchMap(const std::map<tuint32 , GUID> mapAlarmTypeToTriggerSwitch);

	virtual tuint32 GetNodeAbstractType(void) const;

	virtual void CopyMap(CLocalNode *pNewNode);
	//获取服务器是否是缺省创建的
	virtual tuint32 GetIsDefault() const;
	//获取服务器是否是热备服务器
	virtual bool IsBackupServer() const;
	//PMS新增
	virtual tuint32 GetEnable() const;//获取启用状态1-启用 0-未启用 -- 停车场节点\出入口节点\车道节点使用
	virtual GUID GetParentParkGUID(void) const;//获取父停车场GUID -- 停车场节点\出入口节点使用
	virtual tuint32 GetGatewayNum() const;//获取出入口数量 -- 停车场节点使用
	virtual bool GetNodeMemo(char *buf, int buflen) const;//获取备注信息
	virtual tuint32 GetParkingSpaceNum() const;//获取车位数量  -- 停车场节点使用
	virtual tuint32 GetRemainParkingSpaceNum() const;//获取剩余车位数量  -- 停车场节点使用
	virtual tuint32 GetNodeIndex() const;//获取序号 -- 停车场节点\出入口节点\车道节点使用
	virtual GUID GetParkChargeRuleGUID() const;//获取收费规则  -- 停车场节点使用
	virtual tuint32 GetLaneNum() const;//获取车道数量 -- 出入口节点使用
	virtual tuint32 GetTempCarIn() const;//临时车可否通行 1-允许 0-不允许 -- 车道节点使用
	virtual tuint32 GetLaneType(void) const;//获取车道类型 1-入口 2-免费出口 3-收费出口 -- 车道节点使用
	virtual tuint32 GetAutoReleaseRule() const;//是否自动放行 -暂时未使用 -- 车道节点使用
	virtual GUID GetGatewayGUID() const;//所属出入口GUID -- 车道节点使用

	virtual tuint32 GetVehicleType() const;//车辆类型 1小型车 2大型车 3其他 -- 收费规则节点\固定车节点使用
	virtual tuint32 GetChargeType() const;//收费类型 0免费 1按次收费 2按时收费 3自定义收费  -- 收费规则节点使用
	virtual float GetChargeAmount() const;//收费金额  -- 收费规则节点使用
	virtual tuint32 GetFreeParkMinute() const;//免费时长 分钟    -- 收费规则节点使用
	virtual tuint32 GetDayMaxAmount() const;//每天最大收费金额   -- 收费规则节点使用
	virtual tuint32 GetFirstSectionMinute() const;//首段收费时间 -- 收费规则节点使用
	virtual tuint32 GetFirstSectionAmount() const;//首段收费金额 -- 收费规则节点使用
	virtual tuint32 GetIntervalMinute() const;//单位停车时间 -- 收费规则节点使用
	virtual tuint32 GetIntervalAmount() const;//单位收费金额 -- 收费规则节点使用
	virtual tuint32 GetMatchTempCar() const;//是否匹配临时车辆 0匹配 1不匹配 -- 收费规则节点使用
	virtual bool GetLicensePlate(char *buf, int buflen) const;//车牌号 -- 固定车节点使用
	virtual tuint64 GetParkRight() const;//停车场的停车权限 1向左位移“停车场的序号Node_Index”这么多位，该位为1代表有该停车场的停车权限 -- 固定车节点使用
	virtual tuint32 GetParkType() const;//停车类型 0月租车 1黑名单 2其他 -- 固定车节点/车辆组使用
	virtual bool GetVehicleColor(char *buf, int buflen) const;//车辆颜色 -- 固定车节点使用
	virtual bool GetOwnerName(char *buf, int buflen) const;//车主姓名 -- 固定车节点使用
	virtual bool GetOwnerPhone(char *buf, int buflen) const;//车主电话 -- 固定车节点使用
	virtual tuint32 GetStartTime() const;//月租开始时间 -- 固定车节点使用
	virtual tuint32 GetEndTime() const;//月租结束时间 -- 固定车节点使用
	virtual tuint32 GetEnableBlock() const;//是否禁止进入 -- 黑名单车辆节点使用

	virtual GUID GetParentGUID() const;//获取所属收费规则GUID，车辆所属组GUID  -- 自定义收费时间段，固定车辆节点使用
	virtual bool GetSectionStartTime(char *buf, int buflen) const;//开始时间 -- 自定义收费时间段节点使用
	virtual bool GetSectionEndTime(char *buf, int buflen) const;//结束时间 -- 自定义收费时间段节点使用
	virtual tuint32 GetChargeUnitTime() const;//收费单位时间 -- 自定义收费时间段节点使用
	virtual tuint32 GetAmount() const;//收费单位金额 -- 自定义收费时间段节点使用/月卡包月节点使用
	virtual tuint32 GetminChargeTime() const;
	virtual tuint32 GetmaxAmount() const;
	virtual tuint32 GetTime() const;//获取收费时间 -- 月卡包月节点使用

	virtual bool AddBindDevice(const GUID &DeviceGUID/*摄像机或显示屏*/);//设置绑定的摄像机或显示屏 -- 车道节点使用
	virtual bool DelBindDevice(const GUID &DeviceGUID);
	virtual bool GetBindDeviceList(std::list<GUID> &ListGUID);//获取绑定的摄像机或显示屏 -- 车道节点使用

	virtual bool GetGB28181ID(char *buf, int buflen) const;//获取国标编码 级联网关/国标设备/国标平台用到
	virtual bool GetUpperGB28181ID(char *buf, int buflen) const;//自身当作上联网关是的设备/通道的上联国标编码
	virtual bool GetGB28181AppendInfo(char *buf, int buflen) const;//获取国标设备的国标相关附加信息 国标设备用到
	virtual GUID GetCascadePlatformGUID() const;//获取级联网关的GUID
	virtual bool GetGB28181Password(char *buf, int buflen) const;//获取国标密码 级联网关用到
	virtual bool GetDevModel(char *buf, int buflen) const;
	virtual bool GetDevVersion(char *buf, int buflen) const;
	virtual bool GetDevHttpPort(unsigned int &httpPort) const;
	/*rtsp模拟长连接*/
	virtual bool GetRtspUrl(char *buf, int buflen) const;//获取rtsp的url
	virtual bool GetNodeRes(char *buf, int buflen) const;//门禁KANTECH用到了

	//加密登录认证新增
	virtual bool GetLoginNonce(int& loginNonce) const;			//获取用户名:对于Sensor,通道,报警输出节点返回所在设备用户名

	tuint32 GetConnectCount(){m_nConnectCount++; return m_nConnectCount;}

	virtual tuint32 GetConfigKey(void) const;//系统配置key

	virtual GUID GetAlbumANSServerGUID(void) const;//目标库获取所属智能分析服务器
	virtual bool GetAlbumRes1(char *buf, int buflen) const;//目标库获取保留字段1
	virtual bool GetAlbumRes2(char *buf, int buflen) const;//目标库获取保留字段2
	virtual GUID GetParentOrganizationGUID() const;
	virtual bool GetGAT1400ID(char *buf, int buflen) const;//作为上级时，设备发过来的国标id
	virtual bool GetUpperGAT1400ID(char *buf, int buflen) const;//作为下级时，发送给上级的国标id

private:
	//新增通用的报警排程设置，上面那些设置排程接口都不用
	CLock m_mapAlarmType_Schedule_Lock;
	std::map<tuint32 , GUID> m_mapAlarmType_Schedule;
	//新增通用的任务排程设置，上面那些设置排程接口都不用
	CLock m_mapTaskType_Schedule_Lock;
	std::map<tuint32 , GUID> m_mapTaskType_Schedule;

	CLock m_mapAlarmTypeToTriggerSwitchLock;
	std::map<tuint32,tuint32> m_mapAlarmTypeToTriggerSwitch;//报警联动对应的联动开关map

	CLock m_listBindDeviceLock;
	std::list<GUID> m_listBindDevice;//绑定的摄像机或显示屏 -- 车道节点使用
	tuint32 m_nConnectCount;//连接计数，用于P2P设备，单双数区分1.0和2.0
};

#pragma pack(pop)  
#endif //__LOCALNODE_H__
