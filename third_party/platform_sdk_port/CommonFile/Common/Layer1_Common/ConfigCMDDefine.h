#ifndef CONFIG_CMD_DEFINE_HEAD
#define CONFIG_CMD_DEFINE_HEAD

#include "NVMS1000RightDefine.h"
#include <string>
#include <map>

using namespace std;

namespace CFG_CMD_DEFINE
{
	static const string HTTP_CMD_EDITSYSRECPARTITION("editSysRecPartition");//修改系统录像的分区信息

	//服务器相关命令
	static const string HTTP_CMD_CREATE_SERVER("createServer");//创建服务器
	static const string HTTP_CMD_QUERY_SERVER("queryServer");//查询服务器
	static const string HTTP_CMD_EDIT_SERVER("editServer");//修改服务器
	static const string HTTP_CMD_DEL_SERVER("delServer");//删除服务器

	//电子地图相关命令
	static const string HTTP_CMD_CREATEEMAP("createEMap");//创建电子地图
	static const string HTTP_CMD_EDITEMAPNAME("editEMapName");//修改电子地图名称
	static const string HTTP_CMD_QUERYEMAPTREE("queryEMapTree");//请求电子地图树
	static const string HTTP_CMD_DELEMAP("delEMap");//删除电子地图
	static const string HTTP_CMD_CREATEHOTAREA("createHotArea");//添加热区
	static const string HTTP_CMD_QUERYHOTAREA("queryHotArea");//查询热区
	static const string HTTP_CMD_EDITHOTAREA("editHotArea");//修改热区信息
	static const string HTTP_CMD_DELHOTAREA("delHotArea");//删除热区
	static const string HTTP_CMD_CREATEHOTPOINT("createHotPoint");//添加热点
	static const string HTTP_CMD_QUERYHOTPOINT("queryHotPoint");//查询热点信息
	static const string HTTP_CMD_EDITHOTPOINT("editHotPoint");//修改热点信息
	static const string HTTP_CMD_DELHOTPOINT("delHotPoint");//删除热点
	static const string HTTP_CMD_QUERYHOTPOSITION("queryHotPosition");//查询热区，热点位置
	static const string HTTP_CMD_EDITHOTPOSITION("editHotPosition");//修改热区，热点位置

	//add by wangxin
	static const string HTTP_CMD_QUERYEMAP("queryEMap");//请求电子地图信息
	static const string HTTP_CMD_EDITEMAP("editEMap");
	static const string HTTP_CMD_QUERYEMAPLIST("queryEMapList");
	static const string HTTP_CMD_QUERYHOTPOINTLIST("queryHotPointList");
	static const string HTTP_CMD_QUERYHOTPOINTBYRESOURCE("queryHotPointByResource");//根据具体资源查询热点信息

	//用户账户和权限相关命令
	static const string HTTP_CMD_QUERYUSERLIST("queryUserList");//查询用户列表
	static const string HTTP_CMD_QUERYUSER("queryUser");//查询用户信息
	static const string HTTP_CMD_CREATEUSER("createUser");//创建用户
	static const string HTTP_CMD_EDITUSER("editUser");//修改用户信息
	static const string HTTP_CMD_DELUSER("delUser");//删除用户
	static const string HTTP_CMD_RETRIEVEUSERPWD("retrieveUserPwd");//获取超管用户admin的加密密码
	static const std::string HTTP_CMD_EDITUSERPASSWORD("editUserPassword");						// 修改用户密码
	static const std::string HTTP_CMD_RESETUSERPASSWORD("resetUserPassword");						// 重置用户密码

	//权限组
	static const string HTTP_CMD_QUERYAUTHGROUPLIST("queryAuthGroupList");//查询权限组列表不包含权限信息
	static const string HTTP_CMD_QUERYAUTHGROUP("queryAuthGroup");//查询权限组单项的信息包含权限信息
	static const string HTTP_CMD_CREATEAUTHGROUP("createAuthGroup");//创建权限组
	static const string HTTP_CMD_EDITAUTHGROUP("editAuthGroup");//修改权限组
	static const string HTTP_CMD_DELAUTHGROUP("delAuthGroup");//删除权限组

	//密保问题（密码重置时使用）
	static const string HTTP_CMD_SETSECURITYQUESTION("setSecurityQuestion");//设置密保问题
	static const string HTTP_CMD_QUERYSECURITYQUESTION("querySecurityQuestion");//查询密保问题
	static const string HTTP_CMD_AUTHSECURITYQUESTION("authSecurityQuestion");//验证密保问题
	static const string HTTP_CMD_DELSECURITYQUESTION("deleteSecurityQuestion");//删除密保问题
	static const string HTTP_CMD_QUERYDYNAMICINFO("queryDynamicInfo");//查询动态口令信息
	static const string HTTP_CMD_AUTHDYNAMICRESULT("authDynamicResult");//验证动态口令

	//节点
	//static const string HTTP_CMD_RECOVERDEFAULTAUTHORITY("recoverDefaultAuthority");//恢复默认权限
	//static const string HTTP_CMD_EDITUSERAUTHORITY("editUserAuth");//修改用户权限(包括设备监控点,电子地图,系统)
	static const string HTTP_CMD_EDIT_NODE_ENCODEINFO("editNodeEncodeInfo");//修改节点编码信息
	static const string HTTP_CMD_QUERY_NODE_ENCODEINFO("queryNodeEncodeInfo");//查询节点编码信息  
	static const string HTTP_CMD_COPY_NODE_ENCODEINFO("copyNodeEncodeInfo");//节点编码信息复制到
 	static const string HTTP_CMD_EDIT_NODE_APPENDINFO("editNodeAppendInfo");//修改节点附加信息

	//设备系统配置
	static const string HTTP_CMD_QUERY_DEV_SYSCFG("queryDevSysCfg");//设备系统配置-查询
	static const string HTTP_CMD_EDIT_DEV_SYSCFG("editDevSysCfg");//设备系统配置-修改
	static const string HTTP_CMD_EDIT_DEV_PARA("editDevPara");//修改设备主动上报参数
	static const string HTTP_CMD_CREATE_AREA("createArea");//创建区域
	static const string HTTP_CMD_EDIT_AREA("editArea");//修改区域
	static const string HTTP_CMD_DEL_AREA("delArea");//删除区域
	static const string HTTP_CMD_REMOVE_CHL_FROM_TREE("removeChlFromTree");//移除监控点
	static const string HTTP_CMD_EDIT_CHL_AREA("editChlArea");//修改监控点所属区域
	static const string HTTP_CMD_EDIT_AREA_CHL("editAreaChl");//修改区域下的监控点
	static const string HTTP_CMD_QUERY_CHL_STREAM("queryChlStream");//监控点远程配置--查询监控点图像质量
	static const string HTTP_CMD_EDIT_CHL_STREAM("editChlStream");//监控点远程配置--修改监控点图像质量
	static const string HTTP_CMD_QUERY_CHL_REC_CFG("queryChlRecCfg");//监控点远程/系统配置--查询监控点录像配置
	static const string HTTP_CMD_EDIT_CHL_REC_CFG("editChlRecCfg");//监控点远程/系统配置--修改监控点录像配置
	static const string HTTP_CMD_QUERY_CHL_MOTION_AREA("queryChlMotionArea");//监控点远程配置--查询监控点移动报警区域配置
	static const string HTTP_CMD_EDIT_CHL_MOTION_AREA("editChlMotionArea");//监控点远程配置--修改监控点移动报警区域配置
	static const string HTTP_CMD_QUERY_CHL_MOTION_TRIGGER("queryChlMotionTrigger");//监控点远程配置--查询监控点移动报警联动配置
	static const string HTTP_CMD_EDIT_CHL_MOTION_TRIGGER("editChlMotionTrigger");//监控点远程配置--修改监控点移动报警联动配置
	static const string HTTP_CMD_QUERY_CHL_VIDEO_LOSS_TRIGGER("queryChlVideoLossTrigger");//监控点远程配置--查询监控点视频丢失报警
	static const string HTTP_CMD_EDIT_CHL_VIDEO_LOSS_TRIGGER("editChlVideoLossTrigger");//监控点远程配置--修改监控点视频丢失报警

	//监控点云台协议配置
	static const string HTTP_CMD_QUERY_CHL_PTZ_PROTOCOL("queryPtzProtocol");//-查询监控点云台协议配置
	static const string HTTP_CMD_EDIT_CHL_PTZ_PROTOCOL("editPtzProtocol");//-修改监控点云台协议配置 

	//预置点管理
	static const string HTTP_CMD_QUERY_CHL_PRESET_LIST("queryChlPresetList");//--查询预置点列表
	static const string HTTP_CMD_CREATE_CHL_PRESET("createChlPreset");//--添加预置点
	static const string HTTP_CMD_EDIT_CHL_PRESET("editChlPreset");//--修改预置点名称
	static const string HTTP_CMD_EDIT_CHL_PRESET_POSITION("editChlPresetPosition");//--修改预置点位置
	static const string HTTP_CMD_DEL_CHL_PRESET("delChlPreset");// --删除预置点(N9K)
	static const string HTTP_CMD_DEL_IPC_CHL_PRESET("PtzDeletePreset");// --删除预置点(TVT IPC)
	//监控点
	static const string HTTP_CMD_QUERY_CHL_CRUISE_LIST("queryChlCruiseList");//监控点--查询巡航线列表
	static const string HTTP_CMD_QUERY_CHL_CRUISE("queryChlCruise");//监控点--查询单个巡航线的预置点信息 
	static const string HTTP_CMD_CREATE_CHL_CRUISE("createChlCruise");//监控点--新增巡航线
	static const string HTTP_CMD_EDIT_CHL_CRUISE("editChlCruise");//监控点--修改巡航线
	static const string HTTP_CMD_DEL_CHL_CRUISE("delChlCruise");//监控点--删除巡航线

	//排程相关命令
	static const string HTTP_CMD_QUERY_SCHEDULE_LIST("queryScheduleList");//请求排程列表--排程管理
	static const string HTTP_CMD_CREATE_SCHEDULE("createSchedule");//创建排程--排程管理
	static const string HTTP_CMD_QUERY_SCHEDULE_INFO("querySchedule");//请求排程信息--排程管理
	static const string HTTP_CMD_EDIT_SCHEDULE("editSchedule");//修改排程--排程管理
	static const string HTTP_CMD_DEL_SCHEDULE("delSchedule");//删除排程--排程管理

	//时间段帧率模板相关命令
	static const string HTTP_CMD_CREATE_TIMESECRATE("createTimeSecRate");
	static const string HTTP_CMD_EDIT_TIMESECRATE("editTimeSecRate");
	static const string HTTP_CMD_DEL_TIMESECRATE("delTimeSecRate");

	/************************************************************************/
	/* 设备相关                                                                     */
	/************************************************************************/ 
	/// 设备管理相关命令 	  N9000需要使用的 N5000V2.0也是用的这个
	static const string HTTP_CMD_CREATE_DEV_LIST("createDevList");// 创建设备  List
	static const string HTTP_CMD_QUERY_DEV_LIST("queryDevList");  // 请求设备列表  List  
	static const string HTTP_CMD_QUERY_DEV("queryDev");// 请求设备 
	static const string HTTP_CMD_DEL_DEV_LIST("delDevList");// 删除设备	 List
	static const string HTTP_CMD_EDIT_DEV("editDev");			//编辑设备  
	static const string HTTP_CMD_EDIT_DEV_LIST("editDevList");	//编辑设备 List	

	//查询支持的接入协议
	static const string HTTP_CMD_QUERY_SUPPORT_DEV_PROTOCOLS("querySupportDevProtocols");

	//通道相关
	static const string HTTP_CMD_QUERY_CHANNELINFO("queryChannelInfo");//请求通道信息
	static const string HTTP_CMD_EDIT_CHANNELINFO("editChannelInfo");//请求通道信息

	//解码器
	static const string HTTP_CMD_CREATE_DECODER_LIST("createDecoderList");//
	static const string HTTP_CMD_DEL_DECODER_LIST("delDecoderList");
	static const string HTTP_CMD_QUERY_DECODER_LIST("queryDecoderList");//
	static const string HTTP_CMD_QUERY_DECODER("queryDecoder");//
	static const string HTTP_CMD_EDIT_DECODER("editDecoder");//
	static const string HTTP_CMD_EDIT_DECODER_LIST("editDecoderList");//

	//报警主机
	static const string HTTP_CMD_CREATE_ALARMHOST_LIST("createAlarmHostList");
	static const string HTTP_CMD_QUERY_ALARMHOST_LIST("queryAlarmHostList");
	static const string HTTP_CMD_QUERY_ALARMHOST("queryAlarmHost");
	static const string HTTP_CMD_DEL_ALARMHOST_LIST("delAlarmHostList");
	static const string HTTP_CMD_EDIT_ALARMHOST("editAlarmHost");
	static const string HTTP_CMD_EDIT_ALARMHOST_LIST("editAlarmHostList");

	//报警主机子系统
	static const string HTTP_CMD_QUERY_SUBSYSTEM_LIST("querySubSystemList");
	static const string HTTP_CMD_EDIT_SUBSYSTEM_LIST("editSubSystemList");
	static const string HTTP_CMD_QUERY_SUBSYSTEM("querySubSystem");
	static const string HTTP_CMD_EDIT_SUBSYSTEM("editSubSystem");

	//报警主机防区
	static const string HTTP_CMD_QUERY_ZONE_LIST("queryZoneList");
	static const string HTTP_CMD_EDIT_ZONE_LIST("editZoneList");
	static const string HTTP_CMD_QUERY_ZONE("queryZone");
	static const string HTTP_CMD_EDIT_ZONE("editZone");

	//访客
	static const string HTTP_CMD_CREATE_FK_LIST("createFkList");
	static const string HTTP_CMD_EDIT_FK_LIST("editFkList");
	static const string HTTP_CMD_QUERY_FK_LIST("queryFkList");
	static const string HTTP_CMD_DEL_FK_LIST("delFkList");

	//门禁系统
	static const string HTTP_CMD_CREATE_ACSSYSTEM_LIST("createAcsSystemList");
	static const string HTTP_CMD_QUERY_ACSSYSTEM_LIST("queryAcsSystemList");
	static const string HTTP_CMD_DEL_ACSSYSTEM_LIST("delAcsSystemList");
	static const string HTTP_CMD_EDIT_ACSSYSTEM_LIST("editAcsSystemList");

	//门禁设备
	static const string HTTP_CMD_CREATE_ACSDEVICE_LIST("createAcsDeviceList");
	static const string HTTP_CMD_QUERY_ACSDEVICE_LIST("queryAcsDeviceList");
	static const string HTTP_CMD_DEL_ACSDEVICE_LIST("delAcsDeviceList");

	//门
	static const string HTTP_CMD_CREATE_ACSDOOR_LIST("createAcsDoorList");
	static const string HTTP_CMD_QUERY_ACSDOOR_LIST("queryAcsDoorList");
	static const string HTTP_CMD_DEL_ACSDOOR_LIST("delAcsDoorList");

	//读头
	static const string HTTP_CMD_CREATE_ACSREADER_LIST("createAcsReaderList");
	static const string HTTP_CMD_QUERY_ACSREADER_LIST("queryAcsReaderList");
	static const string HTTP_CMD_DEL_ACSREADER_LIST("delAcsReaderList");

	//设备（DVR，IPC，N9100，N9000，N1000统称为设备）
	//系统维护命令 
	static const string HTTP_CMD_REBOOT("reboot");//重启
	static const string HTTP_CMD_RESTORE_DEFAULTS("restoreDefaults");//恢复默认配置
	static const string HTTP_CMD_IMPORT_CONFIG("importConfig");//导入配置
	static const string HTTP_CMD_EXPORT_CONFIG("exportConfig");//导出配置
	static const string HTTP_CMD_UPGRADE("upgrade");//升级
	static const string HTTP_CMD_CONNECT_TEST("httpConnectionTest");

    //设备
    static const string HTTP_CMD_QUERY_SYSTEMCAPS("querySystemCaps");//查询系统能力集 
    static const string HTTP_CMD_QUERY_BASICCFG("queryBasicCfg");//查询基本信息   
    static const string HTTP_CMD_EDIT_BASICCFG("editBasicCfg");//修改基本信息   
    static const string HTTP_CMD_QUERY_TIMECFG("queryTimeCfg");//查询时间配置   
    static const string HTTP_CMD_EDIT_TIMECFG("editTimeCfg");//修改时间配置   
    static const string HTTP_CMD_QUERY_LOG("queryLog");//查询日志    
    static const string HTTP_CMD_EXPORT_LOG("exportLog");//导出日志  
	static const string HTTP_CMD_QUERY_CONTENT_BY_LOGKEY("queryContentByLogKey");//查询sop日志    
	static const string HTTP_CMD_EDIT_CONTENT_BY_LOGKEY("editContentByLogKey");//导出sop日志  
	static const string HTTP_CMD_CLOSE_CONTENT_BY_LOGKEY("closeContentByLogKey");//关闭sop日志  
	static const string HTTP_CMD_QUERY_LOG_EVENT_DICTIONARY("queryLogEventDictionary");
    //static const string HTTP_CMD_QUERY_DISKINFO("queryDiskInfo");//查询设备磁盘信息  
	static const string HTTP_CMD_QUERY_PERSONNAL_GROUP_LIST("queryFacePersonnalInfoGroupList");//查询人脸特征分组  
	static const string HTTP_CMD_QUERY_PERSONNAL_LIST("queryFacePersonnalInfoList");//查询人脸特征
	static const string HTTP_CMD_REQUEST_PERSONNAL_IMAGE("requestFacePersonnalInfoImage");//请求人脸特征图片
	static const string HTTP_CMD_QUERY_CREATE_FACE_GROUP("createFacePersonnalInfoGroup");//创建目标库
	static const string HTTP_CMD_QUERY_EDIT_FACE_GROUP("editFacePersonnalInfoGroup");//编辑目标库
	static const string HTTP_CMD_QUERY_DEL_FACE_GROUP("delFacePersonnalInfoGroups");//删除目标库
	static const string HTTP_CMD_QUERY_DEL_FACE("delFacePersonnalInfo");//删除目标
	static const string HTTP_CMD_QUERY_CH_FACE_IMAGELIST("queryChSnapFaceImageList");//查询人脸图片
	static const string HTTP_CMD_REQUEST_CH_FACE_IMAGE("requestChSnapFaceImage");//请求人脸图片
	static const string HTTP_CMD_CREATE_PERSONNAL_INFO("createFacePersonnalInfo");//创建人脸特征
	static const string HTTP_CMD_EDIT_PERSONNAL_INFO("editFacePersonnalInfo");//编辑人脸特征
	static const string HTTP_CMD_SEARCH_IMAGE_BY_IMAGE("searchImageByImage");//以图搜图
	static const string HTTP_CMD_QUERY_FACE_MATCH_CFG("queryFaceMatchAlarm");//人脸比对报警配置查询
	static const string HTTP_CMD_EDIT_FACE_MATCH_CFG("editFaceMatchAlarm");//修改人脸比对报警配置
	static const string HTTP_CMD_FACE_QUERY_VFD("queryVfd");//查询人脸侦测
	static const string HTTP_CMD_FACE_EDIT_VFD("editVfd");//修改人脸侦测
	static const string HTTP_CMD_FACE_QUERY_BACK_VFD("queryBackFaceMatch");//查询人脸后侦测
	static const string HTTP_CMD_FACE_EDIT_BACK_VFD("editRealFaceMatch");//修改人脸后侦测
	static const string HTTP_CMD_QUERY_AOI_ENTRY("GetAOIEntryConfig");//进入区域配置查询
	static const string HTTP_CMD_EDIT_AOI_ENTRY("SetAOIEntryConfig");//进入区域配置修改
	static const string HTTP_CMD_QUERY_AOI_LEAVE("GetAOILeaveConfig");//离开区域配置查询
	static const string HTTP_CMD_EDIT_AOI_LEAVE("SetAOILeaveConfig");//离开区域配置修改
	static const string HTTP_CMD_QUERY_SMART_AOI_ENTRY("querySmartAOIEntryConfig");
	static const string HTTP_CMD_EDIT_SMART_AOI_ENTRY("editSmartAOIEntryConfig");
	static const string HTTP_CMD_QUERY_SMART_AOI_LEAVE("querySmartAOILeaveConfig");
	static const string HTTP_CMD_EDIT_SMART_AOI_LEAVE("editSmartAOILeaveConfig");
    static const string HTTP_CMD_QUERY_YITU_ALGORITHM_CONFIG("GetYiTuBoxFaceSetting");//依图盒子算法设置
    static const string HTTP_CMD_EDIT_YITU_ALGORITHM_CONFIG("SetYiTuBoxFaceSetting");
	static const string HTTP_CMD_QUERY_TEMPERATURE_CONFIG("GetTemperatureConfig");//获取热成像
	static const string HTTP_CMD_EDIT_TEMPERATURE_CONFIG("SetTemperatureConfig");
	static const string HTTP_CMD_QUERY_WEARMASK_CONFIG("GetWearMaskConfig");//口罩
	static const string HTTP_CMD_EDIT_WEARMASK_CONFIG("SetWearMaskConfig");
    static const string HTTP_CMD_QUERY_THERMAL_CORRECTION_CONFIG("GetTemperatureCalibrationConfig");//温度校正
    static const string HTTP_CMD_EDIT_THERMAL_CORRECTION_CONFIG("SetTemperatureCalibrationConfig");
    static const string HTTP_CMD_DEAL_TEMP_CALIBRATION("DealTemperatureCalibration");// 黑体校正
    static const string HTTP_CMD_QUERY_DOT_TEMP("GetDotTemperature");// 获取点温度

    static const string HTTP_CMD_QUERY_THERMAL_CALIBRATION_CONFIG("GetThermalCalibrationConfig");//工业测温校正设置
    static const string HTTP_CMD_EDIT_THERMAL_CALIBRATION_CONFIG("SetThermalCalibrationConfig");
    static const string HTTP_CMD_QUERY_THERMAL_ALARM_CONFIG("GetThermalAlarmConfig");//工业测温报警设置
    static const string HTTP_CMD_EDIT_THERMAL_ALARM_CONFIG("SetThermalAlarmConfig");
	static const string HTTP_CMD_QUERY_THERMAL_EARLY_ALARM_CONFIG("GetThermalEarlyAlarmConfig");//工业测温报警设置
    static const string HTTP_CMD_EDIT_THERMAL_EARLY_ALARM_CONFIG("SetThermalEarlyAlarmConfig");
    static const string HTTP_CMD_ADD_FACE("AddTargetFace");// 添加人脸
    static const string HTTP_CMD_DEL_FACE("DeleteTargetFace");// 删除人脸
    static const string HTTP_CMD_MODIFY_FACE("EditTargetFace");// 编辑人脸
    static const string HTTP_CMD_QUERY_FACE("GetTargetFace");// 获取人脸
    static const string HTTP_CMD_UNLOCKING_BY_ADMIN("UnLockingByAdmin");//开锁
	static const string HTTP_CMD_QUERY_SENSORLIST_CONFIG("querySensorList");//查询N9K sensor列表
	static const string HTTP_CMD_QUERY_SENSORSCHEDULELIST_CONFIG("querySensorScheduleList");//查询N9K sensor排程列表
	static const string HTTP_CMD_QUERY_SENSOR_CONFIG("querySensor");//查询N9K sensor参数
	static const string HTTP_CMD_EDIT_SENSOR_CONFIG("editSensor");//设置N9K sensor参数
	static const string HTTP_CMD_QUERY_CHANNELLIST_CONFIG("queryChannelList");//查询通道列表
	static const string HTTP_CMD_QUERY_ALARMOUTLIST_CONFIG("queryAlarmOutList");//查询alarm_out列表
	static const string HTTP_CMD_QUERY_SCHEDULEINFO_CONFIG("queryScheduleInfo");//查询排程信息
	static const string HTTP_CMD_ADD_SCHEDULEINFO_CONFIG("addScheduleInfo");//新增排程信息
	static const string HTTP_CMD_EDIT_SCHEDULEINFO_CONFIG("editScheduleInfo");//设置排程信息
	static const string HTTP_CMD_DEL_SCHEDULEINFO_CONFIG("delScheduleInfo");//删除排程信息
	static const string HTTP_CMD_GET_HEATMAP_CONFIG("GetHeatMapConfig");//热地图
	static const string HTTP_CMD_SET_HEATMAP_CONFIG("SetHeatMapConfig");//
	static const string HTTP_CMD_GET_REGIONSTATISTIC_CONFIG("GetTrafficConfig");//区域统计
	static const string HTTP_CMD_SET_REGIONSTATISTIC_CONFIG("SetTrafficConfig");//

	//网络
    static const string HTTP_CMD_QUERY_NETSTATUS("queryNetStatus");//查询网络状态 
    static const string HTTP_CMD_QUERY_NETCFG("queryNetCfg");//查询网络配置  
    static const string HTTP_CMD_EDIT_NETCFG("editNetCfg");//修改网络配置 
	static const string HTTP_CMD_QUERY_NET_PROT_CFG("queryNetPortCfg");//查询网络端口配置
	static const string HTTP_CMD_EDIT_NET_PROT_CFG("editNetPortCfg");//修改网络端口配置  
    static const string HTTP_CMD_QUERY_DDNS("queryDDNSCfg");//查询DDNS配置  
    static const string HTTP_CMD_EDIT_DDNS("editDDNSCfg");//修改DDNS配置  如果是网络上需要转发的节点需要带目标节点GUID
	static const string HTTP_CMD_QUERY_EMAILCFG("queryEmailCfg");//查询Email配置 
	static const string HTTP_CMD_EDIT_EMAILCFG("editEmailCfg");//修改Email配置 
	static const string HTTP_CMD_QUERY_UPNP("queryUPnPCfg");//查询UPnP配置
	static const string HTTP_CMD_EDIT_UPNP("editUPnPCfg");//修改UPnP配置 
	static const string HTTP_CMD_QUERY_FTP("queryFTPCfg");//查询FTP配置 
	static const string HTTP_CMD_EDIT_FTP("editFTPCfg");//修改FTP配置 	
	static const string HTTP_CMD_QUERY_PPPOE("queryPPPoECfg");//查询PPPOE配置 
	static const string HTTP_CMD_EDIT_PPPOE("editPPPoECfg");//修改PPPOE配置 
	static const string HTTP_CMD_QUERY_SERINALNET("QueryTranServerConfig");//查询透明串口配置  
	static const string HTTP_CMD_EDIT_SERINALNET("EditTranServerConfig");//修改透明串口配置  
	//黑白名单的配置
	static const string HTTP_CMD_QUERY_BLACK_AND_WHITE_LIST("queryBlackAndWhiteList");//查询黑白名单的配置 
	static const string HTTP_CMD_EDIT_BLACK_AND_WHITE_LIST("editBlackAndWhiteList");//修改黑白名单的配置  

	//报警管理
	static const string HTTP_CMD_QUERY_ALARM_OUT_PARAM("queryAlarmOutParam");//报警管理--查询报警输出参数
	static const string HTTP_CMD_EDIT_ALARM_OUT_PARAM("editAlarmOutParam");//报警管理--编辑报警输出的参数
	static const string HTTP_CMD_QUERY_ALARM_IN_TRIGGER("queryAlarmIn");//报警管理--查询报警输入的信息
	static const string HTTP_CMD_EDIT_ALARM_IN("editAlarmIn");//报警管理--编辑报警输入的信息
	static const string HTTP_CMD_QUERY_ABNORMAL_TRIGGER("queryAbnormalTrigger");//报警管理--查询系统异常的联动信息
	static const string HTTP_CMD_EDIT_ABNORMAL_TRIGGER("editAbnormalTrigger");//报警管理--编辑系统异常的联动信息
	//传感器报警输出
	static const string HTTP_CMD_QUERY_CHANNEL_LIST("queryChannelList");//传感器报警输出--查询channel列表信息
	static const string HTTP_CMD_QUERY_ALARMOUT_LIST("queryAlarmOutList");//传感器报警输出--查询alarmout列表信息
	static const string HTTP_CMD_QUERY_SENSOR_LIST("querySensorList");//传感器报警输出--查询sensor列表信息
	static const string HTTP_CMD_QUERY_SENSOR_SCHEDULE_LIST("querySensorScheduleList");//传感器报警输出--查询排程列表信息
	static const string HTTP_CMD_QUERY_SENSOR_PARAM("querySensor");//传感器报警输出--查询sensor信息
	static const string HTTP_CMD_EDIT_SENSOR_PARAM("editSensor");//传感器报警输出--设置sensor信息
	static const string HTTP_CMD_QUERY_SENSOR_SCHEDULE_PARAM("queryScheduleInfo");//传感器报警输出--查询排程信息
	//排程设置
	static const string HTTP_CMD_ADD_SCHEDULE("addScheduleInfo");//排程--新增排程信息
	static const string HTTP_CMD_EDIT_SCHEDULEINFO("editScheduleInfo");//排程--设置排程信息
	static const string HTTP_CMD_DELETE_SCHEDULE("delScheduleInfo");//排程--删除排程信息
	//报警管理----联动
    static const string HTTP_CMD_QUERY_VIDEO_LOSS_TRIGGER("queryVideoLossTrigger");//报警管理--查询视频丢失的联动信息
    static const string HTTP_CMD_EDIT_VIDEO_LOSS_TRIGGER("editVideoLossTrigger");//报警管理--编辑视频丢失的联动信息
	static const string HTTP_CMD_SEARCH_PICTURES_DATA("searchPictures");//查询NVR图片
	//搜索车牌
	static const string HTTP_CMD_SEARCH_SMART_TARGET("searchSmartTarget");
	static const string HTTP_CMD_SEARCH_SMART_TARGET_IMAGE("requestSmartTargetSnapImage");

	//前端掉线
	static const string HTTP_CMD_QUERY_FRONT_END_OFFLINE_TRIGGER("queryFrontEndOfflineTrigger");
	static const string HTTP_CMD_EDIT_FRONT_END_OFFLINE_TRIGGER("editFrontEndOfflineTrigger");

	//人脸侦测
	static const string HTTP_CMD_QUERY_VFD("queryVfd");
	static const string HTTP_CMD_EDIT_VFD("editVfd");

	//人数统计
	static const string HTTP_CMD_QUERY_CPC("queryCpc");
	static const string HTTP_CMD_EDIT_CPC("editCpc");

	//人群密度
	static const string HTTP_CMD_QUERY_CDD("queryCdd");
	static const string HTTP_CMD_EDIT_CDD("editCdd");

	//人员入侵
	static const string HTTP_CMD_QUERY_IPD("queryIpd");
	static const string HTTP_CMD_EDIT_IPD("editIpd");

	//人脸比对
	static const string HTTP_CMD_QUERY_FACE_MATCH("queryFaceMatchAlarmParam");
	static const string HTTP_CMD_EDIT_FACE_MATCH("editFaceMatchAlarmParam");
	static const string HTTP_CMD_QUERY_FACE_MATCH_CONFIG("queryFaceMatchConfig");
	static const string HTTP_CMD_EDIT_FACE_MATCH_CONFIG("editFaceMatchConfig");
	static const string HTTP_CMD_QUERY_CHL_FACE_MATCH_CFG("queryChlFaceMatchConfig");
	static const string HTTP_CMD_EDIT_CHL_FACE_MATCH_CFG("editChlFaceMatchConfig");

	//录像模式
	static const string HTTP_CMD_QUERY_DEVREC_PARAM("queryDevRecParam");//设备录像模式及通道排程配置
	static const string HTTP_CMD_EDIT_DEVREC_PARAM("editDevRecParam");//设备录像模式及通道排程配置
	static const string HTTP_CMD_QUERY_RECORD_MODE("queryRecordDistributeInfo");
	static const string HTTP_CMD_EDIT_RECORD_MODE("editRecordDistributeInfo");//修改录像模式

	//与本地系统交互命令
	static const char SHAKE_HANDS_CHECKSUM_STRING[] = {"{D79E94C5-70F0-46BD-965B-E17497CCB598}"};//与本地系统握手校验字符串
	static const string HTTP_CMD_REQUEST_SYS_CFG("requestSystemConfig");//请求本地系统配置信息
	static const string HTTP_CMD_SAVE_SYS_CFG("saveSystemConfig");//保存本地系统配置信息
	static const string HTTP_CMD_REQUEST_NODE_STATE("requestNodeState");//配置服务向管理服务器请求节点状态//add
	//static const string HTTP_CMD_REQUEST_SYSTEM_INFO("requestSystemInfo");//请求系统信息,如系统类型,启动时向系统请求
	static const string HTTP_CMD_REQUEST_SYSTEM_CAPABILITY_SET_INFO("requestSystemCapabilitySetInfo");//系统能力集信息
	static const string HTTP_CMD_REQUEST_SYSTEM_ENABLE_HTTPS("enableHttps");//启动https
	static const string HTTP_CMD_REQUEST_SYSTEM_DISABLE_HTTPS("disableHttps");//关闭https
	//本地系统主动上报相关命令
	static const string HTTP_CMD_REPORT_SYS_MSU_PARTITION_STATE("reportSysMsuPartState");//本地系统主动上报存储服务器的分区信息
	static const string HTTP_CMD_REPORT_SYS_MSU_PARTITION_GROUP("reportSysMsuPartGroup");//本地系统主动上报默认分区组信息
	static const string HTTP_CMD_REPORT_DEVICE_APPEND_INFO("reportDeviceAppendInfo");//本地系统主动上报设备附加信息更新，同步通道数量，输入和输出数量
	static const string HTTP_CMD_REPORT_CHANNEL_NAME("reportChannelName");//本地系统主动上报通道名称
	static const string HTTP_CMD_REPORT_SENSOR_NAME("reportSensorName");//本地系统主动上报Sensor名称
	static const string HTTP_CMD_REPORT_ALARMOUT_NAME("reportAlarmOutName");//本地系统主动上报AlarmOut名称
	static const string HTTP_CMD_REPORT_DEVICE_NAME("reportDeviceName");//本地系统主动上报设备名称
	static const string HTTP_CMD_REPORT_CHANNEL_APPEND_INFO("reportChannelAppendInfo");//本地系统主动上报通道附加信息更新
	static const string HTTP_CMD_REPORT_SYSTEM_CAPABILITY_SET_INFO("reportSystemCapabilitySetInfo");//本地系统主动上报系统能力集

	//GB28181
	static const string HTTP_CMD_REPORT_DEVICE_GB28181INFO("reportDeviceGb28181Info");//本地系统主动上报设备国标ID
	static const string HTTP_CMD_REPORT_CHANNEL_GB28181INFO("reportChannelGb28181Info");//本地系统主动上报通道国标ID
	//GAT1400
	static const string HTTP_CMD_REPORT_CHANNEL_GAT1400INFO("reportChannelGAT1400Info");//本地系统主动上报通道GAT1400id
	//通知配置服务器与管理同步内存数据
	static const string HTTP_CMD_REPORT_SYS_REFRESH_CFG("reportSysRefreshCfg");
	static const string HTTP_CMD_REPORT_ACS_DEVICE_INFO("reportAcsDeviceInfo");
	static const string HTTP_CMD_REPORT_ACS_DOOR_INFO("reportAcsDoorInfo");
	static const string HTTP_CMD_REPORT_ACS_READER_INFO("reportAcsReaderInfo");

	//通知目标库列表
	static const string HTTP_CMD_REPORT_ALBUM_INFO("reportAlbumInfo");//智能上报给管理，管理上报给配置目标库
	static const string HTTP_CMD_QUERY_ALBUM_INFO("queryAlbumInfo");//配置找管理，管理找智能请求目标库

	//IP通道  AND  通道
	static const string HTTP_CMD_CREATE_CHL_LIST("createChlList");//创建通道列表
	static const string HTTP_CMD_EDIT_CHL_NAME("editChlName");//编辑通道名称
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_MOTION("queryMotion");//IP通道或通道的移动侦测
	//static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_MOTION_LIST("queryIPChlORChlMotionList");//IP通道或通道的移动侦测
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_MOTION("editMotion");//IP通道或通道的移动侦测
	//static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_MOTION_LIST("editIPChlORChlMotionList");//IP通道或通道的移动侦测

	//通道监控点报警联动N5000相关
	//通道的区域入侵联动查询和修改
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_PERIMETER("queryPerimeter");
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_PERIMETER("editPerimeter");

	//通道的区域越界联动查询和修改
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_TRIPWIRE("queryTripwire");
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_TRIPWIRE("editTripwire");

	//通道的物品看护联动查询和修改
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_OSC("queryOsc");
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_OSC("editOsc");

	//通道的视频异常联动查询和修改
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_AVD("queryAvd");
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_AVD("editAvd");

	//报警管理
	static const string HTTP_CMD_COPY_ALARM_TRIGGER("copyAlarmTrigger");//报警管理批复制
	static const string HTTP_CMD_BATCH_COPY_ALARM_TRIGGER("batchCopyAlarmTrigger");//报警管理批复制通用版

	//通道----移动侦测
	static const string HTTP_CMD_QUERY_MOTION_TRIGGER("queryMotionTrigger");//通道--查询移动侦测的联动信息
	static const string HTTP_CMD_EDIT_MOTION_TRIGGER("editMotionTrigger");//通道--编辑移动侦测的联动信息

	//IP通道或通道的录像排程
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_REC_SCHEDULE("queryRecordSchedule");//IP通道或通道的录像排程
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_REC_SCHEDULE_LIST("queryRecordScheduleList");//IP通道或通道的录像排程
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_REC_SCHEDULE("editRecordSchedule");//IP通道或通道的录像排程
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_REC_SCHEDULE_LIST("editRecordScheduleList");//IP通道或通道的录像排程

	//监控点配置
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_OSD("queryIPChlORChlOSD");//监控点配置--查询监控点OSD信息
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_OSD("editIPChlORChlOSD");//监控点配置--修改监控点OSD信息

	//监控点远程配置
    static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_VIDEO_PARAM("queryChlVideoParam");//监控点远程配置--查询监控点前端视频参数
    static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_VIDEO_PARAM("editChlVideoParam");//监控点远程配置--修改监控点前端视频参数

	//镜头控制
    static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_CAMERA_LENS_CTRL_PARAM("queryCameraLensCtrlParam");//镜头控制
    static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_CAMERA_LENS_CTRL_PARAM("editCameraLensCtrlParam");//

	//
	static const string HTTP_CMD_QUERY_EVENT_RECORDCFG("queryEventRecordCfg");//
	static const string HTTP_CMD_EDIT_EVENT_RECORDCFG("editEventRecordCfg");//

	//IP通道或通道的视频遮挡
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_PRIVACY_MASK("queryPrivacyMask");//查询IP通道或通道的视频遮挡
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_PRIVACY_MASK("editPrivacyMask");//修改IP通道或通道的视频遮挡

	//IP通道或通道的Roi
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_ROI("queryRoiConfig");//查询IP通道或通道的Roi
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_ROI("editRoiConfig");//修改IP通道或通道的Roi

	//视频流编码参数信息
    static const string HTTP_CMD_QUERY_CHL_VIDEO_STREAM_CFG("GetVideoStreamConfig");//获取视频流编码参数信息
    static const string HTTP_CMD_EDIT_CHL_VIDEO_STREAM_CFG("SetVideoStreamConfig");//设置视频流编码参数信息

	//磁盘和通道存储
	static const string HTTP_CMD_QUERY_RAID_CFG("queryRaidCfg");
	static const string HTTP_CMD_CREATE_RAID_CFG("createRaid");//创建RAID
	static const string HTTP_CMD_DELETE_RAID_CFG("delRaid");//删除RAID
	static const string HTTP_CMD_EXPAND_RAID_CFG("expandRaid");//扩容
	static const string HTTP_CMD_SET_TO_SPARE("setToSpare");//创建热备盘
	static const string HTTP_CMD_SET_TO_FREE_DISK("setToFreeDisk");//删除热备盘
	static const string HTTP_CMD_QUERY_STORAGE_DEV_INFO("queryStorageDevInfo");//
	static const string HTTP_CMD_QUERY_DISK_DETAIL_INFO("queryDiskDetailInfo");//
	static const string HTTP_CMD_QUERY_RAID_STATISTICS("queryRaidStatistics");//
	static const string HTTP_CMD_QUERY_STORAGE_DEV_STATISTICS("queryStorageDevStatistics");//
	static const string HTTP_CMD_QUERY_RAID_STATUS("queryRaidStatus");//
	static const string HTTP_CMD_QUERY_DISK_STATUS("queryDiskStatus");//
 	static const string HTTP_CMD_QUERY_DISK_GROUP_LIST("queryDiskGroupList");//
	static const string HTTP_CMD_QUERY_LOGICAL_DISK_LIST("queryLogicalDiskList");//
	static const string HTTP_CMD_QUERY_DISK_SMART_INFO("queryDiskSmartInfo");//获取硬盘的SMART信息

	//new add  20190716 by zcy
	static const string HTTP_CMD_FORMAT_DISK("formatDisk");//格式化磁盘  
	static const string HTTP_CMD_QUERY_EXTERNAL_DISK_INFO("queryExternalDisks");//查询设备接入的外接磁盘
	static const string HTTP_CMD_CREATE_REC_BACKUP_TASK("createRecBackupTask");//创建录像备份任务
	static const string HTTP_CMD_QUERY_REC_BACKUP_TASK_LIST("queryRecBackupTaskList");//查询录像备份任务列表
	static const string HTTP_CMD_QUERY_CTRL_REC_BACKUP_TASK("ctrlRecBackupTask");//操作录像备份任务

	static const string HTTP_CMD_QUERY_MANUAL_RECORD("queryManualRecord");	//查询手动录像配置
	static const string HTTP_CMD_EDIT_MANUAL_RECORD("editManualRecord");	//修改手动录像配置




	//////编辑元素和集合的关系
	static const string HTTP_CMD_EDIT_SET_AND_ELEMENT_RELATION("editSetAndElementRelation");  

	//查询状态
	static const string HTTP_CMD_QUERY_VIDEO_LINKS("queryVideoLinks");
	static const string HTTP_CMD_QUERY_RECORD_STATUS("queryRecStatus");
	static const string HTTP_CMD_QUERY_LAN_FREE_DEVICE_LIST("queryLanFreeDeviceList");
	static const string HTTP_CMD_QUERY_CHANNELS_STATUS("queryChlStatus");
 	static const string HTTP_CMD_QUERY_ALARM_STATUS("queryAlarmStatus");
	static const string HTTP_CMD_QUERY_LAN_RECORDER_LIST("queryLanRecorderList");//查询局域网的录像机列表
	static const string HTTP_CMD_QUERY_LAN_FREE_DECORDER_LIST("queryLanFreeDecoderList");

	//
	static const string HTTP_CMD_QUERY_DATES_EXIST_REC("queryDatesExistRec");
	static const string HTTP_CMD_QUERY_CHLS_EXIST_REC("queryChlsExistRec");

	//查询通道录像日志
    static const string HTTP_CMD_QUERY_CHL_REC_LOG("queryChlRecLog");

	//抓图
    static const string HTTP_CMD_SNAP_CHL_PICTURE("snapChlPicture");

	//手动触发报警
    static const string HTTP_CMD_MANUAL_ALARM_OUT("manualAlarmOut");

	//设置手动触发报警
	static const string HTTP_CMD_SET_ALARM_OUT_STATUS("setAlarmOutStatus");

	//语言
	static const string HTTP_CMD_QUERY_LANG_CONTENT("getLangContent");
	static const string HTTP_CMD_QUERY_SUPPORT_LANG_LIST("getSupportLangList");

	//监控点配置
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_DEVICEINFO("GetDeviceInfo");//监控点配置--查询设备信息
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_TRACES("GetTraces");//监控点配置--查询轨迹
	static const string HTTP_CMD_DELETE_IPCHL_OR_CHL_TRACE("DeleteTrace");//监控点配置--删除轨迹
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_OSCCONFIG("GetOscConfig");//查询物品看护配置参数
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_OSCCONFIG("SetOscConfig");//修改物品看护配置参数
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_AVDCONFIG("GetAvdConfig");//查询异常侦测配置参数
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_AVDCONFIG("SetAvdConfig");//修改异常侦测配置参数
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_TRIPWIRECONFIG("GetTripwireConfig");//查询越界侦测配置参数
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_TRIPWIRECONFIG("SetTripwireConfig");//修改越界侦测配置参数
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_PERIMETERCONFIG("GetPerimeterConfig");//查询区域入侵侦测配置参数
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_PERIMETERCONFIG("SetPerimeterConfig");//编辑区域入侵侦测配置参数
	static const string HTTP_CMD_QUERY_INTEL_AREA_CONFIG("queryIntelAreaConfig");//查询区域入侵-NVR
	static const string HTTP_CMD_EDIT_INTEL_AREA_CONFIG("editIntelAreaConfig");//编辑区域入侵-NVR
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_GUARDINFO("GetPtzGuardInfo");//监控点配置--查询看守位
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_GUARDINFO("SetPtzGuardInfo");//监控点配置--修改看守位
	static const string HTTP_CMD_QUERY_IPCHL_OR_CHL_VFDCONFIG("GetVfdConfig");//查询人脸侦测配置参数
	static const string HTTP_CMD_EDIT_IPCHL_OR_CHL_VFDCONFIG("SetVfdConfig");//修改人脸侦测配置参数

	//智能IPC新增 1.7版本以后
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_CDDCONFIG("GetSmartCddConfig");//查询人群密度检测配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_CDDCONFIG("SetSmartCddConfig");//修改人群密度检测配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_IPDCONFIG("GetSmartIpdConfig");//查询人员入侵侦测配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_IPDCONFIG("SetSmartIpdConfig");//修改人员入侵侦测配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_CPCCONFIG("GetSmartPassLineCountConfig");//查询人数统计配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_CPCCONFIG("SetSmartPassLineCountConfig");//修改人数统计配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_CPCCONFIG_old("GetSmartCpcConfig");//查询人数统计配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_CPCCONFIG_old("SetSmartCpcConfig");//修改人数统计配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_VFDCONFIG("GetSmartVfdConfig");//查询人脸识别侦测配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_VFDCONFIG("SetSmartVfdConfig");//编辑人脸识别侦测配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_PERIMETERCONFIG("GetSmartPerimeterConfig");//查询区域入侵侦测配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_PERIMETERCONFIG("SetSmartPerimeterConfig");//编辑区域入侵侦测配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_TRIPWIRECONFIG("GetSmartTripwireConfig");//查询越界侦测配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_TRIPWIRECONFIG("SetSmartTripwireConfig");//编辑越界侦测配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_OSCCONFIG("GetSmartOscConfig");//查询物品看护配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_OSCCONFIG("SetSmartOscConfig");//编辑物品看护配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_AVDCONFIG("GetSmartAvdConfig");//查询异常侦测配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_AVDCONFIG("SetSmartAvdConfig");//编辑异常侦测配置参数
	static const string HTTP_CMD_QUERY_SMART_IPCHL_OR_CHL_VEHICLECONFIG("GetVehicleConfig");//查询车牌配置参数
	static const string HTTP_CMD_EDIT_SMART_IPCHL_OR_CHL_VEHICLECONFIG("SetVehicleConfig");//编辑车牌配置参数
	static const string HTTP_CMD_QUERYNVR_SMART_IPCHL_OR_CHL_VEHICLECONFIG("queryVehicleConfig");//NVR查询车牌配置
	static const string HTTP_CMD_EDIT_SMART_N9KCHL_OR_CHL_VEHICLECONFIG("editVehicleConfig");//NVR查询车牌配置

	static const string HTTP_CMD_QUERY_DEVICE_DETAIL("GetDeviceDetail");//查询设备详细信息

	static const string HTTP_CMD_QUERY_NVR_CHL_CPCCONFIG("queryPls");//查询NVR人数统计配置参数
	static const string HTTP_CMD_EDIT_NVR_CHL_CPCCONFIG("editPls");//修改NVR人数统计配置参数
	//ftp服务器IPC获取设置
	static const string HTTP_CMD_QUERY_FTP_DETAIL("QueryFtpConfig");//查询FTP详细信息
	static const string HTTP_CMD_EDIT_FTP_DETAIL("EditFtpConfig");//设置FTP详细信息

	//通道图像参数设置
	static const string HTTP_CMD_QUERY_IMAGE_SCHEDULE("QueryImageConfigFileInfo");//排程查询
	static const string HTTP_CMD_EDIT_IMAGE_SCHEDULE("EditImageConfigFileInfo");//排程设置

	//警戒IPC新增
	static const string HTTP_CMD_QUERY_DEVICE_ALARM_OUT_AUDIO("GetAudioAlarmOutConfig");//警戒IPC新增， 请求声音告警输出信息
	static const string HTTP_CMD_QUERY_DEVICE_ALARM_OUT_FLASH("GetWhiteLightAlarmOutConfig");//警戒IPC新增， 请求闪光告警输出信息
	static const string HTTP_CMD_SET_DEVICE_ALARM_OUT_AUDIO("SetAudioAlarmOutConfig");//警戒IPC新增， 请求声音告警输出信息
	static const string HTTP_CMD_SET_DEVICE_ALARM_OUT_FLASH("SetWhiteLightAlarmOutConfig");//警戒IPC新增， 请求闪光告警输出信息
	static const string HTTP_CMD_GET_DEVICE_ALARM_TRIGGER_CONFIG("GetAlarmTriggerConfig");//警戒IPC新增， 请求警戒IPC的告警联动设置
	static const string HTTP_CMD_SET_DEVICE_ALARM_TRIGGER_CONFIG("SetAlarmTriggerConfig");//警戒IPC新增， 请求警戒IPC的告警联动设置
	static const string HTTP_CMD_UPLOAD_AUDIO("UpLoadCustomizeAudioAlarm");//警戒IPC新增， 上传音频数据
	static const string HTTP_CMD_LISTEN_AUDIO("TryListenCustomizeAudioAlarm");//警戒IPC新增， 试听音频
	static const string HTTP_CMD_DELETE_AUDIO("ReMoveCustomizeAudioAlarm");//警戒IPC新增， 删除音频


	//鱼眼IPC新增 
	static const string HTTP_CMD_QUERY_FISHEYE_IPCHL_OR_CHL_CONFIG("GetIPChlORChlFishEye");//查询鱼眼配置参数
	static const string HTTP_CMD_EDIT_FISHEYE_IPCHL_OR_CHL_CONFIG("SetIPChlORChlFishEye");//修改鱼眼配置参数

	//监控点远程配置
	static const string HTTP_CMD_QUERY_CHL_OSC_TRIGGER("queryChlOSCTrigger");//监控点远程配置--查询监控点物品看护报警联动配置
	static const string HTTP_CMD_EDIT_CHL_OSC_TRIGGER("editChlOSCTrigger");//监控点远程配置--修改监控点物品看护报警联动配置
	static const string HTTP_CMD_QUERY_CHL_AVD_TRIGGER("queryChlAVDTrigger");//监控点远程配置--查询监控点视频异常报警联动配置
	static const string HTTP_CMD_EDIT_CHL_AVD_TRIGGER("editChlAVDTrigger");//监控点远程配置--修改监控点视频异常报警联动配置
	static const string HTTP_CMD_QUERY_CHL_PEA_TRIPWIRE_TRIGGER("queryChlPeaTripWireTrigger");//监控点远程配置--查询监控点越界报警联动配置
	static const string HTTP_CMD_EDIT_CHL_PEA_TRIPWIRE_TRIGGER("editChlPeaTripWireTrigger");//监控点远程配置--修改监控点越界报警联动配置
	static const string HTTP_CMD_QUERY_CHL_PEA_PERIMETER_TRIGGER("queryChlPeaPerimeterTrigger");//监控点远程配置--查询监控点区域入侵报警联动配置
	static const string HTTP_CMD_EDIT_CHL_PEA_PERIMETER_TRIGGER("editChlPeaPerimeterTrigger");//监控点远程配置--修改监控点区域入侵报警联动配置

	//
	static const string HTTP_CMD_QUERY_RECORD_CFG("GetRecordCfg");//监控点远程配置--
	static const string HTTP_CMD_SET_RECORD_CFG("SetRecordCfg");//监控点远程配置--

    //vr实景
    static const string HTTP_CMD_GET_PTZ_STEP_CONFIG("GetStepInfo");

    /////////////服务器
    /////////////报警输入
    /////////////报警输出
    /////////////排程
    /////////////用户
    /////////////区域
    /////////////磁盘
    /////////////电子地图
    /////////////热区  热点	
    /////////////需要转发的特点描述

    /////////////其他特殊的
    //transportContent
    static const std::string HTTP_CMD_TRANSPORT_CONTENT("transportContent");//

    static const std::string HTTP_CMD_SYNC_NODE_INFO("syncNodeInfo");

	static const std::string HTTP_CMD_QUERY_TREE_DATA("queryTreeData");

	//子系统和防区的报警处理方式
	static const string HTTP_CMD_QUERY_SUBSYSTEM_OR_ZONE_ALARM("querySubSystemORZoneAlarm");
	static const string HTTP_CMD_EDIT_SUBSYSTEM_OR_ZONE_ALARM("editSubSystemORZoneAlarm");

	//门的报警处理方式
	static const string HTTP_CMD_QUERY_ACS_ALARM("queryAcsAlarm");
	static const string HTTP_CMD_EDIT_ACS_ALARM("editAcsAlarm");

	//系统运行状态
	static const string HTTP_CMD_QUERY_SERVER_STATE("queryServerState");
	static const string HTTP_CMD_QUERY_DEVICE_STATE("queryDeviceState");
	static const string HTTP_CMD_QUERY_DECODER_STATE("queryDecoderState");
	static const string HTTP_CMD_QUERY_ALARMHOST_STATE("queryAlarmHostState");
	static const string HTTP_CMD_QUERY_RECORD_STATE("queryRecordState");
	static const string HTTP_CMD_QUERY_CHL_ONLINE_STATE("queryChlOnlineState");
	static const string HTTP_CMD_QUERY_ACSSYSTEM_STATE("queryAcsSystemState");
	static const string HTTP_CMD_QUERY_STATE_LOG("queryStateLog");//查询状态日志    
	static const string HTTP_CMD_EXPORT_STATE_LOG("exportStateLog");//导出状态日志    

	//N5000的通道组
	static const string HTTP_CMD_QUERY_CHL_GROUP_LIST("queryChlGroupList");//查询通道组列表
	static const string HTTP_CMD_QUERY_CHL_GROUP("queryChlGroup");//查询单个通道组的信息

	//N5000的报警输出组
	static const string HTTP_CMD_QUERY_ALARMOUT_GROUP_LIST("queryAlarmOutGroupList");//查询报警输出组列表
	static const string HTTP_CMD_QUERY_ALARMOUT_GROUP("queryAlarmOutGroup");//查询单个报警输出组的信息

	//多节点告警任务
	static const string HTTP_CMD_QUERY_ALARMTASK_GROUP_LIST("queryMultiNodeAlarmTaskList");//查询告警任务组列表
	static const string HTTP_CMD_QUERY_ALARMTASK_GROUP("queryMultiNodeAlarmTask");//查询单个告警任务组的信息

	static const string HTTP_CMD_QUERY_SOP_LIST("querySOPInfoList");//查询处警标准操作列表
	static const string HTTP_CMD_QUERY_SOP("querySOPInfo");//查询单个处警标准操作的信息

	//配置服务器通过管理将配置写入到库
	static const string HTTP_CMD_ADD_CONFIGLOG_RECORD_LOG("addConfigLogRecord");//写配置日志
	static const string HTTP_CMD_ADD_OPERATELOG_RECORD_LOG("addOperateLogRecord");//写操作日志

	//查询局域网空闲的转发服务器
	static const string HTTP_CMD_QUERY_LAN_FREE_MEDIASERVER_LIST("queryLanFreeMediaServerList");

	//查询局域网空闲的存储服务器
	static const string HTTP_CMD_QUERY_LAN_FREE_STORAGESERVER_LIST("queryLanFreeStorageServerList");

	//查询局域网空闲的服务器
	static const string HTTP_CMD_QUERY_LAN_FREE_SERVER_LIST("queryLanFreeServerList");

	//查询license信息
	static const string HTTP_CMD_QUERY_LICENSE_INFO("queryLicenseInfo");

	//导入license
	static const string HTTP_CMD_IMPORT_LICENSE("importLicense");

	//查询不在线的监控点
	static const string HTTP_CMD_QUERY_OFFLINE_CHL_LIST("queryOfflineChlList");

	//查询管理服务器信息
	static const string HTTP_CMD_QUERY_RSU_CFG("queryRSUCfg");

	//N9K,IPC兼容配置命令
	static const string HTTP_CMD_ADAPT_REQUEST_SAVE_PRESET("requestSavePreset");
	static const string HTTP_CMD_ADAPT_REQUEST_CONFIG_CRUISE("requsetConfigCurise");

	//查询任务排程
	static const string HTTP_CMD_QUERY_TASK_EVENT_DICTIONARY("queryTaskEventDictionary");
	static const string HTTP_CMD_QUERY_TASK_SCHEDULE("queryTaskSchedule");
	static const string HTTP_CMD_EDIT_TASK_SCHEDULE("editTaskSchedule");

	//查询报警事件联动
	static const string HTTP_CMD_QUERY_ALARM_EVENT_DICTIONARY("queryAlarmEventDictionary");
	static const string HTTP_CMD_QUERY_ALARM_TRIGGER("queryAlarmTrigger");
	static const string HTTP_CMD_EDIT_ALARM_TRIGGER("editAlarmTrigger");

	//语音播报文件
	static const string HTTP_CMD_Create_VOICE_BROADCAST_FILE("createVoiceBroadcastFile");
	static const string HTTP_CMD_Delete_VOICE_BROADCAST_FILE("delVoiceBroadcastFile");
	static const string HTTP_CMD_Query_VOICE_BROADCAST_FILE("queryVoiceBroadcastFileList");

	//配置服务器发送数据到管理--通用
	static const string HTTP_CMD_SEND_DATA_TO_RSU("sendCommonDataToRsu");

	static const string SEND_DATA_TO_RSU_OutLicenseForAddChannel("OutLicenseForAddChannel");

	//请求存储服务器的分区信息
	static const string SEND_DATA_TO_RSU_RequestPartitionList("RequestPartitionList");

	//PMS固定车辆
	static const string HTTP_CMD_CREATE_PMSFIXEDVEHICLE_LIST("createPmsFixedVehicleList");
	static const string HTTP_CMD_QUERY_PMSFIXEDVEHICLE_LIST("queryPmsFixedVehicleList");
	static const string HTTP_CMD_DEL_PMSFIXEDVEHICLE_LIST("delPmsFixedVehicleList");
	static const string HTTP_CMD_EDIT_PMSFIXEDVEHICLE_LIST("editPmsFixedVehicleList");

	//PMS车辆组
	static const string HTTP_CMD_CREATE_PMSVEHICLEGROUP_LIST("createPmsVehicleGroupList");
	static const string HTTP_CMD_DEL_PMSVEHICLEGROUP_LIST("delPmsVehicleGroupList");
	static const string HTTP_CMD_EDIT_PMSVEHICLEGROUP_LIST("editPmsVehicleGroupList");

	//PMS黑名单车辆---弃用，合入固定车辆
	static const string HTTP_CMD_CREATE_PMSBLACKLISTCAR_LIST("createPmsBlacklistCarList");
	static const string HTTP_CMD_QUERY_PMSBLACKLISTCAR_LIST("queryPmsBlacklistCarList");
	static const string HTTP_CMD_DEL_PMSBLACKLISTCAR_LIST("delPmsBlacklistCarList");
	static const string HTTP_CMD_EDIT_PMSBLACKLISTCAR_LIST("editPmsBlacklistCarList");

	//PMS LED显示屏
	static const string HTTP_CMD_CREATE_PMSLED_LIST("createPmsLedList");
	static const string HTTP_CMD_QUERY_PMSLED_LIST("queryPmsLedList");
	static const string HTTP_CMD_DEL_PMSLED_LIST("delPmsLedList");
	static const string HTTP_CMD_EDIT_PMSLED_LIST("editPmsLedList");

	//PMS 车道绑定LED/摄像机
	static const string HTTP_CMD_CREATE_PMLANECAMORLED_LIST("createPmsCamorLedList");
	static const string HTTP_CMD_DEL_PMLANECAMORLED_LIST("delPmsCamorLedList");

	//PMS 创建停车场
	static const string HTTP_CMD_PMS_CREATE_PARK("createPmsPark");
	//PMS 创建出入口
	static const string HTTP_CMD_PMS_CREATE_GATEWAY("createPmsGateway");
	//PMS 创建车道
	static const string HTTP_CMD_PMS_CREATE_LANE("createPmsLane");
	//PMS 修改主停车场
	static const string HTTP_CMD_EDIT_PARK_INFO("editPmsParkInfo");
	//PMS 修改出入口
	static const string HTTP_CMD_EDIT_GATWAY_INFO("editPmsGatway");
	//PMS 修改车道
	static const string HTTP_CMD_EDIT_LANE_INFO("editPmsLaneInfo");
	//PMS 删除 1:停车场/2:出入口/3:车道
	static const string HTTP_CMD_DEL_PARK_GATEWAY_LANE("delPmsParkGatewayLane");

	//批量修改停车场/车道/出入口
	static const string HTTP_CMD_EDIT_MUTI_PARK_GATEWAY_LANE_INFO("editPmsParkGatewayLaneInfo");

	//PMS 收费规则
	static const string HTTP_CMD_CREATE_PMSCHARGERULES("createPmsChargeRules");
	static const string HTTP_CMD_QUERY_PMSCHARGERULES("queryPmsChargeRules");
	static const string HTTP_CMD_DEL_PMSCHARGERULES("delPmsChargeRules");
	static const string HTTP_CMD_EDIT_PMSCHARGERULES("editPmsChargeRules");

	//PMS 自定义收费规则
	static const string HTTP_CMD_CREATE_PMSSECTIONCHARGERULES("createPmsSectionChargeRules");
	static const string HTTP_CMD_QUERY_PMSSECTIONCHARGERULES("queryPmsSectionChargeRules");
	static const string HTTP_CMD_DEL_PMSSECTIONCHARGERULES("delPmsSectionChargeRules");
	static const string HTTP_CMD_EDIT_PMSSECTIONCHARGERULES("editPmsSectionChargeRules");

	//PMS 包期规则
	static const string HTTP_CMD_CREATE_PMSBAGPERIODRULE("createPmsBagPeriodRule");
	static const string HTTP_CMD_QUERY_PMSBAGPERIODRULE("queryPmsBagPeriodRule");
	static const string HTTP_CMD_DEL_PMSBAGPERIODRULE("delPmsBagPeriodRule");
	static const string HTTP_CMD_EDIT_PMSBAGPERIODRULE("editPmsBagPeriodRule");

	//PMS 通行记录
    static const string HTTP_CMD_COUNT_PMSPASSREOCRD("countPmsPassRecord");
	static const string HTTP_CMD_QUERY_PMSPASSREOCRD("queryPmsPassRecord");
	static const string HTTP_CMD_DEL_PMSPASSRECORD("delPmsPassRecord");
	static const string HTTP_CMD_EDIT_PMSPASSRECORD("editPmsPassRecord");
	static const string HTTP_CMD_QUERY_PMSPARKTIME("queryPmsParkTime");

	// 访客记录
	static const string HTTP_CMD_FUZZYQUERY_LOGVISITORRECORD("fuzzyQueryLogVisitorRecord");
	static const string HTTP_CMD_QUERY_LOGVISITORRECORD("queryLogVisitorRecord");
	static const string HTTP_CMD_DEL_LOGVISITORRECORD("delLogVisitorRecord");
	static const string HTTP_CMD_EDIT_LOGVISITORRECORD("editLogVisitorRecord");
	static const string HTTP_CMD_ADD_LOGVISITORRECORD("addLogVisitorRecord");

	//PMS 通行收费记录  
	static const string HTTP_CMD_CREATE_PMSPASSCHARGE("createPmsPassCharge");
	static const string HTTP_CMD_QUERY_PMSPASSCHARGE("queryPmsPassCharge");
	//PMS 固定车缴费记录
	static const string HTTP_CMD_PMS_ADD_ACCOUNTCHARGE("addPmsAccountCharge");
	static const string HTTP_CMD_PMS_QUERY_ACCOUNTCHARGE("queryPmsAccountCharge");

	//访客机人员信息记录
	static const string HTTP_CMD_QUERY_FKVISITORREOCRD("queryFkVisitorInfoList");
	static const string HTTP_CMD_DEL_FKVISITORRECORD("editFkVisitorInfoList");
	static const string HTTP_CMD_EDIT_FKVISITORRECORD("delFkVisitorInfoList");
	static const string HTTP_CMD_EDIT_FKVISITORFORMCARD("delFkVisitorformCard");

	//访客机----部门管理
	static const string HTTP_CMD_CREATEFKDEPARTMENT("createDepartment");
	static const string HTTP_CMD_CREATEFKGROUP("createGroup");
	static const string HTTP_CMD_MODIFYFKDEPARTMENT("modifyDepartment");
	static const string HTTP_CMD_MODIFYFKGROUP("modifyGroup");
	static const string HTTP_CMD_DELETEFKDEPARTMENT("deleteDepartment");
	static const string HTTP_CMD_DELETEFKGROUP("deleteGroup");

	//级联平台
	static const string HTTP_CMD_CREATE_CASCADEPLATFORM_LIST("createCascadePlatformList");
	static const string HTTP_CMD_QUERY_CASCADEPLATFORM_LIST("queryCascadePlatformList");
	static const string HTTP_CMD_DEL_CASCADEPLATFORM_LIST("delCascadePlatformList");
	static const string HTTP_CMD_EDIT_CASCADEPLATFORM_LIST("editCascadePlatformList");
	
	//
	static const string HTTP_CMD_EDIT_CONFIG_INFO("editConfigInfo");
	static const string HTTP_CMD_EDIT_CUSTOM_INFO("editCustomInfo");
	//查询设备详细信息
	static const string HTTP_CMD_GETDEVICEDETAIL("GetDeviceDetail");

	//向图片服务器获取或存储图片
	static const string HTTP_CMD_GET_PICTURE("getPicture");
	static const string HTTP_CMD_SAVE_PICTURE("savePicture");

	//GB28181 GAT1400 id配置
	static const string HTTP_CMD_EDIT_UPPERGBGATID("editUpperGBGATID");
	static const string HTTP_CMD_EDIT_GAT1400ID("editGAT1400ID");//配置设备本身的GAT1400id
	//查询主动上报到平台，但是平台未添加的设备。
	static const string HTTP_CMD_QUERY_REPORTENCODE_DEVICE_LIST("queryReportDeviceNoAddVms");


	
	//wifi探针
	static const string HTTP_CMD_CREATEWIFIPROBE("createWifiProbeList");
	static const string HTTP_CMD_MODIFYWIFIPROBE("editWifiProbeList");
	static const string HTTP_CMD_DELETEWIFIPROBE("delWifiProbeList");

	//wifi探针记录
	static const string HTTP_CMD_QUERY_WIFIPROBERREOCRD("queryWifiProbeInfoList");
	static const string Cmd_Device_Manage_Delete_WifiProbe_Record_ByTimePoint = "deleteWifiProbeInfoList_ByTimePoint";
	static const string Cmd_Device_Manage_Delete_WifiProbe_Record_ByTimePeriod = "deleteWifiProbeInfoList_ByTimePeriod";

	//节点经纬度和行政区域代码设置
	static const string HTTP_CMD_EDIT_COORDINATE("editCoordinate");
    //N9K控制道闸
    static const string HTTP_CMD_CHL_BARRIER_CONTROL("chlBarrierControl");
}
#endif //CONFIG_CMD_DEFINE_HEAD


