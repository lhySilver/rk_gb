#ifndef __DEV_INTERFACE_H__
#define __DEV_INTERFACE_H__

#include "DevDefine.h"
#include <string>

/*
<?xml version="1.0" encoding="UTF-8"?>
<userinfo>
<username></username>
<password></password>
<ip></ip>
</userinfo>
*/
class CDevInterface
{
public:
	virtual ~CDevInterface(){};

	/*
	*  该接口用来网络库不能够处理的http命令
	*/
	virtual tint32 WebProcExt( const std::string& strWebContent, NETNS::HTTP_STATUS_CODE& status, std::string& strWebReponse) = 0;
	// @param pszRespXml由类分配内存
	virtual tint32 NetProcExt( const char* pContent, tint32 dwLen, char *& pszRespXml, tint32& nRespLen, NETNS::HTTP_STATUS_CODE& status) =0;

	virtual void  ParseFailedResource( const char* pUrl, char*& pResourceExt, tuint32 &dwRetExtLen) = 0;

	virtual void DelResponseBuff(char *&pszRespXml) = 0;
	
	/*
	* support function
	*/
	virtual NETNS::DEV_RET SupportIntegratedPTZ( bool &bSupport) = 0;
	virtual NETNS::DEV_RET SupportRS485PTZ( bool &bSupport) = 0;
	virtual NETNS::DEV_RET SupportSDCard( bool &bSupport) = 0;

	/*
	* system
	*/
	virtual NETNS::DEV_RET GetUserInfo( NETNS::DEV_USER_INFO *pCfg, tuint32 nSize, tuint32& nNeedSize) = 0;
	virtual NETNS::DEV_RET GetDeviceInfo( const char* pUser, NETNS::DEV_INFO& info) = 0;
	virtual NETNS::DEV_RET GetSystemTime( const char* pUser, NETNS::DEV_SYSTIME& sysTime) = 0;
	virtual NETNS::DEV_RET SetSystemTime( const char* pUser, const NETNS::DEV_SYSTIME& sysTime) = 0;

	virtual NETNS::DEV_RET SetCurTime( const char* pUser, const NETNS::DEV_DATETIME& datetime) = 0;

	/*
	* Stream Capabilities
	*/
	// camara
	virtual NETNS::DEV_RET GetImageCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM_CAPS &caps) = 0;
	virtual NETNS::DEV_RET GetImageParam( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM &param) = 0;
	virtual NETNS::DEV_RET SetImageParam( const char* pUser, tuint32 nChnID, NETNS::DEV_CAMERA_PARAM &param) = 0;
	virtual NETNS::DEV_RET GetSnapshot( const char* pUser, tuint32 nChnID, char*& pPic, tuint32& nPicSize) = 0;
	 

	// stream
	virtual NETNS::DEV_RET GetStreamCaps( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, NETNS::DEV_SREAM_CAPS& caps) = 0;
	virtual NETNS::DEV_RET GetStreamName( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, char* pStreamName, tuint32 nNameLen, tuint32& nNeedNameLen) = 0;
	virtual tuint32 GetStreamCnt( tuint32 nChnID ) = 0;
	virtual tint32 SupportAudioEncode( tuint32 nChnID, NETNS::DEV_AUDIO_ENCODE encode) = 0;
	virtual NETNS::DEV_RET SetAudioEncode(  const char* pUser, tuint32 nChnID, const NETNS::DEV_AUDIO_ENCODE encode) = 0;
	virtual NETNS::DEV_RET GetVideoStream( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, NETNS::DEV_SREAM_CFG &cfg) = 0;
	virtual NETNS::DEV_RET SetVideoStream( const char* pUser, tuint32 nChnID, tuint32 nStreamIdx, const NETNS::DEV_SREAM_CFG& cfg) = 0;
	virtual NETNS::DEV_RET RequestKeyFrame( const char* pUser, tuint32 nChnID) = 0;
	virtual bool RtspNeedAuthorization() = 0;

	// OSD
	virtual NETNS::DEV_RET GetOsdTimestamp( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_TIMESTAMP& cfg) = 0;
	virtual NETNS::DEV_RET GetOsdChnName( const char* pUser, tuint32 nChnID,  NETNS::DEV_OSD_DEVICE_NAME& cfg) = 0;
	virtual NETNS::DEV_RET SetOsdTimestamp( const char* pUser,  tuint32 nChnID, const NETNS::DEV_OSD_TIMESTAMP cfg) = 0;
	virtual NETNS::DEV_RET SetOsdChnName( const char* pUser, tuint32 nChnID,  const NETNS::DEV_OSD_DEVICE_NAME cfg) = 0;

	// privacy mask
	virtual NETNS::DEV_RET GetPrivacyMaskCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_PRIVACY_MASK* pCfg, tuint32 nSize, tuint32& nNeedSize) = 0;
	virtual NETNS::DEV_RET SetPrivacyMaskCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_OSD_PRIVACY_MASK* pCfg, tuint32 nSize) = 0;

	/*
	* protocol
	*/
	virtual NETNS::DEV_RET PtzGetCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CAPS& cfg) = 0;
	virtual NETNS::DEV_RET PtzGetCaps( const char* pUser, tuint32 nChnID, NETNS::DEV_DOM_PTZ_CAPS& cfg) = 0;
	virtual NETNS::DEV_RET PtzGetProtocolCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_PROTO& cfg) = 0;
	virtual NETNS::DEV_RET PtzSetProtocolCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_PTZ_PROTO& cfg) = 0;

	// ptz ctrol
	virtual NETNS::DEV_RET PtzCtrl( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CTRL_TYPE action, tuint32 nSpeed) = 0;

	// preset
	virtual NETNS::DEV_RET PtzGotoPreset( const char* pUser, tuint32 nChnID, tuint32 nPresetID) = 0;
	virtual NETNS::DEV_RET PtzGetPresets( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_PRESET* pPresets, tuint32 nNum, tuint32& nNeedSize) = 0;
	virtual tuint32 PtzGetPresetsMaxNum( tuint32 nChnID ) = 0;
	virtual NETNS::DEV_RET PtzAddPreset( const char* pUser, tuint32 nChnID, const char* pName) = 0;
	virtual NETNS::DEV_RET PtzModifyPresetName( const char* pUser, tuint32 nChnID, tuint32 nPresetID, const char* pName) = 0;
	virtual NETNS::DEV_RET PtzDeletePreset( const char* pUser, tuint32 nChnID, tuint32 nPresetID) = 0;
	virtual NETNS::DEV_RET PtzModifyPresetPosition( const char* pUser, tuint32 nChnID, tuint32 nPresetID) = 0;

	// cruise
	virtual NETNS::DEV_RET PtzGetCruises( const char* pUser, tuint32 nChnID, NETNS::DEV_PTZ_CRUISE* pCruises, tuint32 nSize, tuint32& nNeedSize) = 0;
	virtual NETNS::DEV_RET PtzGetCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID, NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize, tuint32& nNeedSize) = 0;
	virtual NETNS::DEV_RET PtzAddCruise( const char* pUser, tuint32 nChnID, const char* pCruiseName,  NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize) = 0;
	virtual NETNS::DEV_RET PtzModifyCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID, const char* pCruiseName,  NETNS::DEV_PTZ_PRESET* pPreset, tuint32 nSize ) = 0;
	virtual NETNS::DEV_RET PtzDeleteCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID) = 0;
	virtual NETNS::DEV_RET PtzRunCruise( const char* pUser, tuint32 nChnID, tuint32 nCruiseID) = 0;
	virtual NETNS::DEV_RET PtzStopCruise( const char* pUser, tuint32 nChnID) = 0;

	/*
	* Motion Detection
	*/
	virtual NETNS::DEV_RET  GetMotionCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_MOTION_CFG& cfg) = 0;
	virtual NETNS::DEV_RET  GetMotionSensitivity( const char* pUser, tuint32 nChnID, NETNS::DEV_RANGE& sen) = 0;
	virtual NETNS::DEV_RET  SetMotionCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_MOTION_CFG &cfg) = 0; 

	/*
	* Alarm
	*/
	virtual NETNS::DEV_RET GetAlarmInCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_SENSOR_CFG &cfg) = 0;
	virtual NETNS::DEV_RET SetAlarmInCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_SENSOR_CFG &cfg) = 0;
	virtual NETNS::DEV_RET ManualAlarmOut( const char* pUser, tuint32 nChnID, bool bStatus) = 0;
	virtual NETNS::DEV_RET GetAlarmoutCfg( const char* pUser, tuint32 nChnID, NETNS::DEV_ALARMOUT_CFG& cfg ) = 0;
	virtual NETNS::DEV_RET SetAlarmoutCfg( const char* pUser, tuint32 nChnID, const NETNS::DEV_ALARMOUT_CFG &Cfg) = 0;
	virtual NETNS::DEV_RET GetAlarmStatus( const char* pUser, char*& pOutAlarmXml, tint32& nAlarmXmlLen) = 0;

	/*
	* TCP/IPV4
	*/
	virtual NETNS::DEV_RET GetNetBasicCfg( const char* pUser, NETNS::DEV_IPV4_CFG& cfg) = 0;
	virtual NETNS::DEV_RET SetNetBasicCfg( const char* pUser, const NETNS::DEV_IPV4_CFG& cfg) = 0;
	virtual NETNS::DEV_RET GetNetPppoeConfig( const char* pUser, NETNS::DEV_PPPOE_CFG& cfg) = 0;
	virtual NETNS::DEV_RET SetNetPppoeConfig( const char* pUser, const NETNS::DEV_PPPOE_CFG& cfg) = 0;

	virtual NETNS::DEV_RET GetPortCfg( const char* pUser, NETNS::DEV_NET_PORT& cfg) = 0;
	virtual NETNS::DEV_RET SetPortCfg( const char* pUser, const NETNS::DEV_NET_PORT& cfg) = 0;
	virtual NETNS::DEV_RET GetDdnsCfg( const char* pUser, NETNS::DEV_DDNS_CFG& cfg) = 0;
	virtual NETNS::DEV_RET SetDdnsCfg( const char* pUser, const NETNS::DEV_DDNS_CFG& cfg) = 0;

	/*
	* User Management
	*/
	virtual NETNS::DEV_RET ModifyPassword( const char* pUser, const char* pNewPassword ) = 0;
	/*
	*
	*/
	virtual NETNS::DEV_RET RebootDevice( const char* pUser) = 0;
	virtual NETNS::DEV_RET GetChnList( const char* pUser, tuint32* pChnID, tuint32 dwChnSize, tuint32& dwRetNeedSize) = 0;


	/*
	*   Playback Management
	*/
	virtual NETNS::DEV_RET RecSearchByDay( const char* pUser, NETNS::DEV_DATE* pDateList, tuint32 dwListSize, tuint32& dwRetNeedSize) = 0;
	virtual NETNS::DEV_RET RecSearchByTime( const char* pUser, const NETNS::DEV_DATETIME& searchStart, const NETNS::DEV_DATETIME& searchStop
			, NETNS::DEV_REC_SEARCH* pRecList, tuint32 dwListSize, tuint32& dwRetNeedSize) = 0;

	/*
	*  smart interface                                                                    
	*/
	virtual tuint32 GetDevAttriSaveLen( NETNS::DEV_ATTRIBUTE_LEN type) = 0;

};


#endif