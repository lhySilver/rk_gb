#ifndef _HYBASETYPE_H
#define _HYBASETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef short HINT16;
typedef int HINT32;
typedef unsigned short HUINT16;
typedef char HCHAR;
typedef unsigned HUCHAR;
typedef int HBOOL;
#define HFALSE 0
#define HTRUE 1
#define HY_ErrOpen -1
#define HY_ErrSize -1
#define ERROR_LOG printf

// typedef struct _S_FN_USERINFO
// {
//     int x;
//     int y;
// }S_FN_USERINFO;

//////////////////////////////////////////
typedef struct _SDK_AVPARAM
{
    int nWidth;
    int nHeight;
    int nFps;
    int nGop;
    int nKBps;
    HBOOL bWithAudio;
}S_VIDEO_PARAM;

typedef struct _S_AUDIO_PARAM
{
    int nSampleRate;
    int nKBPS;

}S_AUDIO_PARAM;

typedef struct _SDK_AVPARAM_GET_RES
{
    int nVChnNum;
    S_VIDEO_PARAM sVideParam[2];
    S_AUDIO_PARAM sAudioParam;
}SDK_AVPARAM_GET_RES;
/////////////////////////////////////////////

typedef struct _SDK_SPCDEVPARAM
{
    char chSerialNum[64];
    char chVendor[64];
    char chModel[64];
}SDK_SPCDEVPARAM;

typedef struct _SDK_SPCDEVPARAM_GET_RES
{
    SDK_SPCDEVPARAM stSpecDevParam;
}SDK_SPCDEVPARAM_GET_RES;




#ifdef __cplusplus
}
#endif

#endif
