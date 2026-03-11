/** 
*
*
**/

#ifndef TVT_TA_HEADER__H
#define TVT_TA_HEADER__H

#include "base_type_define.h"

#ifdef __cplusplus // __cplusplus
extern "C" {
#endif

//#ifdef TVT_IA_EXPORTS
//#   define TVT_IA_LIB __declspec(dllexport)
//#else
//#   define TVT_IA_LIB __declspec(dllimport)
//#endif

#ifdef WIN32
    #ifndef TVT_IA_EXPORTS
    #define TVT_IA_LIB __declspec(dllexport)
    #else
    #define TVT_IA_LIB __declspec(dllimport)
    #endif
#else
    #define TVT_IA_LIB  __attribute__ ((visibility("default")))
#endif

// 定义错误码
#define TVT_IA_NORMAL   (0)

//////////////////////////////////////////////////////////////////////////
// 人脸检测结构
typedef void*   TVT_FaceDetectHandle;
typedef void*   TVT_FacialDetectHandle;
typedef void*   TVT_FaceRecognizeHandle;
typedef void*   TVT_AlbumHandle;

// 人脸检测模式 静止、移动
enum TVT_FaceDetectMode{
    TVT_FACE_DETECT_MODE_STILL = 0,
    TVT_FACE_DETECT_MODE_MOVE
};

struct FaceDetectParam{
    TVT_FaceDetectMode eMode;

    tint32  nMinFaceSize;   // 人脸图片的最小尺寸  40
    tint32  nMaxFaceSize;   // 人脸图片的最小尺寸  8192

    tuint32 nRollAngle;     // 人脸图片的旋转角度  0--360;

    tint32  nStillModeThreshold;    // 阈值 0--1000
    tint32  nStillModeDensityCoeff; // 密度系数 20--40
    tint32  nStillModeTimeout;      // ms 0--10,000,000

    tint32  nMoveModeThreshold;
    tint32  nMoveModeDensityCoeff;
    tint32  nMoveModeRefreshCount;  // 更新数量  1--100
    tint32  nMoveModeFaceRetryCount;// 跟踪丢失后的重试次数 0--10
    tint32  nMoveModeHeadRetryCount;// 跟踪丢失后的重试次数 0--10
    tint32  nMoveModeHoldCount;     // 跟踪丢失后的保持数量 0--10
    tint32  nMoveModeModifyRate;    // 更新速率  0--14
    tint32  nMoveModeTimeout;       // ms 0--10,000,000

    FaceDetectParam(){
        eMode = TVT_FACE_DETECT_MODE_STILL;
        nMinFaceSize = 40;
        nMaxFaceSize = 8192;
        nRollAngle = 360;

        nStillModeThreshold = 500;
        nStillModeDensityCoeff = 30;
        nStillModeTimeout = 0;

        nMoveModeThreshold = 500;
        nMoveModeDensityCoeff = 30;
        nMoveModeRefreshCount = 20;
        nMoveModeFaceRetryCount = 2;
        nMoveModeHeadRetryCount = 3;
        nMoveModeHoldCount = 2;
        nMoveModeModifyRate = 4;
        nMoveModeTimeout = 0;
    }
};

// 待检测图像结构，需要转换为灰度图
typedef struct{
    tint32  nWidth;     
    tint32  nHeight;
    tuint8* pGray;  
}GrayFormat;

// 人脸检测结果
typedef struct{
    INT32           nID;            /* ID Number */
    POINT           ptLeftTop;      /* Left-Top     Face Coordinates    */
    POINT           ptRightTop;     /* Right-Top    Face Coordinates    */
    POINT           ptLeftBottom;   /* Left-Bottom  Face Coordinates    */
    POINT           ptRightBottom;  /* Right-Bottom Face Coordinates    */
    INT32           nPose;          /* Face Pose                        */
    INT32           nConfidence;    /* Confidence Degree                */
}FaceDetectResult;

// 人脸识别的结果
typedef struct{
    tint32 nUserID; /* 人脸ID */
    tint32 nScore;  /* 人脸相似度得分 0---1000, 和阈值(标准值500)比较可得到是否为同一个人*/
}FaceRecognizeResult;



#ifdef __cplusplus
}
#endif  // __cplusplus

#endif