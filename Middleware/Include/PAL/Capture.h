
#ifndef __PAL_CAPTURE_H__
#define __PAL_CAPTURE_H__

#include "Types.h"
#include "PAL/MotionDetect.h"
#include "PAL/libdmc.h"

#ifdef __cplusplus
extern "C" {
#endif

//图像旋转属
typedef enum ROTATE_ATTR_e
{
	RA_NONE = 0,
	RA_90,
	RA_180,
	RA_270,
	RA_BUTT
}RotateAttr_t;

//检测目标类型
typedef enum {
    DETECT_OBJECT_TYPE_NONE = 0,
    DETECT_OBJECT_OBJECT_TYPE_PERSON = 1,
    DETECT_OBJECT_OBJECT_TYPE_VEHICLE = 2,
    DETECT_OBJECT_OBJECT_TYPE_NON_VEHICLE = 3,
    DETECT_OBJECT_OBJECT_TYPE_MAX
} DETECT_OBJECT_TYPE;

//检测目标点
typedef struct tagDETECT_POINT{
    unsigned int x; /*万分之一*/
    unsigned int y; /*万分之一*/
} DETECT_POINT;

//检测目标点坐标
typedef struct {
    DETECT_POINT topLeft;     /*左上*/
    DETECT_POINT bottomRight; /*右下*/
} DETECT_RECT;

//检测结果
typedef struct tagDETECT_RESULT
{
	int ObjectType; //DETECT_OBJECT_TYPE
	int Status;
	DETECT_RECT Rect;
} DETECT_RESULT;

typedef void (* CaptureDetectCallback)(int status, DETECT_RESULT result);

typedef struct tagDETECT_ATTR
{
	int ObjectType;
	int Level;
	CaptureDetectCallback Callback;
} DETECT_ATTR;

typedef struct tagREGIONS_ATTR
{
	int Num;
	unsigned int RegionAttr[4];
} REGION_ATTR;

typedef struct tagDETECT_INIT
{
	RotateAttr_t Rotate;
	int Level;//1-100
	int RegionEnable;
	REGION_ATTR Region;
} DETECT_INIT;


//移动追踪检测信息
typedef struct 
{
    int ObjectType; //DETECT_OBJECT_TYPE
	DETECT_RECT Rect;
} MOTION_TRACKER_INFO;

//移动追踪检测结果
typedef struct 
{
	int Status;                       /* 为0数据有效 */
    int ObjNum;                       /* 目标个数 */
    MOTION_TRACKER_INFO ObjInfo[128]; /* 各目标检测信息 */
} MOTION_TRACKER_RESULT;

typedef void (* CaptureMotionTrackerCallback)(MOTION_TRACKER_RESULT result);


int AVSetLogLevel(int level);
int AvInit(float sd,int ispmode);
int AvRelease();
int CaptureGetChannels(void);
int CaptureSetStreamCallBack(char *module_name, int media_type, dmc_media_input_fn proc);
int CaptureCreate(int channel);
int CaptureDestroy(int channel);
int CaptureStart(int  channel, unsigned int dwType);
int CaptureStop(int  channel, unsigned int dwType);

int CaptureForceIFrame(int  channel, unsigned int dwType);
int CaptureSetISPMode(int iMode);
int CaptureSetBitRate(int Channel, int iTargetBitRate);
int CaptureSetRotate(int enRotate);
int CaptureSetfps(int fps);
int CaptureGetResolution(int stream_id, int *pWith,int *pHeight);

unsigned int CaptureGetEncodeFrameCount(int Channel);
int CaptureSnapshotGetBuffer(char *buffer, int size, int timeout_s);

int CaptureSetWdr(int onoff);
int CaptureSetOSDSwitch(int onoff);
int CaptureSetAntiFlicker(int antiflicker);

int CaptureSetSaturation();
int CaptureSetContrast();
int CaptureSetSharpness();
int CaptureSetPwrFrequency();

int CaptureSetEptz(int scale); //eptz

int CaptureDetectInit(DETECT_INIT *pAttr);
int CaptureDetectDeInit();
int CaptureDetectStart();
int CaptureDetectStop();
int CaptureDetectSet(DETECT_INIT *pAttr);
int CaptureDetectGet(DETECT_INIT *pAttr);
int CaptureDetectObjectStart(DETECT_ATTR *pAttr);
int CaptureDetectObjectStop(int ObjectType);

int CaptureGetMeanLuma(float *value);

int CaptureMotionTrackerStart(CaptureMotionTrackerCallback cb);
int CaptureMotionTrackerStop();

#ifdef __cplusplus
}
#endif

#endif

