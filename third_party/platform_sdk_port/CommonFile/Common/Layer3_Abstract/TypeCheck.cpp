
#include "TypeCheck.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
CTypeCheck::CPUTypeCheck::CPUTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}

CTypeCheck::CPUTypeCheck::CPUTypeCheck(tuint32 dwTypeValue)
{
	m_dwTypeValue = dwTypeValue;
}

CTypeCheck::CPUTypeCheck::~CPUTypeCheck()
{

}

tuint32 CTypeCheck::CPUTypeCheck::GetTWAbstractDevice() 
{
	return NODE_TYPE_PERIPHERAL_ABSTRACT_DEVICE;
}

tuint32 CTypeCheck::CPUTypeCheck::GetTWIPCTypeValue() 
{
	return PU_TVT_IPCAMERA;
}

tuint32 CTypeCheck::CPUTypeCheck::GetTWDVRTypeValue() 
{
	return PU_TVT_DVR;
}
tuint32 CTypeCheck::CPUTypeCheck::GetHUAANIPCTypeValue() 
{
	return PU_HUAAN_IPC;
}
tuint32 CTypeCheck::CPUTypeCheck::GetTWMobileDVRTypeValue() 
{
	return PU_TVT_MDVR;
}

tuint32 CTypeCheck::CPUTypeCheck::GetHiKVisionDVRTypeValue()
{
	return PU_HIKVISION_DVR;
}
tuint32 CTypeCheck::CPUTypeCheck::GetDaHuaDVRTypeValue()
{
	return PU_DAHUA_DVR;
}

tuint32 CTypeCheck::CPUTypeCheck::GetN9000NVRTypeValue()
{
	return PU_JOIN_PROTOCOL_N9000;
}

tuint32 CTypeCheck::CPUTypeCheck::GetONVIFTypeValue()
{
	return PU_JOIN_PROTOCOL_ONVIF;
}

tuint32 CTypeCheck::CPUTypeCheck::GetN9000DVRTypeValue()
{
	return PU_TVT_N9000_DVR;
}

tuint32 CTypeCheck::CPUTypeCheck::GetAnalogChannelTypeValue()
{
	return PROTOCOL_TYPE_ANALOG_CHANNEL;
}

tuint32 CTypeCheck::CPUTypeCheck::GetZenoBoxTypeValue()
{
	return PU_ZENO_BOX;
}

tuint32 CTypeCheck::CPUTypeCheck::GetGBDeviceTypeValue()
{
	return PU_GB_DEVICE;
}

tuint32 CTypeCheck::CPUTypeCheck::GetRTSPTypeValue()
{	
	return PU_JOIN_PROTOCOL_RTSP;
}

tuint32 CTypeCheck::CPUTypeCheck::Get4GIPCTypeValue()
{
	return PU_4G_IPC;
}

tuint32 CTypeCheck::CPUTypeCheck::GetHuaAnAutoReportTypeValue()
{
	return PU_HUAAN_AUTOREPORT;
};

tuint32 CTypeCheck::CPUTypeCheck::GetSenseTimePadAutoReportTypeValue()
{
    return PU_SENSETIME_PAD_AUTOREPORT;
};

tuint32 CTypeCheck::CPUTypeCheck::GetThinkForceTypeValue()
{
	return PU_THINKFORCE_PAD;
}

tuint32 CTypeCheck::CPUTypeCheck::GetYITUTypeValue()
{
	return PU_YITU_BOX;
}

tuint32 CTypeCheck::CPUTypeCheck::GetYITUPadTypeValue()
{
	return PU_YITU_PAD;
}

tuint32 CTypeCheck::CPUTypeCheck::GetAOPUTypeValue()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_AOPU;
}

tuint32 CTypeCheck::CPUTypeCheck::GetPrIPCTypeValue()
{
	return PU_PR_IPC;
}

tuint32 CTypeCheck::CPUTypeCheck::GetEdgeBoxTypeValue()
{
    return PU_EDGE_BOX;
}

//报警主机
/************************************************************************/
/*                                                                      */
/************************************************************************/
CTypeCheck::CAlarmHostTypeCheck::CAlarmHostTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}

CTypeCheck::CAlarmHostTypeCheck::CAlarmHostTypeCheck(tuint32 dwTypeValue)
{
	m_dwTypeValue = dwTypeValue;
}

CTypeCheck::CAlarmHostTypeCheck::~CAlarmHostTypeCheck()
{

}
//是否为海康网络模块
tuint32 CTypeCheck::CAlarmHostTypeCheck::GetHIKNetModuleTypeValue()
{
	return ALARMHOST_HIKMODULE;
}

//是否为新科姆报警主机
tuint32 CTypeCheck::CAlarmHostTypeCheck::GetXKMAlarmHostTypeValue()
{
	return NODE_TYPE_ALARMHOST_TYPE_XKM;
}

//是否为艾礼安报警主机
tuint32 CTypeCheck::CAlarmHostTypeCheck::GetAleanAlarmHostTypeValue()
{
	return NODE_TYPE_ALARMHOST_TYPE_ALEAN;
}

//DSC pc4401
tuint32 CTypeCheck::CAlarmHostTypeCheck::GetDscPC4401AlarmHostTypeValue()
{
	return NODE_TYPE_ALARMHOST_TYPE_DSC_PC4401;
}

//Paradox
tuint32 CTypeCheck::CAlarmHostTypeCheck::GetParadoxAlarmHostTypeValue()
{
	return NODE_TYPE_ALARMHOST_TYPE_PARADOX;
}

//BOSCH 7400
tuint32 CTypeCheck::CAlarmHostTypeCheck::GetBosch7400AlarmHostTypeValue()
{
	return NODE_TYPE_ALARMHOST_TYPE_BOSCH_7400;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CTypeCheck::CDecoderTypeCheck::CDecoderTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}

CTypeCheck::CDecoderTypeCheck::CDecoderTypeCheck(tuint32 dwTypeValue)
{
	m_dwTypeValue = dwTypeValue;
}

CTypeCheck::CDecoderTypeCheck::~CDecoderTypeCheck()
{

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CTypeCheck::CProtocolTypeCheck::CProtocolTypeCheck()
{
    m_dwProtocolTypeValue = NODE_TYPE_INVALID_NULL;
}

CTypeCheck::CProtocolTypeCheck::CProtocolTypeCheck(tuint32 dwProtocolTypeValue)
{
    m_dwProtocolTypeValue = dwProtocolTypeValue;
}

CTypeCheck::CProtocolTypeCheck::~CProtocolTypeCheck()
{
    m_dwProtocolTypeValue = NODE_TYPE_INVALID_NULL;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetTWIPC_SDK_ProtocolTypeValue()
{
    return PU_TVT_IPCAMERA;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetTWDVR_SDK_Protocol_TypeValue()
{
    return PU_TVT_DVR;
}
tuint32 CTypeCheck::CProtocolTypeCheck::GetHuaAnIPC_SDK_Protocol_TypeValue()
{
    return PU_HUAAN_IPC;
}
tuint32 CTypeCheck::CProtocolTypeCheck::GetTWMobileDVR_SDK_Protocol_TypeValue()
{
    return PU_TVT_MDVR;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetHiKVision_SDK_Protocol_TypeValue()
{
    return PU_HIKVISION_DVR;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetDaHua_SDK_Protocol_TypeValue()
{
    return PU_DAHUA_DVR;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetZeno_SDK_Protocol_TypeValue()
{
	return PU_ZENO_BOX;
}

tuint32 CTypeCheck::CProtocolTypeCheck::Get4GIPC_SDK_Protocol_TypeValue()
{
	return PU_4G_IPC;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetHuaAnAutoReport_SDK_Protocol_TypeValue()
{
	return PU_HUAAN_AUTOREPORT;
}

 tuint32 CTypeCheck::CProtocolTypeCheck::GetSenseTimePadAutoReport_SDK_Protocol_TypeValue()
{
    return PU_SENSETIME_PAD_AUTOREPORT;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetThinkForce_SDK_Protocol_TypeValue()
{
    return PU_THINKFORCE_PAD;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetONVIF_Protocol_TypeValue()
{
	return PU_JOIN_PROTOCOL_ONVIF;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetRTSP_Protocol_TypeValue()
{
	return PU_JOIN_PROTOCOL_RTSP;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetN9000NVR_Protocol_TypeValue()
{
	return PU_JOIN_PROTOCOL_N9000;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetN9000DVR_Protocol_TypeValue()
{
	return PU_TVT_N9000_DVR;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetAnalogChannel_Protocol_TypeValue()
{
	return PROTOCOL_TYPE_ANALOG_CHANNEL;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetGB_Protocol_TypeValue()
{
	return PU_GB_DEVICE;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetYITU_Protocol_TypeValue()
{
	return PU_YITU_BOX;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetYITUPad_Protocol_TypeValue(){
    return PU_YITU_PAD;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetAOPU_Protocol_TypeValue()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_AOPU;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetPrIPC_SDK_Protocol_TypeValue()
{
	return PU_PR_IPC;
}

tuint32 CTypeCheck::CProtocolTypeCheck::GetEdgeBox_Protocol_TypeValue() 
{
    return PU_EDGE_BOX;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CTypeCheck::CEmapFileTypeCheck::CEmapFileTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}

CTypeCheck::CEmapFileTypeCheck::CEmapFileTypeCheck(tuint32 dwProtocolTypeValue)
{
	m_dwTypeValue = dwProtocolTypeValue;
}

CTypeCheck::CEmapFileTypeCheck::~CEmapFileTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
tuint32 CTypeCheck::CEmapFileTypeCheck::GetPictue_EmapTypeValue()
{
	return NODE_TYPE_EMAP_TYPE_PICTURE;
}
tuint32 CTypeCheck::CEmapFileTypeCheck::GetBaidu_EmapTypeValue()
{
	return NODE_TYPE_EMAP_TYPE_BAIDU;
}
tuint32 CTypeCheck::CEmapFileTypeCheck::GetGoogle_EmapTypeValue()
{
	return NODE_TYPE_EMAP_TYPE_GOOGLE;
}
tuint32 CTypeCheck::CEmapFileTypeCheck::GetGaode_EmapTypeValue()
{
	return NODE_TYPE_EMAP_TYPE_GAODE;
}
tuint32 CTypeCheck::CEmapFileTypeCheck::GetWapian_EmapTypeValue()
{
	return NODE_TYPE_EMAP_TYPE_WAPIAN;
}

//门禁考勤系统
CTypeCheck::CAcsSystemTypeCheck::CAcsSystemTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
CTypeCheck::CAcsSystemTypeCheck::CAcsSystemTypeCheck(tuint32 dwProtocolTypeValue)
{
	m_dwTypeValue = dwProtocolTypeValue;
}
CTypeCheck::CAcsSystemTypeCheck::~CAcsSystemTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
tuint32 CTypeCheck::CAcsSystemTypeCheck::GetAbstractAcsSystem()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_ABSTRACT;
}
tuint32 CTypeCheck::CAcsSystemTypeCheck::GetZktEcoAcsSystemTypeValue()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_ZKTECO;
}
tuint32 CTypeCheck::CAcsSystemTypeCheck::GetZLCXAcsSystemTypeValue()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_ZLCX;
}

tuint32 CTypeCheck::CAcsSystemTypeCheck::GetSZWGAcsSystemTypeValue()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_SZWG;
}

tuint32 CTypeCheck::CAcsSystemTypeCheck::GetKANTECHAcsSystemTypeValue()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_KANTECH;
}
tuint32 CTypeCheck::CAcsSystemTypeCheck::GetAOPUAcsSystemTypeValue()
{
	return NODE_TYPE_ACSSYSTEM_TYPE_AOPU;
}

/************************************************************************/
/*   访客机                                                                */
/************************************************************************/
CTypeCheck::CFkTypeCheck::CFkTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
CTypeCheck::CFkTypeCheck::CFkTypeCheck(tuint32 dwProtocolTypeValue)
{
	m_dwTypeValue = dwProtocolTypeValue;
}
CTypeCheck::CFkTypeCheck::~CFkTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
tuint32 CTypeCheck::CFkTypeCheck::GetQianLinValue()
{
	return NODE_TYPE_FK_TYPE_QIANLIN;
}
tuint32 CTypeCheck::CFkTypeCheck::GetDepartmentValue()
{
	return NODE_TYPE_FK_DEPARTMENT_TYPE;
}

//级联平台
CTypeCheck::CCascadePlatformTypeCheck::CCascadePlatformTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
CTypeCheck::CCascadePlatformTypeCheck::CCascadePlatformTypeCheck(tuint32 dwProtocolTypeValue)
{
	m_dwTypeValue = dwProtocolTypeValue;
}
CTypeCheck::CCascadePlatformTypeCheck::~CCascadePlatformTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
tuint32 CTypeCheck::CCascadePlatformTypeCheck::GetAbstractCascadePlatform()
{
	return NODE_TYPE_CASCADE_PLATFORM_TYPE_ABSTRACT;
}
tuint32 CTypeCheck::CCascadePlatformTypeCheck::GetGB28181CascadePlatformTypeValue()
{
	return NODE_TYPE_CASCADE_PLATFORM_TYPE_GB28181;
}

//LED屏
tuint32 CTypeCheck::CPmsLedTypeCheck::GetAbstractPmsLed()
{
	return NODE_TYPE_SCREEN_LED_ABSTRACT_DEVICE;
}

tuint32 CTypeCheck::CPmsLedTypeCheck::GetIPHongMenLedTypeValue()
{
	return NODE_TYPE_SCREEN_LED_IP_HONGMEN;
}

tuint32 CTypeCheck::CPmsLedTypeCheck::GetCheYiFuTypeValue()
{
	return NODE_TYPE_SCREEN_LED_CHEYIFU;
}

tuint32 CTypeCheck::CPmsLedTypeCheck::GetXCP10LedTypeValue()
{
    return NODE_TYPE_SCREEN_LED_XCP10;
}

CTypeCheck::CWifiProbeTypeCheck::CWifiProbeTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
CTypeCheck::CWifiProbeTypeCheck::CWifiProbeTypeCheck(tuint32 dwTypeValue)
{
	m_dwTypeValue = dwTypeValue;
}
CTypeCheck::CWifiProbeTypeCheck::~CWifiProbeTypeCheck()
{
	m_dwTypeValue = NODE_TYPE_INVALID_NULL;
}
tuint32 CTypeCheck::CWifiProbeTypeCheck::GetWifiProbeValue()
{
	return NODE_TYPE_WIFIPROBE_TYPE;
}


//分区组
bool CTypeCheck::IsPartitionGroup_s(tuint32 nodeType)
{
	return (nodeType == GetPartitionGroupTypeValue());
}
//分区
bool CTypeCheck::IsPartition_s(tuint32 nodeType)
{
	return (nodeType == GetPartitionTypeValue());
}

bool CTypeCheck::IsAcsDevice_s(tuint32 nodeType)
{
	return (nodeType == GetAcsDeviceTypeValue());
}
bool CTypeCheck::IsAcsDoor_s(tuint32 nodeType)
{
	return (nodeType == GetAcsDoorTypeValue());
}
bool CTypeCheck::IsAcsReader_s(tuint32 nodeType)
{
	return (nodeType == GetAcsReaderTypeValue());
}
bool CTypeCheck::IsConfig_s(tuint32 nodeType)
{
	return (nodeType == GetConfigTypeValue());
}
bool CTypeCheck::IsCustom_s(tuint32 nodeType)
{
	return (nodeType == GetCustomTypeValue());
}
bool CTypeCheck::IsN5000Server_s(tuint32 nodeType)
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsN5000Server()); 
	
}

/************************************************************************/
/*                  CTypeCheck                                                    */
/************************************************************************/
CTypeCheck::CTypeCheck(tuint32 dwType)
{
	m_dwType = dwType;
}

CTypeCheck::~CTypeCheck(void)
{
}

//是否为媒体存储单元
bool CTypeCheck::IsMSU(void) const
{
	return (m_dwType ==  GetMSUTypeValue());     
}

//是否为GPS存储单元
bool CTypeCheck::IsGSU(void) const
{
	return (m_dwType ==  GetGSUTypeValue());     
}

//是否为媒体转发单元
bool CTypeCheck::IsMDU(void) const
{
	return (m_dwType == GetMDUTypeValue() );  
}

//是否为独立媒体转发服务器
bool CTypeCheck::IsMediaServer(void) const
{
	return (m_dwType == GetMediaServerTypeValue() );  
}

bool CTypeCheck::IsPCNVR9000(void) const
{
	return (m_dwType == GetPCNVR9000TypeValue() );  
}

//是否为注册服务器单元
bool CTypeCheck::IsRSU(void) const
{
	return (m_dwType == GetRSUTypeValue() );  
}

//是否为电子地图单元
/*	BOOL IsEMU(void) const;*/

//是否为报警转发单元
bool CTypeCheck::IsADU(void) const
{
	return (m_dwType == GetADUTypeValue() );  
}

//是否为智能分析单元
bool CTypeCheck::IsANS(void) const
{
	return (m_dwType == GetANSTypeValue() );  
}

//是否为级联网关单元
bool CTypeCheck::IsCGU(void) const
{
	return (m_dwType == GetCGUTypeValue() );  
}
//是否为应用服务器单元
bool CTypeCheck::IsAPPServer(void) const
{
	return (m_dwType == GetApplicationTypeValue());  
}
/////////是否为1400上级网关单元
bool CTypeCheck::Is1400IN(void) const
{
	return (m_dwType == Get1400INTypeValue() );  
}

//是否为GPS警情分析单元
bool CTypeCheck::IsGPSServer(void) const
{
	return (m_dwType == GetGPSServerTypeValue() );  
}

//是否电视墙服务器
bool CTypeCheck::IsTVW(void) const
{
	return (m_dwType == GetTVWTypeValue());  
}

//是否为墙
bool CTypeCheck::IsWall(void) const
{
	return (m_dwType == GetWallTypeValue());  
}

//是否为通道组
bool CTypeCheck::IsChannelGroup(void) const
{
	return (m_dwType == GetChannelGroupValue());
}

//是否为报警输出组
bool CTypeCheck::IsAlarmOutGroup(void) const
{
	return (m_dwType == GetAlarmOutGroupValue());
}

//是否为告警任务组
bool CTypeCheck::IsAlarmTaskGroup(void) const{
	return (m_dwType == GetAlarmTaskTypeValue());
}

//是否为处警标准操作
bool CTypeCheck::IsSop(void) const
{
	return (m_dwType == GetSopValue());
}
//是否为收件人信息
bool CTypeCheck::IsEmail(void) const
{
	return (m_dwType == GetEmailTypeValue());
}


//是否接入服务器
bool CTypeCheck::IsJSU(void) const
{
	return (m_dwType == GetJSUTypeValue()); 
}

//是否状态服务器
bool CTypeCheck::IsSTU(void) const
{
	return (m_dwType == GetSTUTypeValue()); 
}

bool CTypeCheck::IsClient(void) const
{
	if (m_dwType == NODE_TYPE_NET_KEYBOARD)
	{
		return true;
	}
	return ((m_dwType >=NODE_TYPE_CLIENT_BEGIN ) && (m_dwType <= NODE_TYPE_CLIENT_END));  
}

/////////是否为Monitor单元
bool CTypeCheck::IsMCU(void) const
{
	return (m_dwType == GetMCUTypeValue() );  
}

/////////是否为网络键盘单元
bool CTypeCheck::IsNKB(void) const
{
	return (m_dwType == GetNetkeyboardTypeValue() );  
}

/////////是否为配置客户端单元
bool CTypeCheck::IsCCU(void) const
{
	return (m_dwType == GetCCUTypeValue() );  
}

/////////是否为解码器客户端单元
bool CTypeCheck::IsDCU(void) const
{
	return (m_dwType == GetDecoderClientTypeValue() );  
}

//是否为N5000的服务器
bool CTypeCheck::IsN5000Server() const
{
	return ( (m_dwType >= NODE_TYPE_NVMS5000_SERVER_BEGIN) && \
		m_dwType < NODE_TYPE_NVMS5000_SERVER_END );  
}

/////////是否为编码设备单元
bool CTypeCheck::IsPU(CPUTypeCheck *pPUTypeCheck) const
{
	if(NULL != pPUTypeCheck)
	{
		pPUTypeCheck->m_dwTypeValue = m_dwType;
	}

	return ( (m_dwType > NODE_TYPE_PERIPHERAL_DEVICE_BEGIN) && \
		m_dwType < NODE_TYPE_PERIPHERAL_DEVICE_END );  
}

/////////是否为解码设备单元
bool CTypeCheck::IsDecoder(CDecoderTypeCheck *pDecoderTypeCheck ) const
{
	if(NULL != pDecoderTypeCheck)
	{
		pDecoderTypeCheck->m_dwTypeValue = m_dwType;
	}

	return ( (m_dwType > NODE_TYPE_PERIPHERAL_DECODER_BEGIN) && \
		m_dwType < NODE_TYPE_PERIPHERAL_DECODER_END ); 
}

//是否为报警主机
bool CTypeCheck::IsAlarmHost(CAlarmHostTypeCheck *pAlarmHostTypeCheck ) const
{
	if(NULL != pAlarmHostTypeCheck)
	{
		pAlarmHostTypeCheck->m_dwTypeValue = m_dwType;
	}

	return ( (m_dwType > NODE_TYPE_PERIPHERAL_ALARMHOST_BEGIN) && \
		m_dwType < NODE_TYPE_PERIPHERAL_ALARMHOST_END ); 
}

bool CTypeCheck::IsAlarmIn(CAlarmInTypeCheck *pAlarmInTypeCheck) const
{
	if(NULL != pAlarmInTypeCheck)
	{
		pAlarmInTypeCheck->m_dwTypeValue = m_dwType;
	}

	return ( (m_dwType > NODE_TYPE_PERIPHERAL_ALARMIN_BEGIN) && \
		m_dwType < NODE_TYPE_PERIPHERAL_ALARMIN_END );  
}

bool CTypeCheck::IsAlarmOut(CAlarmOutTypeCheck *pAlarmOutTypeCheck) const
{
	if(NULL != pAlarmOutTypeCheck)
	{
		pAlarmOutTypeCheck->m_dwTypeValue = m_dwType;
	}

	return ( (m_dwType > NODE_TYPE_PERIPHERAL_ALARMOUT_BEGIN) && \
		m_dwType < NODE_TYPE_PERIPHERAL_ALARMOUT_END );  

}

/////////是否为门禁考勤报警系统
bool CTypeCheck::IsAcsSystem(CAcsSystemTypeCheck *pTypeCheck ) const
{
	if(NULL != pTypeCheck)
	{
		pTypeCheck->m_dwTypeValue = m_dwType;
	}

	return ( (m_dwType > NODE_TYPE_ACSSYSTEM_TYPE_BEGIN) && \
		m_dwType < NODE_TYPE_ACSSYSTEM_TYPE_END );  
}

bool CTypeCheck::IsFkSystem(CFkTypeCheck *pTypeCheck) const 
{
	if(NULL != pTypeCheck)
	{
		pTypeCheck->m_dwTypeValue = m_dwType;
	}

	return ( (m_dwType > NODE_TYPE_FK_TYPE_BEGIN) && \
		m_dwType < NODE_TYPE_FK_TYPE_END );  
}

//是否为目标库
bool CTypeCheck::IsAlbum(void) const
{
	return (m_dwType == GetAlbumTypeValue() );  
}

//是否为级联平台
bool CTypeCheck::IsCascadePlatform(void) const
{
	return ( (m_dwType > NODE_TYPE_CASCADE_PLATFORM_TYPE_BEGIN) && \
		m_dwType < NODE_TYPE_CASCADE_PLATFORM_TYPE_END );  
}

//是否为通道节点
bool CTypeCheck::IsChannel(void) const
{
	return (m_dwType == GetChnnTypeValue() );  
}

bool CTypeCheck::IsIPChannel(void) const
{
	return m_dwType == GetIPChannelTypeValue();
}

//是否为子系统
bool CTypeCheck::IsSubSystem(void) const
{
	return (m_dwType == GetSubSystemTypeValue() );  
}
//是否为防区
bool CTypeCheck::IsZone(void) const
{
	return (m_dwType == GetZoneTypeValue() );  
}

//是否为门禁设备
bool CTypeCheck::IsAcsDevice(void) const
{
	return (m_dwType == GetAcsDeviceTypeValue() );  
}
//是否为门
bool CTypeCheck::IsAcsDoor(void) const
{
	return (m_dwType == GetAcsDoorTypeValue() );  
}
//是否为读头
bool CTypeCheck::IsAcsReader(void) const
{
	return (m_dwType == GetAcsReaderTypeValue() );  
}
//pms
bool CTypeCheck::IsPmsPark(void) const
{
	return (m_dwType == GetPmsParkTypeValue() ); 
}
bool CTypeCheck::IsPmsGateway(void) const
{
	return (m_dwType == GetPmsGatewayTypeValue() ); 
}
bool CTypeCheck::IsPmsLane(void) const
{
	return (m_dwType == GetPmsLaneTypeValue() ); 
}
bool CTypeCheck::IsPmsLed(void) const
{
	return ( (m_dwType > NODE_TYPE_SCREEN_LED_BEGIN) && \
		m_dwType < NODE_TYPE_SCREEN_LED_END );   
}
bool CTypeCheck::IsPmsChargeRule(void) const
{
	return (m_dwType == GetPmsChargeRuleTypeValue() ); 
}
bool CTypeCheck::IsPmsPassRecord(void) const
{
	return (m_dwType == GetPmsPassRecordTypeValue() ); 
}
bool CTypeCheck::IsPmsFixedVehicle(void) const
{
	return (m_dwType == GetPmsFixedVehicleTypeValue() ); 
}
bool CTypeCheck::IsPmsTimeSectionChargeRule(void) const
{
	return (m_dwType == GetPmsTimeSectionChargeRuleTypeValue() ); 
}
bool CTypeCheck::IsPmsBagPeriod(void) const
{
	return (m_dwType == GetPmsBagPeriodTypeValue() ); 
}
bool CTypeCheck::IsPmsVehicleGroup(void) const
{
	return (m_dwType == GetPmsVehicleGroupTypeValue() ); 
}

//是否为电子地图服务器
bool CTypeCheck::IsEMU(void) const
{
	return (m_dwType == GetEMUTypeValue() );  
}

//是否为手机客户端
bool CTypeCheck::IsMobile(void) const
{
	return (m_dwType == GetMobileTypeValue() );  
}

/////////是否为1400OUT客户端单元
bool CTypeCheck::Is1400Out(void) const
{
	return (m_dwType == Get1400OUTTypeValue() );  
}

//是否为IE客户端
bool CTypeCheck::IsIEClient(void) const
{
	return (m_dwType == GetIEClientTypeValue() );  
}

//是否为电视墙客户端
bool CTypeCheck::IsTVWClient(void) const
{
	return (m_dwType == GetTVWallClientTypeValue() );  
}

/////////是否区域
bool CTypeCheck::IsArea(void) const
{
	return IsArea_s(m_dwType);// ( m_dwType == GetAreaTypeValue() );  
}

bool CTypeCheck::IsNVMS1000(void) const
{
	return IsNVMS1000_s(m_dwType);// ( m_dwType == GetAreaTypeValue() );  
}

bool CTypeCheck::IsNVMS5000(void) const
{
	return IsNVMS5000_s(m_dwType);// ( m_dwType == GetAreaTypeValue() );  
}

//
bool CTypeCheck::IsUserGroup(void) const
{
	return IsUserGroup_s(m_dwType);
}

//是否是电子地图
bool CTypeCheck::IsEmapFile(void) const
{
	return IsEmapFile_s(m_dwType);
}
//是否是热区
bool CTypeCheck::IsHotArea(void) const
{
	return IsHotArea_s(m_dwType); 
}
//是否是热点
bool CTypeCheck::IsHotSpot(void) const
{
	return IsHotSpot_s(m_dwType); 
}
//是否是时间段帧率模板
bool CTypeCheck::IsTimeSecRate(void) const
{
	return IsTimeSecRate_s(m_dwType); 
}
//是否为分区组
bool CTypeCheck::IsPartitionGroup(void) const
{
	return IsPartitionGroup_s(m_dwType);
}

//是否为分组
bool CTypeCheck::IsPartition(void) const
{
	return IsPartition_s(m_dwType);
}

//是否为系统配置
bool CTypeCheck::IsConfig(void) const
{
	return IsConfig_s(m_dwType);
}
bool CTypeCheck::IsCustom(void) const
{
	return IsCustom_s(m_dwType);
}

bool CTypeCheck::IsWifiProbe(void)const
{
	return (m_dwType == GeWifiProbeTypeValue());
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CTypeCheck::IsInvalidNodeType_s(tuint32 nodeType)
{
	return (nodeType == GetInvalidTypeValue());
}

//存储服务器
bool CTypeCheck::IsMSU_s(tuint32 nodeType)
{
	return (nodeType == GetMSUTypeValue());
}

//GPS存储服务器
bool CTypeCheck::IsGSU_s(tuint32 nodeType)
{
	return (nodeType == GetGSUTypeValue());
}

//媒体转发服务器
bool CTypeCheck::IsMDU_s(tuint32 nodeType)
{
	return (nodeType == GetMDUTypeValue());
}

//独立媒体转发服务器
bool CTypeCheck::IsMediaServer_s(tuint32 nodeType)
{
	return (nodeType == GetMediaServerTypeValue());
}

bool CTypeCheck::IsPCNVR9000_s(tuint32 nodeType)
{
	return (nodeType == GetPCNVR9000TypeValue());
}
//认证服务器
bool CTypeCheck::IsRSU_s(tuint32 nodeType)
{
	return (nodeType == GetRSUTypeValue());
}

//报警服务器
bool CTypeCheck::IsADU_s(tuint32 nodeType)
{
	return (nodeType == GetADUTypeValue());
}

//智能分析服务器
bool CTypeCheck::IsANS_s(tuint32 nodeType)
{
	return (nodeType == GetANSTypeValue());
}

bool CTypeCheck::IsAPPS_s(tuint32 nodetype)
{
	return (nodetype == GetApplicationTypeValue());
}

//接入网关
bool CTypeCheck::IsCGU_s(tuint32 nodeType)
{
	return (nodeType == GetCGUTypeValue());
}

//GPS警情分析服务器
bool CTypeCheck::IsGPSServer_s(tuint32 nodeType)
{
	return (nodeType == GetGPSServerTypeValue());
}

//接入服务器
bool CTypeCheck::IsJSU_s(tuint32 nodeType)
{
	return (nodeType == GetJSUTypeValue());
}

//电视墙服务器
bool CTypeCheck::IsTVW_s(tuint32 nodeType)
{
	return (nodeType == GetTVWTypeValue());
}

bool CTypeCheck::IsSTU_s(tuint32 nodeType)
{
	return (nodeType == GetSTUTypeValue());
}
//墙
bool CTypeCheck::IsWall_s(tuint32 nodeType)
{
	return (nodeType == GetWallTypeValue());
}

//通道组
bool CTypeCheck::IsChannelGroup_s(tuint32 nodeType)
{
	return (nodeType == GetChannelGroupValue());
}

//报警输出组
bool CTypeCheck::IsAlarmOutGroup_s(tuint32 nodeType)
{
	return (nodeType == GetAlarmOutGroupValue());
}

//告警任务
bool CTypeCheck::IsAlarmTask_s(tuint32 nodeType)
{
	return (nodeType == GetAlarmTaskTypeValue());
}
//处警标准操作
bool CTypeCheck::IsSop_s(tuint32 nodeType)
{
	return (nodeType == GetSopValue());
}

//是否为收件人信息
bool CTypeCheck::IsEmail_s(tuint32 nodeType)
{
	return (nodeType == GetEmailTypeValue());
}
//监控客户端
bool CTypeCheck::IsMCU_s(tuint32 nodeType)
{
	return (nodeType == GetMCUTypeValue());
}

//解码器客户端
bool CTypeCheck::IsDCU_s(tuint32 nodeType)
{
	return (nodeType == GetDecoderClientTypeValue());
}

bool CTypeCheck::IsNKB_s(tuint32 nodeType) //网络键盘客户端
{
	return (nodeType == GetNetkeyboardTypeValue());
}
//配置客户端
bool CTypeCheck::IsCCU_s(tuint32 nodeType)
{
	return (nodeType == GetCCUTypeValue());
}

bool CTypeCheck::Is1400Out_s(tuint32 nodeType) //1400Out
{
	return (nodeType == Get1400OUTTypeValue());
}

bool CTypeCheck::Is1400In_s(tuint32 nodeType) //1400In
{
	return (nodeType == Get1400INTypeValue());
}

//前端设备
bool CTypeCheck::IsPU_s(tuint32 nodeType, CPUTypeCheck *pPUTypeCheck)
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsPU(pPUTypeCheck)); //  (nodeType == NODE_TYPE_PERIPHERAL_DEVICE);
}
//解码设备
bool CTypeCheck::IsDecoder_s(tuint32 nodeType, CDecoderTypeCheck *pDecoderTypeCheck )
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsDecoder(pDecoderTypeCheck)); 
}

//报警主机
bool CTypeCheck::IsAlarmHost_s(tuint32 nodeType, CAlarmHostTypeCheck *pAlarmHostTypeCheck )
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsAlarmHost(pAlarmHostTypeCheck)); 
}

bool CTypeCheck::IsAcsSystem_s(tuint32 nodeType, CAcsSystemTypeCheck *pTypeCheck ) //第三方门禁考勤系统
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsAcsSystem(pTypeCheck)); 
}

bool CTypeCheck::IsAlbum_s(tuint32 nodeType)//目标库
{
	CTypeCheck TypeCheck(nodeType);
	return TypeCheck.IsAlbum();
}

bool CTypeCheck::IsCascadePlatform_s(tuint32 nodeType)//级联平台
{
	CTypeCheck TypeCheck(nodeType);
	return TypeCheck.IsCascadePlatform();
}

bool CTypeCheck::IsAlarmIn_s(tuint32 nodeType, CAlarmInTypeCheck *pAlarmInTypeCheck) //报警输入
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsAlarmIn(pAlarmInTypeCheck));

}

bool CTypeCheck::IsAlarmOut_s(tuint32 nodeType, CAlarmOutTypeCheck *pAlarmOutTypeCheck) //报警输出
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsAlarmOut(pAlarmOutTypeCheck)); //  (nodeType == NODE_TYPE_PERIPHERAL_DEVICE);
}

//客户端
bool CTypeCheck::IsClient(tuint32 nodeType)
{
	return ((nodeType >=NODE_TYPE_CLIENT_BEGIN ) && (nodeType <= NODE_TYPE_CLIENT_END));  
}

//IE客户端
bool CTypeCheck::IsIEClient_s(tuint32 nodeType)
{
	return (nodeType == GetIEClientTypeValue());
}
//手机客户端
bool CTypeCheck::IsMobile_s(tuint32 nodeType)
{
	return (nodeType == GetMobileTypeValue());
}

//手机客户端
bool CTypeCheck::IsTVWallClient_s(tuint32 nodeType)
{
	return (nodeType == GetTVWallClientTypeValue());
}

//通道
bool CTypeCheck::IsChannel_s(tuint32 nodeType)
{
	return (nodeType == GetChnnTypeValue());
}

bool CTypeCheck::IsIPChannel_s(tuint32 nodeType)
{
	return (nodeType == GetIPChannelTypeValue());
}

//电子地图服务器
bool CTypeCheck::IsEMU_s(tuint32 nodeType)
{
	return (nodeType == GetEMUTypeValue());
}

//区域
bool CTypeCheck::IsArea_s(tuint32 nodeType)
{
	return (nodeType == GetAreaTypeValue());
}

bool CTypeCheck::IsNVMS1000_s(tuint32 nodeType)
{
	return (nodeType == GetNVMS1000TypeValue());
}

bool CTypeCheck::IsNVMS5000_s(tuint32 nodeType)
{
	return (nodeType == GetNVMS5000TypeValue());
}

bool CTypeCheck::IsUserGroup_s(tuint32 nodeType)
{
	return (nodeType == GetUserGroupTypeValue());
}

bool CTypeCheck::IsSubSystem_s(tuint32 nodeType)//子系统
{
	return (nodeType == GetSubSystemTypeValue());
}
bool CTypeCheck::IsZone_s(tuint32 nodeType)//防区
{
	return (nodeType == GetZoneTypeValue());
}

bool CTypeCheck::IsEmapFile_s(tuint32 nodeType)
{
	return (nodeType == GetEmapAbstractTypeValue());
}
bool CTypeCheck::IsHotArea_s(tuint32 nodeType)
{
	return (nodeType == GetHotAreaTypeValue());
}
bool CTypeCheck::IsHotSpot_s(tuint32 nodeType)
{
	return (nodeType == GetHotSpotTypeValue());
}
bool CTypeCheck::IsTimeSecRate_s(tuint32 nodeType)
{
	return (nodeType == GetTimeSecRateTypeValue());
}

//pms
bool CTypeCheck::IsPmsPark_s(tuint32 nodeType)//停车场
{
	return (nodeType == GetPmsParkTypeValue());
}
bool CTypeCheck::IsPmsGateway_s(tuint32 nodeType)//出入口
{
	return (nodeType == GetPmsGatewayTypeValue());
}
bool CTypeCheck::IsPmsLane_s(tuint32 nodeType)//车道
{
	return (nodeType == GetPmsLaneTypeValue());
}
bool CTypeCheck::IsPmsLed_s(tuint32 nodeType)//显示屏
{
	CTypeCheck TypeCheck(nodeType);
	return  (!!TypeCheck.IsPmsLed()); //  (nodeType == NODE_TYPE_PERIPHERAL_DEVICE);
}
bool CTypeCheck::IsPmsChargeRule_s(tuint32 nodeType)//收费规则
{
	return (nodeType == GetPmsChargeRuleTypeValue());
}
bool CTypeCheck::IsPmsFixedVehicle_s(tuint32 nodeType)//固定车辆
{
	return (nodeType == GetPmsFixedVehicleTypeValue());
}
bool CTypeCheck::IsPmsTimeSectionChargeRule_s(tuint32 nodeType)//自定义收费时间段
{
	return (nodeType == GetPmsTimeSectionChargeRuleTypeValue());
}
bool CTypeCheck::IsPmsBagPeriod_s(tuint32 nodeType)//月卡包月收费
{
	return (nodeType == GetPmsBagPeriodTypeValue());
}

bool CTypeCheck::IsLocalAlarmIn() const
{
    return (m_dwType == GetLocalAlarmInTypeValue());
}
bool CTypeCheck::IsLocalAlarmOut() const
{
    return (m_dwType == GetLocalAlarmOutTypeValue());
}
