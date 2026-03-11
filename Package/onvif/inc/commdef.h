#ifndef   _commdef_header__
#define   _commdef_header__
#include <hyBaseType.h>
#include "xml_get_data.h"
////#include "funUserManager.h"



#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NVT_deviceInfo_{
    char FirmwareVersion[64];
    char HardwareId[64];
    char Manufacturer[64];
    char Model[64];
    char SerialNumber[64];
}NVT_deviceInfo, pNVT_deviceInfo;

#define MAX_ONLINE_USER  4
#define MEDIA_URL_LEN	 128

int getLocalIp(HBOOL flag, char* ipaddr);
int GetMacAddress(char* macaddress,const char* localip);
int getRtspPort(HBOOL flag);
int GenRtspToken(char* chToken,int nLen);
int GetHttpPort(HBOOL flag);
int getOnvifPort(HBOOL flag);

////int getUserinfo(HBOOL flag,S_FN_USERINFO **pUserInfos);
////int addUserinfo(int userNum, ONVIF_USER_INFO *userInfo);
////int delUserinfo(int userNum, ONVIF_USER_INFO *userInfo);
////int setUserinfo(int userNum, ONVIF_USER_INFO *userInfo);

int getVideoParam(HBOOL flag, SDK_AVPARAM_GET_RES *videoParam);
int getDevParam(HBOOL flag, SDK_SPCDEVPARAM_GET_RES	*devParam);
////int setPtz(HINT32 nSpeed, HCHAR bOneStep, E_SDK_PTZ_DIRECT ePtzCmd);

int GetDeviceSerialNumber(char *chSerialNumber,const int nLen);

HINT32 getVideoEncodeParam(HUINT16 *nLimitFps, HUINT16 *nLimitKBps, HINT32 nChn);
HINT32 setVideoEncodeParam(HUINT16 nFps, HUINT16 nKBps, HINT32 nChn);

HINT32 setNtpParam(HCHAR *timeStr);
HBOOL	GetOnvifAuthFlag();

#ifdef __cplusplus
}
#endif


#endif

