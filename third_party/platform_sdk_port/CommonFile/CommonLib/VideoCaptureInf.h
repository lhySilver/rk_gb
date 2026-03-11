#ifndef VIDEO_CAPTURE__INTERFACE__H
#define VIDEO_CAPTURE__INTERFACE__H

#include "base_type_define.h"
#include "N_SDKCommonObserver.h"
#include "StreamObservable.h"

enum VideoCaptureInfEnum{VFD_TOPIC = 0};

#ifdef __cplusplus // __cplusplus
extern "C" {
#endif


#ifdef WIN32
#ifdef VIDEO_CAPTURE_EXPORTS//动态库编译
#define IA_VIDEO_CAPTURE_LIB __declspec(dllexport)
#else//使用者编译
#define IA_VIDEO_CAPTURE_LIB __declspec(dllimport)
#endif
#define CALLMETHOD __stdcall
#else
#define IA_VIDEO_CAPTURE_LIB  __attribute__ ((visibility("default")))
#define CALLMETHOD
#endif


	// @Brief: 初始化， 反初始化
	// @Param void* @ref CAppNetDataCtrl* 
    IA_VIDEO_CAPTURE_LIB void CALLMETHOD video_capture_initialize(void* netCtrl);
    IA_VIDEO_CAPTURE_LIB void CALLMETHOD video_capture_deinitialize();

	// @Brief 添加、移除事件观察者，
	// @Param CWaitObserver： 订阅、退订事件观察者， CWaitObserver::OnWaitForResult()订阅、退订是否成功状态
	// @Param CSmartEventDataObserver: 检测到人脸图片之后，人脸抓图观察者， 
	//     CSmartEventDataObserver::EventDataCapture 获取到人脸抓图之后的推送
	//     CSmartEventDataObserver::EventDataComplete useless
    IA_VIDEO_CAPTURE_LIB void CALLMETHOD video_capture_addObserver(CWaitObserver* topicObserver, CSmartEventDataObserver* dataObserver);
    IA_VIDEO_CAPTURE_LIB void CALLMETHOD video_capture_removeObserver(CWaitObserver* topicObserver, CSmartEventDataObserver* dataObserver);

	// @Brief：订阅、退订，某IPC的事件 
	// @Param：GUID， IPC的通道GUID
	// @Param：tuint32， 事件类型, @enum VideoCaptureInfEnum
    IA_VIDEO_CAPTURE_LIB bool CALLMETHOD video_capture_subTopic(const GUID& chnlGuid, const tuint32 topicType = VFD_TOPIC);
    IA_VIDEO_CAPTURE_LIB bool CALLMETHOD video_capture_unsubTopic(const GUID& chnlGuid, const tuint32 topicType = VFD_TOPIC);

	// @Brief: 查看当前已经订阅事件的数量
	// @Return: 返回数量
	IA_VIDEO_CAPTURE_LIB tuint32 CALLMETHOD video_capture_get_topic_count();
#ifdef __cplusplus // __cplusplus
};
#endif

#endif
