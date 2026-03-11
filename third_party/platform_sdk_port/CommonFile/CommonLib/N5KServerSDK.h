


///////////////////////////N5KSeverSDK.h

#ifndef _N5K_SERVER_SDK_H_
#define _N5K_SERVER_SDK_H_

#ifdef WIN32 
#ifdef N5K_SERVER_SDK_EXPORT_STATIC//静态库编译
#define _EXPORT_N5K_SERVER_CLASS
#define _EXPORT_N5K_SERVER_SDK
#else 
#ifdef N5K_SERVER_SDK_EXPORT_DLL  //动态库编译
#define _EXPORT_N5K_SERVER_CLASS __declspec(dllexport)
#define _EXPORT_N5K_SERVER_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_N5K_SERVER_CLASS __declspec(dllimport)
#define _EXPORT_N5K_SERVER_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_N5K_SERVER_CLASS __attribute__ ((visibility("default")))
#define _EXPORT_N5K_SERVER_SDK __attribute__ ((visibility("default")))
#endif

#include "LoginStateObserver.h"
#include "Observer.h"
#include "UpdateOperator.h"
#include "StateObservable.h"
#include "PartitionGroupSDK.h"
using namespace std;
#include<string>
// enum NVMS_SERVICE_ERROR_CODE
// {
// 	NVMS_SERVICE_ERROR_SUCCESS=0x0,
// 	NVMS_SERVICE_ERROR_BASE=0x20000000, ////用户错误都需要这个位为1
// };
// 
// 
// enum
// {
// 	NVMS_SERVICE_SUPPORT_ALARMSTATE=0x01,
// 	NVMS_SERVICE_SUPPORT_RECORDSTATE=0x02,
// 	NVMS_SERVICE_SUPPORT_PARTITIONSTATE=0x04,
// };

enum
{
	STATUS_MASK_RANGE_BEGIN,
	STATUS_MASK_RANGE_END,
};

class Interlocked;

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_SupportFunc(char *szXMLBuf, int &nBufLen);//支持的功能

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_Initial(Interlocked *pGlobalNodeIDBase, const char *szExeFilePath/*路径最后带斜杠*/);
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_Quit(); 

//启动后调用
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_Ready(); 
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_UnReady();

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_SetParam(const char *szXMLBuf, int nBufLen);

//启动Ready成功后调用
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_Start();

//手动停止服务时调用
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_Stop();  

//连接上认证服务器后调用，
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_StartNetService();

//与认证服务器断开连接以后调用
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_StopNetService();  


_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_LoginRSU(const char *pXMLBuf,/*要带结束符*/ int nBufLen,/*带结束符的长度*/ CLoginStateObserver *pObServer);
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_ReadyRSU(GUID userSerial);//参数用户序列号，随便给一个唯一值即可
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_ReLoginRSU(CLoginStateObserver *pObServer);

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_AddStatusObserver(CSyncObserver<char *> *pObserverServer, tuint32 statusMask, const char *szUserParam);
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_DelStatusObserver(CSyncObserver<char *> *pObserverServer);

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_IsNeedConnectNode(tuint32 nodeType,tuint32 nodeID);

_EXPORT_N5K_SERVER_SDK tuint32 NVMS_SERVICE_GetLastError(); //返回NVMS_SOCKET_ERROR_CODE里面的值

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_ListenPort_IsValid(const tint32 port); //判断该监听端口是否可用 added by yushulin 20150721

//注册服务器断开导致的停止服务调用
//_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_AuthenticationDisconnectStop();

//注册服务器断开后再次登陆成功后调用
//_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_AuthenticationDisconnectStart(ECMS_LOGIN_SUCCESS_INFO &LoginSuccess);

//注册服务器断开后再次登陆Ready成功后调用
// _EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_AuthenticationDisconnectStart();
// _EXPORT_N5K_SERVER_SDK BOOL NVMS_SERVICE_SaveMemoryData();

// _EXPORT_N5K_SERVER_SDK HANDLE NVMS_SERVICE_ReloginRSU(CLoginStateObserver *pObServer);
// _EXPORT_N5K_SERVER_SDK BOOL NVMS_SERVICE_IsValidAlarm(DWORD NodeID,BYTE  AlarmType);
// _EXPORT_N5K_SERVER_SDK BOOL NVMS_SERVICE_IsImportCfg();//注册服务器导入配置时返回TRUE
// _EXPORT_N5K_SERVER_SDK BOOL NVMS_SERVICE_ImportCfg(CString &errorTip);//注册服务器导入配置时返回TRUE
 // _EXPORT_N5K_SERVER_SDK BYTE NVMS_SERVICE_GetPartDriverLetterByGUID(const GUID &partguid);
// _EXPORT_N5K_SERVER_SDK DWORD NVMS_SERVICE_GetRecordStreamState(DWORD nodeID);
// _EXPORT_N5K_SERVER_SDK int NVMS_SERVICE_GetAllPartitionInfo(PARTITION_INFO *pPartitionInfo, int InNum);
_EXPORT_N5K_SERVER_SDK int	 NVMS_SERVICE_GetAllPartitionInfo(std::string &strXML);
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_ForamtPartition(GUID &partguid);
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_HaveRecordData(GUID &partguid);
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_IsUsingPartition(GUID &partguid);

_EXPORT_N5K_SERVER_SDK GUID NVMS_SERVICE_GetCurrentAppNodeID();
_EXPORT_N5K_SERVER_SDK tuint32 NVMS_SERVICE_GetCurrentAppNodeType();
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_AddConnectStateObserver(CConnectStateObserver *pObServer);
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_DelConnectStateObserver(CConnectStateObserver *pObServer);
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_AddAlarmStateObserver(CAlarmStateObserver *pObServer);
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_DelAlarmStateObserver(CAlarmStateObserver *pObServer);

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_AddNodeUpdateObserver(CUpdateOperator *pUpdateOperator);
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_DelNodeUpdateObserver(CUpdateOperator *pUpdateOperator);
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_IsValidAlarm(GUID NodeGUID,tint32  AlarmType);

_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_AddRecordStateObserver(CRecordStateObserver *pObServer);//添加录像状态观察者 added by yushulin 20150824
_EXPORT_N5K_SERVER_SDK void NVMS_SERVICE_DelRecordStateObserver(CRecordStateObserver *pObServer);//删除录像状态观察者 added by yushulin 20150824
_EXPORT_N5K_SERVER_SDK tuint32 NVMS_SERVICE_GetRecordState(GUID storageID,GUID chID); //获取录像状态 added by yushulin 20150825

_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_GetConnectState(GUID nodeID);
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_AddPartitionGroupObserver(CPartitionGroupObserver *pObserver); 
_EXPORT_N5K_SERVER_SDK bool NVMS_SERVICE_DelPartitionGroupObserver(CPartitionGroupObserver *pObserver);

#endif // _N5K_SERVER_SDK_H_




