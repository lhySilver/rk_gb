#ifndef _ONVIF_H
#define _ONVIF_H
#include <netinet/in.h>
#include "xml_node.h"
#include <stdbool.h>
#include <hyBaseType.h>

////#include <hySdkDataType.h>
////#include <hyErrorCode.h>
////#include "hySyscfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_HEAD_MAX_LEN 1024
#define HTTP_URL_MAX_LEN 128
#define HTTP_TAG_MAX_LEN 64
#define USERNAME_LEN	 32

#define ONVIF_SOCK_TIMEOUT_SEC			2

typedef int (*ptzcb)(int dir);


typedef enum {	
	TYPE_SDP = 0,
	TYPE_TXT,
	TYPE_HTM,
	TYPE_XML,
	TYPE_BIN,
	TYPE_NULL,
}HTTPTYPE;

typedef struct {
	int nListenFd;
	int nSockFd;
	struct sockaddr_in sClientAddr;
	char cUserName[USERNAME_LEN];
	int nUserRole;
	bool bAuthFlag;
	char cHttpHead[HTTP_HEAD_MAX_LEN];
	char cUrl[HTTP_URL_MAX_LEN];
	char *cRcvBuff;
	int nBufflen;
	char cTag[HTTP_TAG_MAX_LEN];
	HTTPTYPE eDataType;
	char action[HTTP_URL_MAX_LEN];
	XMLN *sXmlData;
} ONVIF_STRUCT;

typedef struct {
	int nListenFd;
	int nSockFd;
	struct sockaddr_in sClientAddr;
} ACCEPT_STRUCT;

#define URL_EVENT "/onvif/pullpointsubcribe"

/*
 *	onvif鍚姩鍋滄鍑芥暟
 */
int StartOnvifPthread();
void StopOnvifPthread(void);
int motor_ctrl(ptzcb cb);

void setOnvifPassword(const char *password);
void setDeviceSerialNumber(const char *num);
void setOnvifAuth(char value);
/*
 *	鑾峰彇onvif涓Щ鍔ㄤ睛娴嬫姤璀︾姸鎬?
 */
HBOOL getMdAlarmStat(time_t seconds);

/*
 *	璁剧疆绉诲姩渚︽祴onvif鏁版嵁锛岄渶瑕佹斁鍏ヨ澶囨娴嬬Щ鍔ㄦ姤璀︾姸鎬佺殑鍑芥暟鍐?
 */
int setMdAlarmState(HBOOL flag);

#ifdef __cplusplus
}
#endif


#endif
