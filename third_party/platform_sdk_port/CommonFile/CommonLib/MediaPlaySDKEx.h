#ifndef MEDIAPLAY_SDK_EX_H_ 
#define MEDIAPLAY_SDK_EX_H_
#include "MediaPlaySDK.h"
/////////////////同步回放函数///////////////////
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_CreateAlwaysSyncPlay(tint32 &hSyncPlay);							//播放过程中一直同步

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_CreateBeginSyncPlay(tint32 &hSyncPlay);							//只是开始同步,之后不控制同步

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_OpenSyncStream(tint32 lPort, tint32 lSize, tint32 hSyncPlay);		//打开一直同步流
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_DestroySyncPlay(tint32 hSyncPlay);
////////////////////////////////////////////////

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_Resize(tint32 lPort, int nWndWidth, int nWndHeight);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetAspectRatio(tint32 lPort, int nWidth, int nHeight);		//设置画面长宽比,(0,0):满屏,(1,1):视频比例,(w,h):w比h

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowMotionRect(tint32 lPort, tuint32 nWGridNum, tuint32 nHGridNum, const tuint32 *pGridData);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowRect(tuint32 lPort, int x1Div100, int y1Div100, int x2Div100, int y2Div100, bool isShow);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowRectList(tuint32 lPort, const tuint8* pVFDRectData, tint32 nBufLen);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowLabel(tuint32 lPort, const tuint8* pVFDRectData, tint32 nBufLen);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowSightBead(tuint32 lPort, const tuint8* pVFDRectData, tint32 nBufLen);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowArrow(tuint32 lPort, tuint32 x1, tuint32 y1, tuint32 x2, tuint32 y2, bool isShow, const char* strText, int textLen);

//nColorType  0=红，1=绿，2=蓝, 目前最多允许创建4个矩形
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowBlockRect(tint32 lPort, RECT *szRect, tint32 nCount, tint32 nColorType, bool bFillColor);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowPolygonArea(tint32 lPort, POINT *szPoint, tint32 nCount);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowPolygonAreaMulti(tint32 lPort, const S_MULTI_POINT *szPoint, tint32 nCount);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowTripwireArea(tint32 lPort, TRIPWIRE_AREA_INFO TripwireAreaInfo);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowTripwireAreaMulti(tint32 lPort, const TRIPWIRE_AREA_INFO* tripwireAreaInfoArr, tint32 nCount);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowROIRect(tuint32 lPort, const tuint32*pRectData);

// 只有对共享窗口模式，才需要注册窗口，(即下面两个函数是为共享窗口提供的)
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_RegisterDisplayWnd(HWND hWnd);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ReleaseDisplayWnd(HWND hWnd);

// 下面四个函数都是为共享窗口模式提供的, 其中MEDIA_PLAY_ResizeEx用于独占窗口模式,在共享窗口模式下无需调用
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_PlayEx(tint32 lPort, HWND hWnd, tint32 nRegionIndex);	//开始播放
//MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ResizeEx(tint32 lPort, int x, int y, int nWidth, int nHeight);			//显示区域改变更新
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndResize(HWND hWnd, int nWndWidth, int nWndHeight);					    //显示窗口改变大小
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndRefreshPlay(HWND hWnd);											    //刷新显示

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowMiddleText(tint32 lPort, const char *szText, bool bUnicode = false);				//在画面正中显示文本
//在画面正中显示图片, szFileName = NULL时，取消显示
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowPicture(tint32 lPort, const char *szFileName);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowPictureEx(tint32 lPort, const char *pPicBuf, tuint32 nPicSize);

//在画面左上角显示标题,szText = NULL时，取消显示
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowTopLeftText(tint32 lPort, const char *szText, bool bUnicode = false, float nOffsetX = 0.0, float nOffsetY = 0.0);

//在画面指定位置显示文本列表
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowTextList(tint32 lPort, const char *szText, tint32 nCount, bool bWideByte, POS_INFO &struPosInfo);

//在画面指定矩形区域显示文本，szText = NULL时，取消显示
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowAutoRectText(tint32 lPort, const char *szText, float offsetX, float offsetY, float offsetWidth, float offsetHeight);	

//在画面右下角显示副标题, szText = NULL时，取消显示
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowBottomRightText(tint32 lPort, const char *szText, bool bUnicode = false);

//在画面左下角显示文本, szText = NULL时，取消显示
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowBottomLeftText(tint32 lPort, const char *szText, bool bUnicode = false);

//在窗口底部中间显示文本, szText = NULL时，取消显示
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndShowBottomMiddleText(HWND hWnd, const char *szText, bool bUnicode = false);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndShowTopMiddleStatePicture(HWND hWnd, STATE_PICTURE StatePicture);	//在窗口顶部中间显示状态图标
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndHideTopMiddleStatePicture(HWND hWnd, STATE_PICTURE StatePicture);	//在窗口顶部中间不显示状态图标

// 在窗口上指定位置显示文本, szText = NULL 则清除文本
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndShowTextByIndex(HWND hWnd, tint32 nRegionIndex, const char *szText);

// 清除所有的索引显示文本
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndClearAllIndexText(HWND hWnd);

// 在窗口指定位置显示图片, szFileName = NULL 则清除图片; 
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndShowPictureByIndex(HWND hWnd, tint32 nRegionIndex, const char *szFileName);

// 清除所有的索引图片
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndClearAllIndexPicture(HWND hWnd);

// 清空播放的缓存数据
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ClearBufferData(tint32 lPort);

// 在视频端口上显示文本<字体大小和样式按照osd定制的,目前只在独占窗口模式下实现> 
// nType: 显示类型 ntype=1显示osd名称, nType=2显示osd时间 可以扩展显示更多内容；szText=NULL 表示清除文本
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ShowOSDInfoByCoord(tint32 lPort, tint32 nType, tint32 nX, tint32 nY, const char *szText);
/* 
参数解析:
pSrcBuf: [in],视频的原始帧数据缓冲区
lSrcSize:[in]，原始帧数据缓冲区长度
pBMPBuf: [in,out],解码后的缓冲区，有用户自己申请；
lBufSize:[in], 解码缓冲区长度
lBmpSize:[out],实际需要的解码缓冲区长度
BMPInfoHeader:[out],位图信息说明
ftFrameTime:[out],帧时间
返回值:
true:数据转换成功,或者pBMPBuf = NULL ,或者lBufSize比实际需要解码缓冲区长度小。此时表明pSrcBuf数据是合格的,lBmpSize返回实际需要的解码缓冲区长度
false:pSrcBuf数据源不符合或转换失败。
*/
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_DecodeOneFrameToBMP(const BYTE *pSrcBuf, tuint32 lSrcSize, BYTE *pBMPBuf, tuint32 lBufSize, tuint32 &lBmpSize, BITMAPINFOHEADER &BMPInfoHeader, FILETIME &ftFrameTime);

// 视频导航功能 暂时不实现，用MEDIA_PLAY_SetDisplayRegion函数
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_VideoNavigate(tint32 lPort, NAVIGATE_COMMAND command, HWND hDestWnd);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_VideoRgnMove(tint32 lPort, tint32 nX, tint32 nY);

// 获取视频播放帧时
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetPlayFrameTime(tint32 lPort, FILETIME &frameTime);

// 获取视频原始宽高
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetFrameOriginalSize(tint32 lPort, tint32& width, tint32& height);

//显示选中视频边框, Color:边框颜色 nRegionIndex:边框索引， 当nRegionIndex=-1,不显示边框
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndShowColorFrame(HWND hWnd, tint32 nRegionIndex, const COLORREF Color);

// 显示或隐藏所有的默认边框<非选中的边框>
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_WndShowAllDefaultFrame(HWND hWnd, bool bShow);

// 前一帧
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_OneByOneBackward(tint32 lPort);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_ReversePlay(tint32 lPort);

MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetFourKMode(bool fourK);

// 分割区域 只能在共享模式窗口中使用
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetRegionCellCount(HWND hWnd, int nReginCellCount);  //设置窗口分割画面数量，在注册窗口后调用
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetRegionCellMax(HWND hWnd, int nRegionCellIndex);   //设置某视频画面最大化
//MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SwapFrame(HWND hWnd, int srcIndex, int destIndex);   //交换两个视频画面
MEDIAPLAY_API tint32 STDCALL MEDIA_PLAY_GetIndexByPoint(HWND hWnd, tint32 nX, tint32 nY);  //根据窗口客户区域点获取到视频区域索引
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_GetRegionRectByIndex(HWND hWnd, tint32 nRegionIndex, RECT &rcRegion);  //根据视频区域索引获取视频区域矩形

// 增加一个新的渲染窗口到播放端口（只支持共享窗口的端口，1个共享窗口里面同时只能有1个端口可以增加渲染窗口）
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_AddRenderWndToPort(tint32 lPort, HWND hWnd);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_RemoveRenderWndToPort(tint32 lPort, HWND hWnd);

//设置鱼眼校正模式，进入鱼眼校正模式与退出鱼眼校正模式都需要调用此接口,仅单窗口模式使用
//输入参数：
//lPort： 播放端口
//fishEyeMode：安装模式+校正模式
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_SetFishEyeAdjust(tint32 lPort, FISHEYE_MODE fishEyeMode);

//设置焦点，用于判断当前操作作用于鱼眼校正的哪个分割区域
//输入参数：
//lPort： 播放端口
//nX：当前焦点的X坐标值，相对于当前播放窗口坐标系
//nY：当前焦点的Y坐标值，相对于当前播放窗口坐标系
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_FishEyeAdjustFocus(tint32 lPort, tint32 nX, tint32 nY);
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_FishEyeAdjustFocusEx(tint32 lPort, tint32 nX, tint32 nY, tint32 &nIndex);
//电子云台移动，只有进入鱼眼校正模式下，属于电子云台的分割才可以移动
//输入参数：
//lPort： 播放端口
//nMoveX：鼠标左键拖拽时相对于起点之X坐标轴上的水平偏移量，向右为正，向左为负，以起点为原点
//nMoveY：鼠标左键拖拽时相对于起点之Y坐标轴上的垂直偏移量，向上为正，向下为负，以起点为原点
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_FishEyeAdjustMove(tint32 lPort, tint32 nMoveX, tint32 nMoveY);

//获取当前焦点所在的校正区域位置
//输入参数：
//lPort： 播放端口
//输出参数
//AreaRect：当前焦点所在校正区域的位置，相对于当前播放窗口坐标系
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_FishEyeAdjustGetArea(tint32 lPort, RECT &AreaRect);

//电子云台放大，只有进入鱼眼校正模式下，属于电子云台的分割才可以放大
//输入参数：
//lPort： 播放端口
//ZoomRect： 指定要放大的区域位置信息，相对于当前播放窗口坐标系
MEDIAPLAY_API bool STDCALL MEDIA_PLAY_FishEyeAdjustZoom(tint32 lPort, const RECT &ZoomRect);

#endif //MEDIAPLAY_SDK_EX_H_
