
#ifndef _COMMON_CMDPROC_PARAM_
#define _COMMON_CMDPROC_PARAM_

#include "NetCommSDK.h"
#include "ConfigPack_Struct_Def.h"

//公共命令处理对象

/************************************************************************/
/* 日志信息的搜索                                                                */
/************************************************************************/
class CCmdProcParamSearchLog : public CCmdProcParam
{
public:
	CCmdProcParamSearchLog(CCmdProcParameter* pShareData, const void *pCmdData=NULL, tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		memset(&m_LogSearchInfo, 0, sizeof(ECMS_SEARCH_LOG_INFO));
		m_pContentKey = NULL;
		m_pInputSrcKey = NULL;
	}

	ECMS_SEARCH_LOG_INFO  m_LogSearchInfo;
	ECMS_SEARCH_LOG_KEY_CONTENT  *m_pContentKey;
	ECMS_SEARCH_LOG_KEY_INPUT_SRC  *m_pInputSrcKey;

	GUID                            m_DestNode;////////////////////搜索设备日志的时候要用到
};



class CCmdProcParamPTZControl : public CCmdProcParam
{
public:
	CCmdProcParamPTZControl(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_PTZCtrlInfo, 0, sizeof(m_PTZCtrlInfo));
	}

	NET_PTZ_CTRL_INFO  m_PTZCtrlInfo;
	GUID              m_DestNode;			//目标节点
};

class CCmdProcParamPTZName : public CCmdProcParam
{
public:
	CCmdProcParamPTZName(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_PTZCtrlInfo, 0, sizeof(m_PTZCtrlInfo));
	}

	ECMS_PTZ_PRESET_INFO  m_PTZCtrlInfo;
	GUID              m_DestNode;			//目标节点
};

class CCmdProcParamPTZGuardInfo : public CCmdProcParam
{
public:
	CCmdProcParamPTZGuardInfo(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_PTZGuardInfo, 0, sizeof(m_PTZGuardInfo));
	}

	ECMS_PTZ_GUARD_INFO  m_PTZGuardInfo;
	GUID              m_DestNode;			//目标节点
};
//用于同为4GIPC升级
class CCmdProcParamDeviceUpdteInfo : public CCmdProcParam
{
public:
	CCmdProcParamDeviceUpdteInfo(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		//memset(&m_TvtIpcUpdateInfo, 0, sizeof(m_TvtIpcUpdateInfo));
	}

	GUID              m_DestNode;			//目标节点
};

class CCmdProcParamGetTvtIpcUpdteStatus : public CCmdProcParam
{
public:
	CCmdProcParamGetTvtIpcUpdteStatus(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		//memset(&m_TvtIpcUpdateInfo, 0, sizeof(m_TvtIpcUpdateInfo));
	}

	GUID              m_DestNode;			//目标节点
};
//用于同为4GIPC升级检查
class CCmdProcParamTvtIpcUpdteCheck : public CCmdProcParam
{
public:
	CCmdProcParamTvtIpcUpdteCheck(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{

	}

	GUID              m_DestNode;			//目标节点
};

class CCmdProcParam3DPTZControl : public CCmdProcParam
{
public:
	CCmdProcParam3DPTZControl(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_PTZ3DCtrlInfo, 0, sizeof(m_PTZ3DCtrlInfo));
	}

	NET_PTZ_3DCTRL_INFO  m_PTZ3DCtrlInfo;
	GUID                 m_DestNode;	//目标节点
};

class CCmdProcParamColorSet : public CCmdProcParam
{
public:
	CCmdProcParamColorSet(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_ColorInfo, 0, sizeof(m_ColorInfo));
	}

	ECMS_SET_CHANNEL_COLOR  m_ColorInfo;
	GUID				m_DestNode;			//目标节点
};

class CCmdProcParamSetTime : public CCmdProcParam
{
public:
	CCmdProcParamSetTime(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		memset(&m_TimeInfo, 0, sizeof(m_TimeInfo));
	}

	ECMS_SET_TIME		m_TimeInfo;
	GUID				m_DestNode;			//目标节点
};


class CCmdProcParamGetDevCapacity : public CCmdProcParam
{
public:
	CCmdProcParamGetDevCapacity(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}

	GUID				m_DestNode;			//目标节点
};

class CCmdProcParamReqSmartStream : public CCmdProcParam
{
public:
	CCmdProcParamReqSmartStream(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}

	GUID				m_DestNode;			//目标节点
	ECMS_REQ_SUB_SMART_STREAM		m_ReqInfo;
};

class CCmdProcParamSetDevChlAlarminAlarmoutGUID : public CCmdProcParam
{
public:
	CCmdProcParamSetDevChlAlarminAlarmoutGUID(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}
	GUID				m_DestNode;			//目标节点
};

class CCmdProcGetDevUpdateStatu : public CCmdProcParam
{
public:
	CCmdProcGetDevUpdateStatu(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}

	GUID				m_DestNode;			//目标节点
};


class CCmdProcParamHostSetTime : public CCmdProcParam
{
public:
	CCmdProcParamHostSetTime(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_HostTimeInfo, 0, sizeof(m_HostTimeInfo));
	}

	ECMS_HOST_SET_TIME		m_HostTimeInfo;
	GUID				m_DestNode;			//目标节点
};

class CCmdProcParamSetIpInfo : public CCmdProcParam
{
public:
	CCmdProcParamSetIpInfo(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}

	GUID				m_DestNode;			//目标节点
};

class CCmdProcParamSetAlarmOut : public CCmdProcParam
{
public:
	CCmdProcParamSetAlarmOut(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_AlarmOutNO, 0, sizeof(m_AlarmOutNO));
	}

	tuint32				m_AlarmOutNO;
	GUID				m_DestNode;			//目标节点
};

class CCmdProcParamChangeSpeed : public CCmdProcParam
{
public:
	CCmdProcParamChangeSpeed(CCmdProcParameter* pShareData,const void *pCmdData=NULL,tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen){
			m_PlaySpeed = 0;
			m_TaskGuid = GUID_NULL;
			m_DestNode = GUID_NULL;
	}

	tuint32				m_PlaySpeed;	// 播放倍速
	GUID					m_TaskGuid;
	GUID					m_DestNode;		// 目标节点
};

class CCmdProcParamPause : public CCmdProcParam
{
public:
	CCmdProcParamPause(CCmdProcParameter* pShareData,const void *pCmdData=NULL,tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen){
			m_TaskGuid = GUID_NULL;
			m_DestNode = GUID_NULL;
	}
	GUID				m_TaskGuid;
	GUID				m_DestNode;		// 目标节点
};

class CCmdProcParamTalkbackBegin : public CCmdProcParam
{
public:
	CCmdProcParamTalkbackBegin(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_StreamCtrlInfo, 0, sizeof(m_StreamCtrlInfo));
		m_audioGUID = GUID_NULL;
		m_DestNode = GUID_NULL;
	}

	NET_STREAM_CTRL_INFO	m_StreamCtrlInfo;
	GUID				m_DestNode;			//目标节点
	GUID				m_audioGUID;		//联动语音播报GUID
};

class CCmdProcParamTalkbackEnd : public CCmdProcParam
{
public:
	CCmdProcParamTalkbackEnd(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_StreamCtrlInfo, 0, sizeof(m_StreamCtrlInfo));
	}

	NET_STREAM_CTRL_INFO	m_StreamCtrlInfo;
	GUID				m_DestNode;			//目标节点
};


class CCmdProcParamGetPTZConfig : public CCmdProcParam
{
public:
	CCmdProcParamGetPTZConfig(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_PTZGetConfigInfo, 0, sizeof(m_PTZGetConfigInfo));
	}

	NET_GET_PTZ_CONFIG_INFO	m_PTZGetConfigInfo;
	GUID					m_DestNode;			//目标节点
};

class CCmdProcParamGetJPEGPicture : public CCmdProcParam
{
public:
	CCmdProcParamGetJPEGPicture(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_GetJPEGPictureInfo, 0, sizeof(m_GetJPEGPictureInfo));
	}

	NET_GET_JPEG_PICTURE_INFO	m_GetJPEGPictureInfo;
	GUID					m_DestNode;			//目标节点
};

class CCmdProcParamGetNvrAlarmChl : public CCmdProcParam
{
public:
	CCmdProcParamGetNvrAlarmChl(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_GetNvrAlarmChlInfo, 0, sizeof(m_GetNvrAlarmChlInfo));
	}

	NET_GET_NVR_ALARMCHL_INFO	m_GetNvrAlarmChlInfo;
	GUID					m_DestNode;			//目标节点
};
class CCmdProcParamTestLoginDev : public CCmdProcParam
{
public:
	CCmdProcParamTestLoginDev(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_TestLoginDevInfo, 0, sizeof(m_TestLoginDevInfo));
	}

	NET_TEST_LOGIN_DEV_INFO	m_TestLoginDevInfo;
	GUID					m_DestNode;			//目标节点
};

// 强制断开设备网络连接
class CCmdProcParamForceDisconectDev : public CCmdProcParam
{
public:
	CCmdProcParamForceDisconectDev(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
		, m_devGUID(GUID_NULL), m_DestNode(GUID_NULL)
	{
	}

	GUID					m_devGUID;
	GUID					m_DestNode;			//目标节点
};

class CCmdProcParamYITUHeartBeat : public CCmdProcParam
{
public:
	CCmdProcParamYITUHeartBeat(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}

	GUID				m_DestNode;			//目标节点
};

/******************************************************************/
/*     转发服务器，转发服务器设备命令，客户端都在使用它修改时小心 */
/************************************************************************/
class CCmdProcParamLiveStream : public CCmdProcParam
{
public:
	CCmdProcParamLiveStream(CCmdProcParameter* pShareData, const void *pCmdData=NULL, tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData,pCmdData, cmdDataLen)
	{
		memset(&m_StreamCtrlInfo, 0, sizeof(m_StreamCtrlInfo));
		m_DestNode = GUID_NULL;
		m_TaskGUID = GUID_NULL;
	}

	NET_STREAM_CTRL_INFO  m_StreamCtrlInfo;
	GUID                            m_DestNode;
	GUID                                m_TaskGUID;
};

///////声明回放流视音频数据的命令处理
class CCmdProcParamRecEvent : public CCmdProcParam
{
public:
	CCmdProcParamRecEvent(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		m_destNodeID = GUID_NULL;
	}
	REC_DATA_SEARCH           m_DataSearchInfo;
	GUID					m_destNodeID;//存储服务器ID 或者设备节点的ID
};

///////声明标签搜索
class CCmdProcParamRecTagSearch : public CCmdProcParam
{
public:
    CCmdProcParamRecTagSearch(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
    {
        m_destNodeID = GUID_NULL;
    }
    REC_TAG_SEARCH           m_DataTagInfo;
    GUID					m_destNodeID;//存储服务器ID 或者设备节点的ID
};

///////声明标签添加
class CCmdProcParamRecTagAdd : public CCmdProcParam
{
public:
    CCmdProcParamRecTagAdd(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
    {
        m_destNodeID = GUID_NULL;
    }
    REC_TAG_LOG           m_DataTagInfo;
    GUID					m_destNodeID;//存储服务器ID 或者设备节点的ID
};

///////声明标签删除
class CCmdProcParamRecTagDel : public CCmdProcParam
{
public:
    CCmdProcParamRecTagDel(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
    {
        m_destNodeID = GUID_NULL;
    }
    REC_TAG_LOG_DEL         m_DataTagInfo;
    GUID					m_destNodeID;//存储服务器ID 或者设备节点的ID
};

// 图片搜索
class CCmdProcParamPicSearch : public CCmdProcParam
{
public:
    CCmdProcParamPicSearch(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData, pCmdData, cmdDataLen)
    {
        m_destNodeID = GUID_NULL;
    }
    NET_PIC_SEARCH_INFO m_picSearchInfo;
    GUID                m_destNodeID; // 存储服务器ID 或者设备节点的ID
};

// 图片数据
class CCmdProcParamPicData : public CCmdProcParam
{
public:
    CCmdProcParamPicData(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData, pCmdData, cmdDataLen)
    {
        m_destNodeID = GUID_NULL;
    }
    NET_PIC_LOG m_picLog;
    GUID        m_destNodeID; // 存储服务器ID 或者设备节点的ID
};

class CCmdProcParamPlateMatchListSearch : public CCmdProcParam
{
public:
	CCmdProcParamPlateMatchListSearch(CCmdProcParameter* pShareData, const void *pCmdData=NULL, tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		memset(&m_PlateMatchSearch, 0, sizeof(NET_PLATEMATCH_SEARCH));
	}

	NET_PLATEMATCH_SEARCH  m_PlateMatchSearch;

	GUID                            m_destNodeID;////////////////////设备节点的ID
};

class CCmdProcParamPlateMatchPicData : public CCmdProcParam
{
public:
	CCmdProcParamPlateMatchPicData(CCmdProcParameter* pShareData, const void *pCmdData=NULL, tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		memset(&m_PlateMatchImage, 0, sizeof(NET_PLATEMATCH_IMAGE));
	}

	NET_PLATEMATCH_IMAGE  m_PlateMatchImage;

	GUID                            m_destNodeID;////////////////////设备节点的ID
};

// 通行记录图片数据
class CCmdProcParamPmsPassRecordPicData : public CCmdProcParam
{
public:
	CCmdProcParamPmsPassRecordPicData(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		m_destNodeID = GUID_NULL;
	}
	NET_PMS_PASSRECORD_PIC m_picLog;
	GUID        m_destNodeID; // 存储服务器ID 
};

///////声明回放流视音频数据的命令处理
class CCmdProcParamPbStream : public CCmdProcParam
{
public:
	CCmdProcParamPbStream(CCmdProcParameter* pShareData,const void *pCmdData=NULL,tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		m_destNodeID = GUID_NULL;
		m_TaskGuid = GUID_NULL;
		memset(&m_DataSearch, 0, sizeof(m_DataSearch));
	}
	REC_DATA_SEARCH           m_DataSearch;
	GUID					m_destNodeID;//存储服务器ID 或者设备节点的ID
	GUID					m_TaskGuid;
};

/************************************************************************/
/* 文件下载参数信息                                                     */
/************************************************************************/
class CCmdProcParamDownFileInfo : public CCmdProcParam
{
public:
	CCmdProcParamDownFileInfo(BYTE fileType,CCmdProcParameter* pShareData) : \
		CCmdProcParam(pShareData, NULL, 0),m_FileType(fileType)
	{
	}

	BYTE m_FileType;
};

class CCmProcParamTaskComplete : public CCmdProcParam
{
public:
	CCmProcParamTaskComplete(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		m_Errorcode = 0;
		m_DestNode = GUID_NULL;
		m_TaskGUID = GUID_NULL;
		m_RouteGUID = GUID_NULL;
	}
	tuint32							   m_Errorcode;
	GUID                               m_DestNode;
	GUID                               m_TaskGUID;
	GUID                               m_RouteGUID;
};

class CCmProcParamFrameInfo : public CCmdProcParam
{
public:
    CCmProcParamFrameInfo(const GUID &nodeID, const GUID &taskguid,const void *pBuf,tuint32 bufLen, CCmdProcParameter* pShareData,bool autoRelease=true)
        : CCmdProcParam(pShareData,pBuf,bufLen, autoRelease), m_DestNode(nodeID),m_TaskGUID(taskguid)
	{
		m_Errorcode =0;
        //m_DestNode = GUID_NULL;
        //m_TaskGUID = GUID_NULL;
	}
	tuint32							m_Errorcode;
	GUID                            m_DestNode;
	GUID                            m_TaskGUID;
};


/************************************************************************/
/* 文件上传/下载参数信息                                                */
/************************************************************************/
class CCmdProcParamFileInfo : public CCmdProcParam
{
public:
	CCmdProcParamFileInfo(CCmdProcParameter *pShareData) : \
		CCmdProcParam(pShareData, NULL, 0)
	{
		memset(&m_FileInfo, 0, sizeof(m_FileInfo));
		m_DestGUID = GUID_NULL;
		m_TaskGUID = GUID_NULL;
	}

	ECMS_NET_DOWN_UPLOAD_FILE_INFO m_FileInfo;
	GUID m_DestGUID;			//目标结点ID
	GUID m_TaskGUID;			//任务GUID
};

///////声明手动报警输出的命令处理
class CCmdProcParamAlarmOut : public CCmdProcParam
{
public:
	CCmdProcParamAlarmOut(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData,NULL,0)
	{
		m_destNodeID = GUID_NULL;
		m_alarmOutNo = 0;
	}
	tuint32					m_alarmOutNo;
	GUID					m_destNodeID;//存储服务器ID 或者设备节点的ID
};

//透明数据传输
class CCmdProcParamTransport : public CCmdProcParam
{
public:
	CCmdProcParamTransport(CCmdProcParameter* pShareData, const void *pCmdData=NULL, tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		memset(&m_TransportInfo, 0, sizeof(ECMS_TRANSPARENT_TRANSPORT));
	}

	ECMS_TRANSPARENT_TRANSPORT  m_TransportInfo;

	GUID                            m_DestNode;////////////////////搜索设备日志的时候要用到
};

class CCmdProcParamAFControl : public CCmdProcParam
{
public:
    CCmdProcParamAFControl(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
    {
        memset(&m_AFCtrlInfo, 0, sizeof(m_AFCtrlInfo));
    }

    NVMS_AF_CONTROL  m_AFCtrlInfo;
    GUID                 m_DestNode;	//目标节点
};

///////声明电视墙信息获取
class CCmdProcParamMTvwall : public CCmdProcParam
{
public:
	CCmdProcParamMTvwall(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		m_destNodeID = GUID_NULL;
	}
	TVWALL_OPT_INFO     m_DataTvwallInfo;
	GUID				m_destNodeID;  // 目标节点GUID
};

// 声明智能分析服务信息获取
class CCmdProcParamIASrv : public CCmdProcParam
{
public:
    CCmdProcParamIASrv(CCmdProcParameter* pShareData, const void* pCmdData, tuint32 cmdDataLen, bool autoRelease=true)
        : CCmdProcParam(pShareData, pCmdData, cmdDataLen, autoRelease)
    {
        m_destNodeGuid = GUID_NULL;
    }
    ~CCmdProcParamIASrv(){
    };
    IASrv_CMD_INFO  m_cmdDataInfo;
    GUID            m_destNodeGuid;
};


class CCmdProcParamTestSmtp : public CCmdProcParam
{
public:
	CCmdProcParamTestSmtp(CCmdProcParameter* pShareData, const void *pCmdData=NULL, tuint32 cmdDataLen=0)
		: CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		m_destNodeGuid = GUID_NULL;
	}
	~CCmdProcParamTestSmtp(){
	};
	TEST_SMTP_PARA  m_cmdDataInfo;
	GUID            m_destNodeGuid;
};


class CCmdProcParam1400IN : public CCmdProcParam
{
public:
    CCmdProcParam1400IN(CCmdProcParameter* pShareData, const void* pCmdData, tuint32 cmdDataLen, bool autoRelease=true)
        : CCmdProcParam(pShareData, pCmdData, cmdDataLen, autoRelease)
    {
        m_destNodeGuid = GUID_NULL;
    }
    ~CCmdProcParam1400IN(){
    };
    GUID            m_destNodeGuid;
};


class CCmdProcParamReportConfigPort : public CCmdProcParam
{
public:
	CCmdProcParamReportConfigPort(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		memset(&m_ConfigPortInfo, 0, sizeof(m_ConfigPortInfo));
	}

	ECMS_REPORT_CONFIG_PORT		m_ConfigPortInfo;
};

//加密狗时间通知
class CCmdProcParamDogTime : public CCmdProcParam
{
public:
	CCmdProcParamDogTime(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		memset(&m_Info, 0, sizeof(m_Info));
	}

	ECMS_REPORT_DOG_TIME m_Info;
};

//访客超时提醒通知
class CCmdProcParamVisitorTimeout : public CCmdProcParam
{
public:
	CCmdProcParamVisitorTimeout(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		memset(&m_Info, 0, sizeof(m_Info));
	}

	ECMS_REPORT_VISITOR_TIMEOUT m_Info;
};

//发送访客数据到智能
class CCmdProcParamVisitorRecord : public CCmdProcParam
{
public:
    CCmdProcParamVisitorRecord(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
    {
        memset(&m_visRcdInfo, 0, sizeof(m_visRcdInfo));
    }

    CFGPACK_DEFINE::LOG_VISITOR_RECORD m_visRcdInfo;
};

//服务器下线通知
class CCmdProcParamServerOffLine : public CCmdProcParam
{
public:
	CCmdProcParamServerOffLine(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		memset(&m_Info, 0, sizeof(m_Info));
	}

	ECMS_REPORT_SERVER_STATE m_Info;
};

///////声明增加预置点/巡航线
class CCmdProcParamAddPtzConfig : public CCmdProcParam
{
public:
    CCmdProcParamAddPtzConfig(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
    {
        m_guidCh = GUID_NULL;
        m_destNodeID = GUID_NULL;
    }
    GUID					m_destNodeID;//存储服务器ID 或者设备节点的ID
    GUID m_guidCh;
};

// 华安外接道闸_道闸控制_命令处理对象
class CCmdProcParamBarrierControl : public CCmdProcParam
{
public:
	CCmdProcParamBarrierControl(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		m_operateType=0;
	}

	tuint32				 m_operateType;
    GUID                 m_ChnGUID;
	GUID                 m_DestNode;	//目标节点
};

// 华安外接485屏_屏显文字_命令处理对象
class CCmdProcParamSerialLedText : public CCmdProcParam
{
public:
	CCmdProcParamSerialLedText(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_SerialLedTextInfo, 0, sizeof(m_SerialLedTextInfo));
	}

	ECMS_SERIAL_LED_TEXT_PARAM	 m_SerialLedTextInfo;
	GUID m_DestNode;	
};

// 同步下级平台资源命令处理对象
class CCmdProcParamSyncLowerCascadePlatform : public CCmdProcParam
{
public:
	CCmdProcParamSyncLowerCascadePlatform(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
		, m_DestNode(GUID_NULL), m_CGUNode(GUID_NULL)
	{		
	}

	GUID m_DestNode;	
	GUID m_CGUNode;
};

// 红门IP-LED屏_屏显文字_命令处理对象
class CCmdProcParamIPLedText : public CCmdProcParam
{
public:
	CCmdProcParamIPLedText(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_LedTextInfo, 0, sizeof(m_LedTextInfo));
	}

	ECMS_LED_TEXT_PARAM	 m_LedTextInfo;
	GUID m_DestNode;	
};
// 发送语音播报音频文件给转发服务
class CCmdProcParamVoiceBroadcastFileStart : public CCmdProcParam
{
public:
	CCmdProcParamVoiceBroadcastFileStart(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{

	}
	struct st_VoiceStartInfo
	{
		tuint32    dwDataLen; 
		char		szSeriaNum[256];
		st_VoiceStartInfo()
		{
			dwDataLen = 0;
			memset(szSeriaNum, 0, 256);
		}
	}m_audioInfo;
};

class CCmdProcParamVoiceBroadcastFileData : public CCmdProcParam
{
public:
	CCmdProcParamVoiceBroadcastFileData(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}	 
};

class CCmdProcParamVoiceBroadcastFileComplete : public CCmdProcParam
{
public:
	CCmdProcParamVoiceBroadcastFileComplete(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
	}	
};

class CCmdProcParamRequestServerRestartInfo : public CCmdProcParam
{
public:
	CCmdProcParamRequestServerRestartInfo(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		//memset(&m_Info, 0, sizeof(m_Info));
	}

	//ECMS_REPORT_DOG_TIME m_Info;
};

class CCmdProcParamBatchConfig : public CCmdProcParam
{
public:
	CCmdProcParamBatchConfig(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}

	GUID				m_DestNode;			//目标节点
};

class CCmdProcPermiterParm : public CCmdProcParam
{
public:
	CCmdProcPermiterParm(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen,const GUID& chid) : CCmdProcParam(pShareData,pCmdData,cmdDataLen),m_chNodeId(chid)
	{
	}
	GUID				m_DestNode;			//目标节点
	GUID				m_chNodeId;
};

class CCmdProcPrPassPicParm : public CCmdProcParam
{
public:
	CCmdProcPrPassPicParm(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen,const GUID& chid) : CCmdProcParam(pShareData,pCmdData,cmdDataLen),m_chNodeId(chid)
	{
		m_DestNode = GUID_NULL;
		memset(&m_logPassRecord, 0, sizeof(m_logPassRecord));
	}
	GUID				m_DestNode;			//目标节点
	GUID				m_chNodeId;
	CFGPACK_DEFINE::PMS_LOG_PASS_RECORD m_logPassRecord;
};

class CCmdProcParamRequestSysServerTime : public CCmdProcParam
{
public:
	CCmdProcParamRequestSysServerTime(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		//memset(&m_Info, 0, sizeof(m_Info));
	}

	//ECMS_REPORT_DOG_TIME m_Info;
};

class CCmdProcParamSetRSUTime : public CCmdProcParam
{
public:
	CCmdProcParamSetRSUTime(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen)
		: CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		//memset(&m_Info, 0, sizeof(m_Info));
	}
};

class CCmdProcGetCustomProduct : public CCmdProcParam
{
public:
	CCmdProcGetCustomProduct(CCmdProcParameter* pShareData) : CCmdProcParam(pShareData)
	{
		memset(&m_CustomProduct, 0, sizeof(m_CustomProduct));
	}

	CUSTOM_PRODUCT		m_CustomProduct;
	GUID				m_DestNode;			//目标节点
};

class CCmdProcParamSetTvWallData : public CCmdProcParam
{
public:
	CCmdProcParamSetTvWallData(CCmdProcParameter* pShareData, const void *pCmdData, tuint32 cmdDataLen) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
	}

	GUID				m_DestNode;			//目标节点
};

class CCmdProcParamFromApplication:public CCmdProcParam
{
public:
	CCmdProcParamFromApplication(CCmdProcParameter* pShareData, const void* pCmdData, tuint32 cmdDataLen)
		: CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		m_destNodeGuid = GUID_NULL;
	}
	~CCmdProcParamFromApplication(){
	};
	GUID            m_destNodeGuid;
};

/************************************************************************/
/* 文件上传/下载新的参数                                                */
/************************************************************************/
class CCmdProcParamFileInfo_V2 : public CCmdProcParam
{
public:
	CCmdProcParamFileInfo_V2(CCmdProcParameter *pShareData, const void *pCmdData=NULL, tuint32 cmdDataLen=0) : \
		CCmdProcParam(pShareData, pCmdData, cmdDataLen)
	{
		memset(&m_FileInfo, 0, sizeof(m_FileInfo));
		m_DestGUID = GUID_NULL;
		m_TaskGUID = GUID_NULL;
	}

	ECMS_NET_UPLOAD_FILE_INFO_V2 m_FileInfo;
	GUID m_DestGUID;			//目标结点ID
	GUID m_TaskGUID;			//任务GUID
};
//获取升级状态
class CCmdProcParamGetDeviceUpdteStatus : public CCmdProcParam
{
public:
	CCmdProcParamGetDeviceUpdteStatus(CCmdProcParameter* pShareData,const void *pCmdData=NULL, tuint32 cmdDataLen=0) : CCmdProcParam(pShareData,pCmdData,cmdDataLen)
	{
		//memset(&m_TvtIpcUpdateInfo, 0, sizeof(m_TvtIpcUpdateInfo));
	}

	GUID              m_DestNode;			//目标节点
};

#endif //_COMMON_CMDPROC_PARAM_

