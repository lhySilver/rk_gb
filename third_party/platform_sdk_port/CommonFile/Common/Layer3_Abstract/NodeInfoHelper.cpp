#include "NodeInfoHelper.h"
#include "Iterator.h"
#include "TVTXMLParse.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNodeInfoHelper::CNodeInfoHelper(tuint32 dwNodeID)
	:m_bIntiSupportFun(false)
{
	ASSERT(dwNodeID);
	m_dwNodeID = dwNodeID;

	SetAllChannelFlagState(false);
	SetAllDeviceFlagState(false);
}

CNodeInfoHelper::~CNodeInfoHelper(void)
{
}

bool CNodeInfoHelper::SupportAudioCtrl()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bFuncSupportAudio;
}

bool CNodeInfoHelper::SupportPTZCtrl()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bFuncEnablePTZ;
}

bool CNodeInfoHelper::InitChannelSupprotFun()
{
	m_bIntiSupportFun=true; 

	COneNodeIterator oneNodeIter(m_dwNodeID);
	if ( oneNodeIter!=NULL )
	{
		if ( (!oneNodeIter->IsChannel()) && (!oneNodeIter->IsIPChannel()) )
		{
			return false;
		}

		//默认支持，有不支持的再根据获取到的信息判断
		SetAllChannelFlagState(true);

		// 声音报警输出、闪光灯报警输出默认没有
		char pcStreamInfo[CFGPACK_DEFINE::APPEND_INFO_BUF_LENGTH];
		memset(pcStreamInfo, 0, sizeof(pcStreamInfo));
		oneNodeIter->GetNodeAppendInfo(pcStreamInfo,sizeof(pcStreamInfo)); 

		CTVTXMLParse   XMLItemInfo;
		tuint_ptr itemPosPTR_Root = XMLItemInfo.AddValue("Root");
		XMLItemInfo.CloneValue(itemPosPTR_Root, pcStreamInfo);

		std::string strNoFunc;// = XMLItemInfo.GetData();
		XMLItemInfo.GetValueStr(itemPosPTR_Root, "chlNoFunc", strNoFunc);

//		int nLen=strNoFunc.length();
		while(strNoFunc.length() > 0)
		{
			int pos=strNoFunc.find( ":" );
			std::string strItem=strNoFunc.substr(0, pos);
			strNoFunc=strNoFunc.substr(pos+1);

			if ( strItem.compare( "c" ) == 0 )
			{
				m_bSFunCuriseLine=false;
			}
			else if ( strItem.compare("d")==0 )
			{
				m_bSFunImageDisplay=false;
			}
			else if ( strItem.compare( "e" ) ==0 )
			{
				m_bSFunEncodeInfoCfg=false;
			}
			else if ( strItem.compare( "l" ) == 0 )
			{
				m_bSFunVideoLoss=false;
			}
			else if ( strItem.compare( "m" ) == 0 )
			{
				m_bSFunMotion=false;
			}
			else if ( strItem.compare("mc") ==0 )
			{
				m_bSFunMotionConfig=false;
			}
			else if ( strItem.compare( "p" ) == 0 )
			{
				m_bSFunPTZ=false;
			}
			else if ( strItem.compare( "pp" ) == 0 )
			{
				m_bSFunPTZProtocol=false;
			}
			else if ( strItem.compare( "r" ) == 0 )
			{
				m_bSFunRecord=false;
			}
			else if ( strItem.compare( "s" ) ==0 )
			{
				m_bSFunColorRegulate=false;
			}
			else if ( strItem.compare( "t" ) ==0 )
			{
				m_bSFunPTZTrajectory=false;
			}
			else if ( strItem.compare( "w" ) ==0 )
			{
				m_bSFunPTZOther=false;
			}
			else if ( strItem.compare( "z" ) ==0 )
			{
				m_bSFunPresetPoint=false;
			}
			else if ( strItem.compare( "v" ) ==0 )
			{
				m_bSFunStreamToFile=false;
			}
			else if(!strItem.compare("a"))
			{
				m_bFuncSupportAudio = false;
			}
			else if(!strItem.compare("ep"))
			{
				m_bFuncEnablePTZ = false;//
			}
			else if(strItem.compare("pa")==0)
			{
				m_bSFunPEA = false;
			}
			else if(strItem.compare("os")==0)
			{
				m_bSFunOSC = false;
			}
			else if(strItem.compare("av")==0)
			{
				m_bSFunAVD = false;
			}
			else if(strItem.compare("sd")==0)
			{
				m_bSDCard = false;
			}
			else if(strItem.compare("cpc")==0)
			{
				m_bSFunCPC = false;
			}
			else if(strItem.compare("ipd")==0)
			{
				m_bSFunIPD = false;
			}
			else if(strItem.compare("cdd")==0)
			{
				m_bSFunCDD = false;
			}
			else if(strItem.compare("vfd")==0)
			{
				m_bSFunVFD = false;
			}
            else if (strItem.compare("vfdm") == 0)
            {
                m_bSFunVFDMatch = false;
            }
			else if (strItem.compare("fisheye")==0)
			{
				m_bSFunFisheye = false;
			}
			else if (strItem.compare("roi")==0)
			{
				m_bSFunROI = false;
			}
			else if (strItem.compare("az")==0)
			{
				m_bSFunAZ = false;
			}
			else if (strItem.compare("osdmask")==0)
			{
				m_bSFunOsdMask = false;
			}
			else if ( strItem.compare( "chltalk" ) == 0 )
			{
				m_bSFunChlTalkback=false;
			}
			else if (strItem.compare("aao") == 0)
			{
				m_bSFunAudioAlarmOut = false;
			}
			else if (strItem.compare("awlo") == 0)
			{
				m_bSFunWhiteLightAlarmOut = false;
			}
			else if (strItem.compare("vehicle") == 0)
			{
				m_bSFunVehicle = false;
			}
			else if (strItem.compare("tripwire") == 0)
			{
				m_bSFunTripwire = false;
			}
			else if (strItem.compare("perimeter") == 0) {
				m_bSFunPerimeter = false;
			}
            else if (strItem.compare("passlinecount") == 0) {
                m_bSFunPassLine = false;
            }
			else if (strItem.compare("aoie") == 0)
			{
				m_bSFunAoiEntry = false;//进入区域
			}
			else if (strItem.compare("aoil") == 0)
			{
				m_bSFunAoiLeave = false;//离开区域
			}
			else if (strItem.compare("hm") == 0)
			{
				m_bSFunHeatMap = false;//热地图
			}
			else if (strItem.compare("rs") == 0)
			{
				m_bSFunRegionStatistic = false;//区域统计
			}
			else if (strItem.compare("humanTemperature") == 0) 
			{
				m_bSFunHumanTemperatureDetect = false;//体温检测
			}
			else if (strItem.compare("temperaturePad") == 0) 
			{
				m_bSFunTemperaturePad = false;//体温检测
			}
			else if (strItem.compare("ac") == 0)
			{
				m_bSFunAccessControl = false;//门禁控制
			}
			else if (strItem.compare("htex") == 0)
			{
				m_bSFunTemperatureEx = false;//工业热成像测温
			}
			else
			{
				ASSERT(false);
			}
		} 		
	}
	else
	{
		ASSERT(false);
		return false;
	}

	return true;
}

bool CNodeInfoHelper::InitDeviceSupprotFun()
{
	m_bIntiSupportFun=true; 

	COneNodeIterator oneNodeIter(m_dwNodeID);
	if ( oneNodeIter!=NULL )
	{
		if ( (!oneNodeIter->IsPU()) && (!oneNodeIter->IsIPChannel()) )
		{
			return false;
		}

		//默认支持，有不支持的再根据获取到的信息判断
		SetAllDeviceFlagState(true);

		char pcStreamInfo[CFGPACK_DEFINE::APPEND_INFO_BUF_LENGTH];
		memset(pcStreamInfo, 0, sizeof(pcStreamInfo));
		oneNodeIter->GetNodeAppendInfo(pcStreamInfo,sizeof(pcStreamInfo)); 

		CTVTXMLParse   XMLItemInfo;
		tuint_ptr itemPosPTR_Root = XMLItemInfo.AddValue("Root");
		XMLItemInfo.CloneValue(itemPosPTR_Root, pcStreamInfo);

		std::string strNoFunc;// ;
		XMLItemInfo.GetValueStr(itemPosPTR_Root, "devNoFunc", strNoFunc);  

		while(strNoFunc.length() > 0)
		{
			int pos=strNoFunc.find( ":" );
			std::string strItem=strNoFunc.substr(0, pos);
			strNoFunc=strNoFunc.substr(pos+1);

			if( strItem.compare( "3" ) == 0)
			{
				m_bS3G=false;
			}
			else if( strItem.compare( "a" ) == 0)
			{
				m_bSAutoReport=false;
			}
			else if ( strItem.compare( "d" ) == 0 )
			{
				m_bSFunDisk=false;
			}			
			else if ( strItem.compare( "e" ) == 0 )
			{
				m_bSFunAbnormalAlarm=false;
			}
			else if ( strItem.compare( "l" ) == 0 )
			{
				m_bSFunLogQuery=false;
			}
			else if ( strItem.compare( "m" ) == 0 )
			{
				m_bSFunEmail=false;
			}
			else if( strItem.compare( "ma" ) == 0)
			{
				m_bSMobileSensor=false;
			}
			else if( strItem.compare( "mg" ) == 0)
			{
				m_bSMobileAcce=false;
			}
			else if( strItem.compare( "mi" ) == 0)
			{
				m_bSMobileInfo=false;
			}
			else if( strItem.compare( "mm" ) == 0)
			{
				m_bSMobileSMS=false;
			}
			else if( strItem.compare( "ms" ) == 0)
			{
				m_bSMobileSpeed=false;
			}
			else if( strItem.compare( "mt" ) == 0)
			{
				m_bSMobileTemp=false;
			}
			else if ( strItem.compare( "n" ) == 0 )
			{
				m_bSFunDDNS=false;
			}
			else if ( strItem.compare( "o" ) == 0 )
			{
				m_bSFunAlarmOut=false;
			}
			else if (  strItem.compare( "oc" ) == 0 )
			{
				m_bSFunOnlineUsers=false;
			}
			else if( strItem.compare( "r" ) == 0)
			{
				m_bSAutoRestart=false;
			}
			else if ( strItem.compare( "t" ) == 0 )
			{
				m_bSFunTalkback=false;
			}
			else if ( strItem.compare( "u" ) == 0 )
			{
				m_bSFunUserManager=false;
			}
			else if( strItem.compare( "w" ) == 0)
			{
				m_bSWIFI=false;
			}
			else if ( strItem.compare( "s" ) == 0 )
			{
				m_bSFunSensorAlarmIn=false;
			}
			else if ( strItem.compare( "rewind" ) == 0 )
			{
				m_bSFunRecRewind=false;
			}
            else if (strItem.compare("f") == 0)
            {
                m_bSFunFaceMatch = false;
            }
			else if (strItem.compare("lf") == 0)
			{
				m_bSFunLocalFaceMatch = false;
			}
			else if (strItem.compare("pos") == 0)
			{
				m_bSFunPOS = false;
			}
			else if (strItem.compare("ht") == 0)
			{
				m_bSDevTemperatureDect = false;
			}
			else if(strItem.compare("tc") == 0)
			{
				m_bTransparentCom = false;
			}
			else if(strItem.compare("sch") == 0)
			{
				m_bSchedule = false;
			}
			else if(strItem.compare("ftp") == 0)
			{
				m_bFtp = false;
			}
			else if(strItem.compare("nvr") == 0)
			{
				m_isNvr = false;
			}
			
			else
			{
				ASSERT(false);
			}
		}
	}
	else
	{
		ASSERT(false);
		return false;
	}

	return true;
}

void CNodeInfoHelper::SetAllChannelFlagState(bool bState)
{
	m_bFuncSupportAudio=bState;
	m_bFuncEnablePTZ=bState;//
	m_bSFunVideoLoss=bState;
	m_bSFunMotion=bState;
	m_bSFunPTZ=bState;
	m_bSFunPTZProtocol=bState;
	m_bSFunRecord=bState;
	m_bSFunEncodeInfoCfg=bState;
	m_bSFunPresetPoint=bState;
	m_bSFunCuriseLine=bState;
	m_bSFunPTZTrajectory=bState;
	m_bSFunPTZOther=bState;
	m_bSFunColorRegulate=bState;
	m_bSFunStreamToFile=bState;
	m_bSFunImageDisplay=bState;
	m_bSFunMotionConfig=bState;
	m_bSFunFisheye=bState;
	m_bSFunChlTalkback=bState;
	m_bSFunAZ=bState;
	m_bSFunHumanTemperatureDetect = bState;
	m_bSFunTemperatureEx = bState;
	m_bSFunTemperaturePad = bState;
	m_bSFunAccessControl = bState;
	m_bTransparentCom=bState;
	bool isTvtIpc = false;
	bool isN9K = false;
	COneNodeIterator pChNode(m_dwNodeID);
	if(pChNode == NULL) 
	{
		ASSERT(false);
		return;
	}
	if(pChNode->IsChannel())
	{
		tuint32 devID=pChNode->GetDevID();
		COneNodeIterator pDevNode(devID);
		if(pDevNode == NULL)
		{
			ASSERT(false);
			return;
		}
		CTypeCheck::CPUTypeCheck devCheck(pDevNode->GetNodeType());
		if(devCheck.IsTWIPC())
			isTvtIpc = true;
		else if (devCheck.IsN9000DVR() || devCheck.IsN9000NVR())
			isN9K = true;
	}
	else if(pChNode->IsPU())
	{
		CTypeCheck::CPUTypeCheck devCheck(pChNode->GetNodeType());
		if(devCheck.IsTWIPC())
			isTvtIpc = true;
		else if (devCheck.IsN9000DVR() || devCheck.IsN9000NVR())
			isN9K = true;
	}
	
	if(isTvtIpc) //同为ipc才支持智能配置、高级配置
	{
		m_bSDCard=bState;
		m_bSFunCPC=bState;
        m_bSFunVFDMatch = bState;   // 是否支持人脸比对
		m_bSFunROI=bState;
		m_bSFunOsdMask=bState;
		//m_bFuncSupportAudio = bState;
		m_bSFunWhiteLightAlarmOut = bState;
		m_bSFunAudioAlarmOut = bState;
		m_bSFunTripwire = bState;	// 是否支持绊线检测
		//m_bSFunPerimeter = bState;	// 是否支持越界侦测
		m_bSFunHeatMap = bState;//热地图
		m_bSFunRegionStatistic = bState;//区域统计
	}
	else
	{
		m_bSDCard=false;
		m_bSFunCPC=false;
        m_bSFunVFDMatch = false;
		m_bSFunROI=false;
		m_bSFunOsdMask=false;
		// 设备默认支持语音
		//m_bFuncSupportAudio = false;

		// 警戒ipc支持的功能
		m_bSFunWhiteLightAlarmOut = false;
		m_bSFunAudioAlarmOut = false;

		//
		m_bSFunTripwire = false;	// 是否支持绊线检测
		//m_bSFunPerimeter = false;	// 是否支持越界侦测
		m_bSFunHeatMap = false;//热地图
		m_bSFunRegionStatistic = false;//区域统计
	}
	
	if (isN9K || isTvtIpc)
	{
		m_bSFunVFD = bState;//人脸检测
		m_bSFunPassLine = bState;//人数统计
		m_bSFunAoiEntry = bState;//进入区域
		m_bSFunAoiLeave = bState;//离开区域
		m_bSFunPEA = bState;//区域入侵、越界侦测
		m_bSFunOSC = bState;//物品看护
		m_bSFunCDD = bState;//人群密度统计
		m_bSFunAVD = bState;//异常侦测
		m_bSFunVehicle = bState;//车牌识别
		m_bSFunIPD = bState; //人员入侵识别
	}
	else
	{
		m_bSFunVFD = false;//人脸检测
		m_bSFunPassLine = false;//人数统计
		m_bSFunAoiEntry = false;//进入区域
		m_bSFunAoiLeave = false;//离开区域
		m_bSFunPEA = false;//区域入侵、越界侦测
		m_bSFunOSC = false;//物品看护
		m_bSFunCDD = false;//人群密度统计
		m_bSFunAVD = false;//异常侦测
		m_bSFunVehicle = false;//车牌识别
		m_bSFunIPD = false;//人员入侵识别
	}
}

void CNodeInfoHelper::SetAllDeviceFlagState(bool bState)
{
	m_bSFunSensorAlarmIn=bState;
	m_bSFunDisk=bState;
	m_bSFunAlarmOut=bState;
	m_bSFunLogQuery=bState;
	m_bSFunAbnormalAlarm=bState;
	m_bSFunTalkback=bState;
	m_bSFunUserManager=bState;
	m_bSFunDDNS=bState;
	m_bSFunEmail=bState;
	m_bSWIFI=bState;//支持WIFI
	m_bS3G=bState;//支持3G
	m_bSAutoRestart=bState;//支持自动重启
	m_bSAutoReport=bState;//支持主动上报
	m_bSMobileInfo=bState;//支持车载基本信息
	m_bSMobileSensor=bState;//支持车载传感器
	m_bSMobileTemp=bState;//支持车载温度报警
	m_bSMobileSMS=bState;//支持车载短信
	m_bSMobileSpeed=bState;//支持车辆速度报警
	m_bSMobileAcce=bState;//支持车载加速度报警
	m_bSFunOnlineUsers=bState;
	m_bSFunRecRewind=bState;
    m_bSFunFaceMatch=(bool)bsearch;
	m_bSDevTemperatureDect=bState;
	m_bFtp=bState;
	m_isNvr=bState;
	m_bTransparentCom=bState;
	bool isN9K = false;
	COneNodeIterator pDevNode(m_dwNodeID);
	if(pDevNode == NULL)
	{
		return;
	}
	if(pDevNode->IsPU())
	{
		CTypeCheck::CPUTypeCheck devCheck(pDevNode->GetNodeType());
		if (devCheck.IsN9000DVR() || devCheck.IsN9000NVR())
			isN9K = true;
	}
	if (isN9K)
	{
		m_bSFunLocalFaceMatch = bState;
		m_bSFunPOS = bState;
		m_bSchedule=bState;
	}
	else
	{
		m_bSFunLocalFaceMatch = false;
		m_bSFunPOS = false;
		m_bSchedule=false;
	}
}

////////////////////////////////////通道
bool CNodeInfoHelper::SupportMultiChPlay()//是否支持多通道同时打开回放
{
	COneNodeIterator pChNode(m_dwNodeID);
	if(pChNode == NULL) 
	{
		ASSERT(false);
		return false;
	}
	tuint32 devID=pChNode->GetDevID();
	COneNodeIterator pDevNode(devID);
	if(pDevNode == NULL)
	{
		ASSERT(false);
		return false;
	}
	CTypeCheck::CPUTypeCheck devCheck(pDevNode->GetNodeType());
	if(devCheck.IsTWDVR() || devCheck.IsTWMobileDVR()) return true;
	else			return false;
}

bool CNodeInfoHelper::SupportFunVideoLoss()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunVideoLoss;
}

bool CNodeInfoHelper::SupportFunMotion()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunMotion;
}

bool CNodeInfoHelper::SupportFunOSC()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunOSC;
}

bool CNodeInfoHelper::SupportFunAVD()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunAVD;
}

bool CNodeInfoHelper::SupportFunVehicle()
{
    if ( !m_bIntiSupportFun)
    {
        if ( !InitChannelSupprotFun() )
        {
            ASSERT (FALSE);
            return FALSE;
        }
    }

    return m_bSFunVehicle;
}
bool CNodeInfoHelper::SupportFunPEA()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunPEA;
}

bool CNodeInfoHelper::SupportFunPTZ()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunPTZ;
}

bool CNodeInfoHelper::SupportFunPTZProtocol()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunPTZProtocol;
}

bool CNodeInfoHelper::SupportFunRecord()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunRecord;
}

bool CNodeInfoHelper::SupportFunEncodeInfoCfg()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunEncodeInfoCfg;
}

bool CNodeInfoHelper::SupportFunCruiseLine()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunCuriseLine;
}

bool CNodeInfoHelper::SupportFunPresetPoint()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunPresetPoint;
}

bool CNodeInfoHelper::SupportFunPTZTrajectory()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunPTZTrajectory;
}

bool CNodeInfoHelper::SupportFunPTZOther()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunPTZOther;
}

bool CNodeInfoHelper::SuppotrFunColorRegulate()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunColorRegulate;
}

bool CNodeInfoHelper::SupportFunAOIEntry()//进入区域
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunAoiEntry;
}

bool CNodeInfoHelper::SupportFunAOILeave()//离开区域
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunAoiLeave;
}

bool CNodeInfoHelper::SupportFunHeatMap()//热地图
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunHeatMap;
}

bool CNodeInfoHelper::SupportFunRegionStatistic()//区域统计
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunRegionStatistic;
}

bool CNodeInfoHelper::SupportHumanTemperatureDetect()//体温检测
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunHumanTemperatureDetect;
}


bool CNodeInfoHelper::SupportTemperaturePad()//体温检测
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunTemperaturePad;
}

bool CNodeInfoHelper::SupportAccessControl()//支持门禁控制
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunAccessControl;
}

bool CNodeInfoHelper::SupportTemperatureEx()//工业热成像
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunTemperatureEx;
}

///////////////////////////////////////////设备

bool CNodeInfoHelper::SupportFunSensorAlarmIn()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunSensorAlarmIn;
}

bool CNodeInfoHelper::SupportStreamToFile()//是否支持流保存成文件比如远程备份、AVI录像等
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunStreamToFile;
}

bool CNodeInfoHelper::SupportFunDisk()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunDisk;	
}

bool CNodeInfoHelper::SupportFunAlarmOut()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunAlarmOut;
}

bool CNodeInfoHelper::SupportFunLogQuery()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunLogQuery;
}

bool CNodeInfoHelper::SupportFunAbnormalAlarm()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunAbnormalAlarm;
}

bool CNodeInfoHelper::SupportFunTalkback()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunTalkback;
}

bool CNodeInfoHelper::SupportFunChlTalkback()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunChlTalkback;
}

bool CNodeInfoHelper::SupportFunUserManager()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunUserManager;

}

bool CNodeInfoHelper::SupportFunDDNS()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunDDNS;
}

bool CNodeInfoHelper::SupportFunEMail()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunEmail;
}

bool CNodeInfoHelper::SupportFuncWIFI()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSWIFI;
}

bool CNodeInfoHelper::SupportFunc3G()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bS3G;
}

bool CNodeInfoHelper::SupportFuncAutoRestart()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSAutoRestart;
}

bool CNodeInfoHelper::SupportFuncAutoReport()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSAutoReport;
}
bool CNodeInfoHelper::SupportFuncMobileInfo()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSMobileInfo;
}

bool CNodeInfoHelper::SupportFuncMobileSensor()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSMobileSensor;
}

bool CNodeInfoHelper::SupportFuncMobileTemp()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSMobileTemp;
}

bool CNodeInfoHelper::SupportFuncMobileSMS()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSMobileSMS;
}

bool CNodeInfoHelper::SupportFuncMobileSpeed()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSMobileSpeed;
}

bool CNodeInfoHelper::SupportFuncMobileAcce()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSMobileAcce;
}

bool CNodeInfoHelper::SupportFuncOnlineUsers()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunOnlineUsers;
}

//设备是否支持倒放
bool CNodeInfoHelper::SupportFuncRecRewind()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitDeviceSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunRecRewind;
}

bool CNodeInfoHelper::SupportFuncFaceMatch()
{
    if (!m_bIntiSupportFun)
    {
        if (!InitDeviceSupprotFun())
        {
            ASSERT(false);
            return false;
        }
    }

    return m_bSFunFaceMatch;
}

bool CNodeInfoHelper::SupportFunLocalFaceMatch()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitDeviceSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bSFunLocalFaceMatch;
}

bool CNodeInfoHelper::SupportFunPOS()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitDeviceSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bSFunPOS;
}

bool CNodeInfoHelper::SupportFuncAudioAlarmOut()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitChannelSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bSFunAudioAlarmOut;
}

bool CNodeInfoHelper::SupportFuncWhiteLightAlarmOut()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitChannelSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bSFunWhiteLightAlarmOut;
}

bool CNodeInfoHelper::SupportFunImageDisplay()//是否支持流保存成文件比如远程备份、AVI录像等
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunImageDisplay;
}

bool CNodeInfoHelper::SupportFunMotionConfig()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSFunMotionConfig;
}

bool CNodeInfoHelper::SupportSDCard()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (false);
			return false;
		}
	}

	return m_bSDCard;
}

//智能IPC新增人群密度检测
bool CNodeInfoHelper::SupportFunCDD()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunCDD;
}

//人员入侵侦测
bool CNodeInfoHelper::SupportFunIPD()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunIPD;
}

//人数统计
bool CNodeInfoHelper::SupportFunCPC()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

    //return m_bSFunCPC;
    return m_bSFunPassLine;
}


//人脸检测
bool CNodeInfoHelper::SupportFunVFD()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunVFD;
}

//人脸比对（智能服务）
bool CNodeInfoHelper::SupportFunVFDMatch()
{
    if (!m_bIntiSupportFun)
    {
        if (!InitChannelSupprotFun())
        {
            ASSERT(FALSE);
            return FALSE;
        }
    }

    return m_bSFunVFDMatch;
}

bool CNodeInfoHelper::SupportFunFisheye()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunFisheye;
}

bool CNodeInfoHelper::IsSmartSupport()
{
	return SupportFunCDD()||SupportFunIPD()||SupportFunCPC()||SupportFunVFD()||SupportFunVFDMatch()||SupportFunAOIEntry()||SupportFunAOILeave()||SupportFunOSC()||SupportFunAVD()||SupportFunPEA();
}


bool CNodeInfoHelper::SupportAlgorithmSetting()
{
    COneNodeIterator pChNode(m_dwNodeID);
    if(pChNode == NULL) 
    {
        ASSERT(false);
        return false;
    }
    CTypeCheck::CPUTypeCheck devCheck(pChNode->GetNodeType());
    if(devCheck.IsYITU()) return true;
    else			return false;
}

bool CNodeInfoHelper::SupportFunROI()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunROI;
}

//对焦
bool CNodeInfoHelper::SupportFunAZ()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunAZ;
}

//视频遮挡
bool CNodeInfoHelper::SupportFunOsdMask()
{
	if ( !m_bIntiSupportFun)
	{
		if ( !InitChannelSupprotFun() )
		{
			ASSERT (FALSE);
			return FALSE;
		}
	}

	return m_bSFunOsdMask;
}

bool CNodeInfoHelper::SupportFuncHumanTemperature()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitDeviceSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bSDevTemperatureDect;
}

bool CNodeInfoHelper::SupportTransparentCom()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitDeviceSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bTransparentCom;
}

bool CNodeInfoHelper::SupportSchedule()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitDeviceSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bSchedule;
}

bool CNodeInfoHelper::SupportFtp()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitDeviceSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_bFtp;
}

bool CNodeInfoHelper::IsNvr()
{
	if (!m_bIntiSupportFun)
	{
		if (!InitDeviceSupprotFun())
		{
			ASSERT(false);
			return false;
		}
	}

	return m_isNvr;
}

