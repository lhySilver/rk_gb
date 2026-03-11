#ifndef MEDIAPLAY_SDK_H_ 
#define MEDIAPLAY_SDK_H_

#include "MediaPlaySDKHeader.h"

#ifdef WIN32

#ifdef MEDIAPLAY_EXPORTS			//动态库编译时用此宏定义
#define MEDIAPLAY_API extern "C" __declspec(dllexport)
#else
#ifdef MEDIAPLAY_EXPORTS_STATIC		//静态库编译和使用静态库都用此宏定义
#define MEDIAPLAY_API extern "C"
#else								//动态库使用时不用任何宏定义
#define MEDIAPLAY_API extern "C" __declspec(dllimport)
#endif
#endif
#else
#define MEDIAPLAY_API __attribute__ ((visibility("default")))
#define COLORREF	tuint32
//#define HDC		void *
#endif //WIN32

/**************错误码定义**************/
/**************************************/

/****************宏定义****************/

//流模式
#define STREAM_REALTIME	1			//实时流模式
#define STREAM_FILE		2			//文件流模式
#define STREAM_AUDIO	3			//音频流

//解码缓冲区大小(字节)
#define MAX_SOURCE_BUF		1024*100000	//解码缓冲区最大值
#define MIN_SOURCE_BUF		1024*50		//解码缓冲区最小值

//播放缓冲帧数
#define MAX_DIS_FRAME		50			//播放缓冲区最大缓冲帧数
#define MIN_DIS_FRAME		3			//播放缓冲区最小缓冲帧数

#define MAX_DISPLAY_WND		4			//显示区域最大数量
/**************************************/

typedef enum 
{
	ERRORCODE_UNKNOWN = 1,    // 未知错误
	ERRORCODE_FULL,           // There is not enough space on the disk.
	ERRORCODE_NOT_FOUND,      // The system cannot find the file specified
	ERRORCODE_NOT_READY,      // The device is not ready.
	ERRORCODE_DEV_NOT_EXIST,  // The specified network resource or device is no longer available.
	ERRORCODE_TYPE_E_IOERROR, // I/O Error.
	ERRORCODE_FILE_EXISTS,	  //The file exists.
	ERRORCODE_TOO_MANY_OPEN_FILES,//The system cannot open the file.
	ERRORCODE_ACCESS_DENIED,  //Access is denied.
	ERRORCODE_NEGATIVE_SEEK,  // An attempt was made to move the file pointer before the beginning of the file.
	ERRORCODE_SEEK_ON_DEVICE, //The file pointer cannot be set on the specified device or file.
	ERRORCODE_DELETE_PENDING, //The file cannot be opened because it is in the process of being deleted.
}WRITE_FILE_ERROR_CODE;


/*****************API******************/
MEDIAPLAY_API tuint32 STDCALL MEDIA_PLAY_GetLastError(tint32 lPort);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Initial(void);		//枚举所有显示器,并为每个显示器创建DirectDraw对象
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Quit(void);			//销毁DirectDraw对象相关资源

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetOverloadFlag();
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetDecoderLostFrameCb(DecodeLostFrameCBFun pDecodeLostFrameCBFun);	//解码器丢帧过载回调

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_CreatePort(tint32 &lPort);	//获取一个空闲端口
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_DestroyPort(tint32 lPort);	//释放端口

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetStreamOpenMode(tint32 lPort, tuint32 dwMode);	//设置实时流还是文件流
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetStreamMode(tint32 lPort);			//获取当前流模式

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_OpenStream(tint32 lPort, tint32 lSize);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_CloseStream(tint32 lPort);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_AdjustBufPoolSize(tint32 lPort, tint32 lSize);	//调整解码前存放数据帧缓冲区大小
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_AdjustDISFrameNum(tint32 lPort, tuint32 dwNum);	//调整播放流畅性级别,即解码后缓冲区大小

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetRenderType(tint32 lPort, tint32 iType);	//设置渲染类型

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Play(tint32 lPort, tint32 iRenderType, HWND hWnd);	//开始播放
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Stop(tint32 lPort);				//停止播放
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_InputData(tint32 lPort, const BYTE *pBuf, tint32 lSize);	//视频流输入

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_PlaySound(tint32 lPort);			//独占的方式打开
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_StopSound(void);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetVolume(unsigned short wVolume);		//调整音量,0~100
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetVolume(unsigned short &wVolume);		//获取音量,0~100

//文件播放
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_OpenFile(tint32 lPort, char * sFileName);		//打开文件
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_CloseFile(tint32 lPort);						//关闭文件
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetPlayPos(tint32 lPort, BYTE byRelativePos);	//设置百分比位置(0~100)

//播放控制,回放和播放文件有效
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Pause(tint32 lPort);				//暂停/恢复正常播放
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Fast(tint32 lPort);				//快放
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Slow(tint32 lPort);				//慢放
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_OneByOneForward(tint32 lPort);	//单帧进

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_RefreshPlay(tint32 lPort);		//刷新显示

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_DeleteSDLWindow(tint32 lPort);		//删除SDL_Window，暂时只有macOS使用
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_CreateSDLWindow(tint32 lPort);		//新建SDL_Window，暂时只有macOS使用

//BMP转换
//把解码后的视频数据转换成BMP数据,当pBitmap空或者lBufSize比实际需要的位图尺寸小会失败，此时lBmpSize返回实际需要位图尺寸
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ConvertToBMP(const BYTE *pBuf, tuint32 lSize, const MSDK_FRAME_INFO &FrameInfo, BYTE *pBitmap, tuint32 lBufSize, tuint32 &lBmpSize, BITMAPINFOHEADER &BMPInfoHeader);
//把解码后的视频数据保存为BMP文件
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ConvertToBMPFile(const BYTE *pBuf, tuint32 lSize, const MSDK_FRAME_INFO &FrameInfo, const char *sFileName);

// 解码库截图
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Capture(tint32 lPort, const char *sFileName);

// Set Output Window Width Height
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetOutput(tint32 lPort, tint32 nWidth, tint32 nHeight);

/*参数解析
lPort:[in]播放端口 
pYUVBuf: [in]数据缓存
lBufSize：[in]数据缓存大小
lYUVSize：[out],数据实际大小，不管成功与否，该值都会返回
FrameInfo:[out]，帧信息
返回值:
成功返回true, 失败返回false
建议调用方式：第一次调用，把pYUVBuf填NULL，lBufSize填0，lYUVSize返回数据实际大小；然后再用正确数据调用一次，获取完整的信息
*/
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetPicYUV(tint32 lPort, BYTE *pYUVBuf, tuint32 lBufSize, tuint32 &lYUVSize, MSDK_FRAME_INFO &FrameInfo);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetPicBMP(tint32 lPort, BYTE *pBitmap, tuint32 lBufSize, tuint32 &lBmpSize, BITMAPINFOHEADER &BMPInfoHeader);		//获取当前显示的那帧图像并转换成BMP格式,一般用于暂停后抓取当前帧
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetPicBMPFile(tint32 lPort, const char *sFileName);																//获取当前显示的那帧图像并保存为BMP文件,一般用于暂停后抓取当前帧

//注册回调函数
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetDecodeCBFun(tint32 lPort, DecodeCBFun pDecCBFun, void *pParam);		//MEDIA_PLAY_SetStreamOpenMode后有效
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetDisplayCBFun(tint32 lPort, DisplayCBFun pDisCBFun, void *pParam);		//MEDIA_PLAY_OpenStream后有效
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetDrawFun(tint32 lPort, DrawCBFun pDrawCBFun, void *pParam);				//MEDIA_PLAY_Play后有效
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetFileEndCBFun(tint32 lPort, FileEndCBFun pFileEndCBFun, void *pParam);	//文件播放结束回调
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetResolutionChangeCBFun(tint32 lPort, ResolutionCBFun pCBFun, void *pParam);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetWriteFileCBFun(tint32 lPort, WriteFileCBFun pWriteFileCBFun, void *pParam);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetTLVDataCBFun(tint32 lPort, TLVDataCBFun pTLVDataCBFun, void *pParam);

//视频放大导航 nRegionNum暂时设置0
//bEnable=true 时打开导航视频窗口，hDestWnd是导航窗口句柄， pSrcRect是视频区域(非窗口区域),如果此时pSrcRect=NULL, 则显示全部视频区域;
// bEnable=false 时，关闭导航窗口,pSrcRect 和hDestWnd设置空即可
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetDisplayRegion(tint32 lPort, tuint32 nRegionNum, const RECT *pSrcRect, HWND hDestWnd, bool bEnable);

//获取视频的大小
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetPictureSize(tint32 lPort, tint32 &lWidth, tint32 &lHeight);

//视频备份
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_StartDataRecord(tint32 lPort, const char *szFileName, const long long& szStartTime,int iMaxFileSize, int nDataType, int nFps);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_StopDataRecord(tint32 lPort, void *pOutFilePath = NULL, int *nOutFilePathLen = NULL);

//状态图片显示
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetStatePicture(STATE_PICTURE StatePicture, const char *szFileName);	//为每一个状态添加一张图片

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowStatePicture(tint32 lPort, STATE_PICTURE StatePicture);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_HideStatePicture(tint32 lPort, STATE_PICTURE StatePicture);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowText(tint32 lPort, const char *szText);

//音频抓取
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_StartAudioCapture(tint32 lPort, AudioCaptureDataCBFun pDataCBFun, void *pParam, int nDataType);	//NVMS_DEFINE::AUDIO_FORMAT
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_StopAudioCapture(tint32 lPort);

//音频编码
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_StartAudioEncode(tint32 lPort, int nDataType);	//NVMS_DEFINE::AUDIO_FORMAT
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_StopAudioEncode(tint32 lPort);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_AudioEncode(tint32 lPort, const void *pInBuf, int nInLen, void *pOutBuf, int &nOutLen);

#endif //MEDIAPLAY_SDK_H_
