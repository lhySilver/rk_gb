
/************************************************************************/
/* 带流媒体和存储功能的SDK                                                                     */
/************************************************************************/
///////////////////////////NVRServerSDKEx.h

#ifndef _NVR_SERVER_SDK_EX_H
#define _NVR_SERVER_SDK_EX_H

#include "base_type_define.h"
#include "NVRServerSDK.h"
#include "Observer.h"
#include "Observable.h"
#include "OnlineUserIterator.h"
#include <string>

class CConfigPack;
class CClientCMDObjectBase;
class CHttpProtocolAnalyse;

/////////////////////管理服务器相关 接口
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ReStart();//注册服务器断开后重新登陆前调用
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LoginRSU(char *pRSUIP,tuint16 RSUPort,char *pUsername,char* pUsrPwd,bool bReconnect,CLoginStateObserver *pObServer);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LoginRSU(char *pRSUIP,tuint16 RSUPort,tuint32 userID,bool bReconnect,CLoginStateObserver *pObServer);//用于IE的OCX登陆
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_StopLoginRSU();

/////////通过扩展信息 传送监听的端口
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ReadyRSU(const GUID uniqueIdentifier,char *pExtInfo=NULL,int exInfoLen=0);//

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_IsNeedConnectNode(tuint32 nodeType,tuint32 nodeID);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_IsStarted();//是否已经start

/////////////////////////////////////////////////////////////////////////////////////////////////////////
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_IsFirstRun(std::string &strAdminName);//第一次运行2.0
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_IsFirstRunPMS();//第一次使用PMS
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_UpdateCfgData(CConfigPack* pConfigData, LPVOID para);//////写在数据库中的配置数据   Start后才能够调用
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_UpdateDataToDatabase(CConfigPack* pConfigData, bool isNewProtocolVer = true);//////直接写在数据库中不会更新到其他内存模块 初始化后就可以调用了
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_AddCfgDataUpdateSyncObserver(CSyncObserver<LPVOID> *);//////
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_DelCfgDataUpdateSyncObserver(CSyncObserver<LPVOID> *);//////
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_ClearAllData();////请空数据库中所有的项
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LoadDBVersionInfo(char *pDBVersion, int nINBufLen, int &nValidDataLen);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LoadOneUserInfo(const std::string & userName, char *pUserInfo, int nINBufLen, int &nValidDataLen);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LoadAllData(CConfigPack *pAllConfigData);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LoadHardDiskRelation(GUID &DefaultGroup, CConfigPack &ConfigPack);

//////////////////////////////////////////////////////////////////////////本地用户登录
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_CheckUser(const char *szName, const char *szPSW, GUID &userID,const char *szMac ,tuint32 & nError , const int loginNonce, char* szClientPwd );	 

/////////////////////////////////	
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_StartWebServer(const char *szWebPagePath, tuint16 nWebPort, tuint16 listenPortForWebServer);
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_StopWebServer();
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ResetWebPort(tuint16 nWebPort);

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_registClientCfgCmdProcObject(const char *szHttpCMD,CClientCMDObjectBase* pClientHttpCmdProcObject) ;
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_unRegistAllClientCfgCmdProcObject();

//更新生成的配置XML,配置时,生成XML后调用此接口,更新完了会通知CSyncObserver
/*  std::pair<tuint32, const char *> > 返回值的ID, 带结束符号的XML信息的缓冲区*/
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_configRequest(const CHttpProtocolAnalyse &HTTPObj, CSyncObserver<std::pair<tuint32, const char *> > * pObserver);	

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ListenPortIsValid(const tuint16 port); //added by yushulin 20150721
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_WaitForResult(tuint32 timeout = INFINITE); //added by yushulin 2015.10.17 用于阻塞线程等待
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_NoticeResult(); //added by yushulin 2015.10.17 用于通知阻塞线程，继续往下执行
//上报存储的分区信息到配置服务 配置服务连接上管理时调用
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ReportSysMsuPartStateToConfig();
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_CreateQueryRecord(tuint64 dwLogTypeMask, tuint32 dwStartTime, tuint32 dwEndTime, const GUID &UserGUID);
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_GetRecordTotalCount(tuint32 handle);
//从第startIndex条开始取count条日志，startIndex从1开始
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_LOG_GetRecord(tuint32 handle, tuint32 startIndex, tuint32 count, char *&pBuf);
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_DestroyQueryRecord(tuint32 handle);
_EXPORT_NVRSERVERSDK_API int NVRServer_SDK_LOG_QueryAbnormalRecord(tuint32 dwStartTime, tuint32 dwEndTime, const char *pFilePath);
//删除dwTime这个时间点之前的日志
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_RemoveHistory(tuint32 dwTime);
//导入配置
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ImportConfig(const char* pData,tuint32 nDataLen ,bool bDBFile/*true表示导入导出的是数据库,false表示是内存配置*/);//out配置文件路径
//导出配置
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ExportConfig(std::string &configfileDir ,std::string &configfilePath , std::string & fileName,bool bDBFile/*true表示导入导出的是数据库,false表示是内存配置*/, std::string& strCheckKey);//out配置文件路径
//设置配置服务器的端口，供客户端查询用
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_SetConfigXmlFilePath( std::string strPath);
//获取加密狗信息
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_GetDogTimeInfo(ECMS_REPORT_DOG_TIME& dogInfo);
//导入license
_EXPORT_NVRSERVERSDK_API tuint64 NVRServer_SDK_ImportLicense(const char* pData,tuint32 nDataLen);
//导入license
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_GetServerPort();

//上传语音播报音频文件
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_ImportVoiceBroadcastFile(const char* pData,tuint32 nDataLen);

//通知配置服务器同步内存配置
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_Report_ConfigServer_RefreshCfg();
//设置平台类型
// enum PLATFORM_TYPE
// {
// 	Enterprise_Type  = 0,//企业版
// 	Standard_Type	 = 1,//标准版
// 	Lite_Type = 2, //单机版
// };
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_SetPlatformType(tuint32 platformType);
//获取平台类型
_EXPORT_NVRSERVERSDK_API tuint32 NVRServer_SDK_GetPlatformType();
//获取管理发送过来的重启指令
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_GetRestartCMD();
//发送异常志
_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_LOG_SendExceptionLog(tuint32 dwTime, tuint32 ExceptionType, const char *szContent);
//发送报警
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_SendAlarm(tuint32 alamType, bool bSendEndAlarm, BYTE alarmStat);
//推送存储信息
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_PushStorageInfo(GUID delPartGuid=GUID_NULL);

_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_DisposeBlacklistVehicle(GUID guidBlackGroup);

//推送存储信息
_EXPORT_NVRSERVERSDK_API void NVRServer_SDK_GetSerialNum(std::string &strSerialNum);

_EXPORT_NVRSERVERSDK_API bool NVRServer_SDK_CheckAllDataBaseTables();//////检查所有数据表，防止新版本导入旧版本配置缺少数据段

#endif  //////_NVR_SERVER_SDK_EX_H








