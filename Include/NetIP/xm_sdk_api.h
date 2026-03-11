#ifndef __XM_SDK_API_H__
#define __XM_SDK_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ez_sdk_def.h"

//=======================接口定义==================
//SDK 初始化
int	XM_SDK_Init();

//SDK 退出清理
int	XM_SDK_CleanUp();

//获得SDK 的版本信息
int	XM_SDK_GetVersion(char *pVersion);

//获取错误号
int	XM_SDK_GetLastError(char *pErrorInfo,int size);

//设置网络参数
int	XM_SDK_SetConnectParam(int timeout,int retry);

//设置设备断开连接的回调
int	XM_SDK_SetDisconnectCB(EZ_SDK_DisconnectCB disconnectCB, int userData);

//登录
// 0:TCP;	1:UDP ; 2:NAT
int	XM_SDK_Login(char *devIP,int devPort,char *username, char *password,int clientType,int transType,int *error);

//登出
int	XM_SDK_Logout(int loginID);

//订阅报警消息
int	XM_SDK_AttachAlarm(int loginID);

//退订报警消息
int	XM_SDK_DetachAlarm(int loginID);

//设置设备信息的回调函数
int	XM_SDK_SetAlarmCB(EZ_SDK_AlarmCB alarmCB, int userData);

//读取能力集
int	XM_SDK_GetDevAbility(int loginID,EZ_SDK_ABILITY_TYPE type,int channel,char *outBuffer,int bufferSize,int* getSize);

//读设备配置
int	XM_SDK_GetDevConfig(int loginID,EZ_SDK_CONFIG_TYPE type,int channel,char *outBuffer,int bufferSize,int* getSize);

//写设备配置
int	XM_SDK_SetDevConfig(int loginID,EZ_SDK_CONFIG_TYPE type,int channel,char *inBuffer,int bufferSize,int timeout=1000);

//设备信息
int	XM_SDK_QueryDevInfo(int loginID,EZ_SDK_INFO_TYPE type,int channel,char *outBuffer,int bufferSize,int* getSize);

//日志查询	
int	XM_SDK_QueryLog(int loginID,EZ_SDK_LOG_CONDITION condition,EZ_SDK_LOG_INFO *logInfo,int timeout=2000);

//录像查询,  按照文件方式
int 	XM_SDK_QueryRecordByFile(int loginID,EZ_SDK_RECORD_CONDITION condition,EZ_SDK_RECORD_FILE *recordFile,int bufferSize,int *getSize,int timeout=2000);

//录像查询,  按照时间方式
int	XM_SDK_QueryRecordByTime(int loginID,EZ_SDK_RECORD_CONDITION condition, EZ_SDK_RECORD_TIME *recordTime,int timeout=2000);

//设备管理	
int 	XM_SDK_DevProc(int loginID,EZ_SDK_PROC_TYPE type,char *inBuffer,int bufferSize);

// 磁盘管理	
int 	XM_SDK_StorageManage(int loginID, EZ_SDK_STORAGE_MANAGE *storageManage);

//用户管理
int 	XM_SDK_UserManage(int loginID, EZ_SDK_USER_MANAGE *userManage);

//强制I  帧
int 	XM_SDK_ForceIFrame(int loginID,int channel, EZ_SDK_STREAM_TYPE stream);

//云台控制
//param1, param2, param3 根据扩展功能不同意义不同：
//云台方向设置时：param1为水平步长，param2为垂直步长param3为速度
//预置点操作时param1为预置点值
//巡航线路操作时param1为巡航线路值，param2为预置点值(如果有)
int 	XM_SDK_PTZ_Control(int loginID,int channel,EZ_SDK_PTZ_OP_CODE opCode,int param1, int param2, int param3,int stop=0);

//===实时监视===
int 	XM_SDK_StartRealPlay(int loginID,EZ_SDK_REALPLAY_REQ req,EZ_SDK_RealDataCB realplayCB,int userData);

int 	XM_SDK_StopRealPlay(int playHandle);

//===下载和回放===
//下载和回放的区别就是在速度控制上以及是否允许丢包。
int 	XM_SDK_StartPlayBackByFile(int loginID,EZ_SDK_PLAYBACK_REQ_BY_FILE req,EZ_SDK_RealDataCB playbackCB,EZ_SDK_ProgressCB progressCB,int userData);

int 	XM_SDK_StartPlayBackByTime(int loginID,EZ_SDK_PLAYBACK_REQ_BY_TIME req,EZ_SDK_RealDataCB playbackCB,EZ_SDK_ProgressCB progressCB,int userData);

int 	XM_SDK_StopPlayBack(int playHandle);

int 	XM_SDK_CtrlPlayBack(int playHandle,EZ_SDK_PLAYBACK_OP opCode,int param1);

int 	XM_SDK_StartDownloadByFile(int loginID,EZ_SDK_PLAYBACK_REQ_BY_FILE req,EZ_SDK_RealDataCB playbackCB,EZ_SDK_ProgressCB progressCB,int userData);

int 	XM_SDK_StartDownloadByTime(int loginID,EZ_SDK_PLAYBACK_REQ_BY_TIME req,EZ_SDK_RealDataCB playbackCB,EZ_SDK_ProgressCB progressCB,int userData);

int	XM_SDK_StopDownload(int playHandle);

int	XM_SDK_GetDownloadPos(int playHandle);

//===语音对讲===
int 	XM_SDK_StartTalk(int loginID,EZ_SDK_RealDataCB talkCB,int userData);

int 	XM_SDK_StopTalk(int talkHandle);	

int 	XM_SDK_SendTalkData(int talkHandle,char *buffer,int size);//8K16bit PCM

//===透明串口===
int 	XM_SDK_StartTransCom(int loginID,int comNo,EZ_SDK_RealDataCB transComCB,int userData);

int 	XM_SDK_StopTransCom(int loginID,int comNo);

int 	XM_SDK_SendTransComData(int loginID,int comNo,char *buffer,int size);

//设备搜索
int 	XM_SDK_SearchDevice(EZ_SDK_SEARCH_DEVICE *deviceBuf,int bufCount,int *getCount,int timeout);

#ifdef __cplusplus
}
#endif

#endif//__XM_SDK_API_H__

