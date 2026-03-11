#ifndef FTPSDK_H
#define FTPSDK_H

#ifdef WIN32

#ifdef FTP_CLIENT_SDK_EXPORT_API//动态库编译
#define FTP_CLIENT_SDK  __declspec(dllexport)
#define FTP_CLIENT_CLASS  __declspec(dllexport)
#else
#ifdef FTP_CLIENT_SDK_EXPORT_STATIC//静态库编译和使用
#define FTP_CLIENT_SDK
#define FTP_CLIENT_CLASS
#else//使用者编译
#define FTP_CLIENT_SDK  __declspec(dllimport)
#define FTP_CLIENT_CLASS  __declspec(dllimport)
#endif
#endif

#else
#define FTP_CLIENT_SDK __attribute__ ((visibility("default")))
#define FTP_CLIENT_CLASS __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

typedef enum _ftp_conn_status
{
    CONN_OK = 1,                 //连接正常
    CREATE_SOCK_ERR,             //创建SOCKET失败
    CHECK_CONN_ERR,              //检测连接状态失败
    USER_NAME_ERR,               //用户名错误
    PASSWD_ERR,                  //用户密码错误
    SYS_TYPE_ERR,                //系统类型检验失败
    PWD_ERR,                     //路径名称错误
    STOR_ERR,                    //数据端口未开启
    CONN_FAIL,                    //连接错误
    OPTS_FAIL                   //opts 错误
}FTP_CONN_STATUS;

typedef enum _ftp_error_code
{
	FTP_ERROR_OK = 1,                 // 成功
	FTP_ERROR_INVALID_PARAM,          // 无效参数
	FTP_ERROR_DISABLE,                // ftp被禁止
	FTP_ERROR_LOGIN_FAILED,           // 登陆ftp服务器失败
    FTP_ERROR_DATAPORT_EXCEPT,        // ftp操作数据端口异常
}FTP_ERROR_CODE;

typedef enum _video_type
{
	VT_INVAILID,                   // 无效类型
    VT_REAL_TIME_VIDEO,            //实时视频
    VT_PLAY_BACK_VIDEO              //回放视频
}VIDEO_TYPE;


typedef struct _ftp_frame_info
{
    BYTE *pBuf;			//帧数据
    tint32 lSize;		//帧数据长度
    tint32 lIndex;		//帧序号
    bool bIsAudio;		//是否音频帧
}FTP_FRAME_INFO;

typedef void (*FTPWriteCBFunc)(tint32 nHandle, const FTP_FRAME_INFO &FrameInfo, const FILETIME &FrameTime, void *pParam);

FTP_CLIENT_SDK bool FTP_Init(void);
FTP_CLIENT_SDK void FTP_Quit(void);

FTP_CLIENT_SDK bool FTP_SetFtpConfig(const char *szXMLConfig);

FTP_CLIENT_SDK bool FTP_Start(void);
FTP_CLIENT_SDK void FTP_Stop(void);

FTP_CLIENT_SDK tint32 FTP_CreateStreamHandle(const char *szFileName, VIDEO_TYPE videoType = VT_REAL_TIME_VIDEO);
FTP_CLIENT_SDK bool FTP_PutStreamFrame(tint32 nStreamHandle, const char *pData, tuint32 nLen);
FTP_CLIENT_SDK bool FTP_DestroyStreamHandle(tint32 nStreamHandle);
FTP_CLIENT_SDK void FTP_SetStreamCallBack(tint32 nStreamHandle,FTPWriteCBFunc streamCallBack,void *param);

FTP_CLIENT_SDK bool FTP_PutPicFrame(const char *szPicName,const char *pData, tuint32 nLen);
FTP_CLIENT_SDK FTP_ERROR_CODE FTP_SyncPutPicFrame(const char *szPicName,const char *pData, tuint32 nLen);

FTP_CLIENT_SDK FTP_CONN_STATUS FTP_TestConnection(const char *szXMLConfig);

#endif // FTPSDK_H
