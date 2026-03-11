#ifndef BACKUP_SDK_HEAD
#define BACKUP_SDK_HEAD

#ifdef WIN32

#ifdef BACKUP_SDK_EXPORT_API//动态库编译
#define BACKUP_SDK  __declspec(dllexport)
#define BACKUP_CLASS  __declspec(dllexport)
#else
#ifdef BACKUP_SDK_EXPORT_STATIC//静态库编译和使用
#define BACKUP_SDK
#define BACKUP_CLASS
#else//使用者编译
#define BACKUP_SDK  __declspec(dllimport)
#define BACKUP_CLASS  __declspec(dllimport)
#endif
#endif

#else
#define BACKUP_SDK __attribute__ ((visibility("default")))
#define BACKUP_CLASS __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

typedef struct _backup_frame_info
{
	BYTE *pBuf;	//帧数据
	tint32 lSize;		//帧数据长度
	tint32 lIndex;		//帧序号
	bool bIsAudio;		//是否音频帧
}BACKUP_FRAME_INFO;

typedef void (*BACKUPWriteCBFunc)(tuint32 dwStreamID, const BACKUP_FRAME_INFO &FrameInfo, const FILETIME &FrameTime, void *pParam, tuint32 dwErrCode);

BACKUP_SDK bool BACKUP_Init(void);
BACKUP_SDK void BACKUP_Quit(void);

BACKUP_SDK bool BACKUP_Start(void);
BACKUP_SDK void BACKUP_Stop(void);


BACKUP_SDK tuint32 BACKUP_CreateBackup(const char *szDirPath);				//在此目录下以当前时间创建子文件夹,之后绑定在此句柄下的流都保存在此子文件夹下
BACKUP_SDK void BACKUP_DestroyBackup(tuint32 hBackup);

BACKUP_SDK bool BACKUP_RecordLogInput(tuint32 hBackup, const void *pEventList);	//hBackup为BACKUP_CreateBackup返回的句柄,pEventList为事件列表
BACKUP_SDK bool BACKUP_DetailLogInput(tuint32 hBackup, const void *pEventList); //hBackup为BACKUP_CreateBackup返回的句柄,pEventList为事件列表
BACKUP_SDK bool BACKUP_FileLogInput(tuint32 hBackup, const char *pBuf, tuint32 dwLen);//hBackup为BACKUP_CreateBackup返回的句柄,pBuf:XML缓存，dwLen:缓存长度

BACKUP_SDK bool BACKUP_AttachStream(tuint32 hBackup, tuint32 dwStreamID, const GUID &ChGUID, const char *szChName);		//绑定一个备份流
BACKUP_SDK void BACKUP_DetachStream(tuint32 dwStreamID);

BACKUP_SDK bool BACKUP_BackupStreamInput(tuint32 dwStreamID, const char *pBuf, tuint32 dwLen);

BACKUP_SDK void BACKUP_SetStreamCallBack(tuint32 dwStreamID, BACKUPWriteCBFunc CBFunc, void *pParam);

#endif // BACKUP_SDK_HEAD
