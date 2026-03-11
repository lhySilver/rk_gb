#ifndef _XML_GET_DATA
#define _XML_GET_DATA

#include "xml_node.h"
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * GetCapabilities
 */
typedef enum {
	CAP_CATEGORY_INVALID = -1,
	CAP_CATEGORY_MEDIA = 1, 
	CAP_CATEGORY_DEVICE,
	CAP_CATEGORY_ANALYTICS,
	CAP_CATEGORY_EVENTS,
	CAP_CATEGORY_IMAGE,
	CAP_CATEGORY_PTZ,
	CAP_CATEGORY_ALL
} CATEGORY_TYPE;

/*
 * PTZcfg
 */
#define	MAX_PRESET_NUM	4
typedef struct _floatRange_
{
	float min;
	float max;
} floatRange;

typedef struct _MOTOR_CONTROL_INFO
{
	int Scale;
	float pos;
	float home;
}MOTOR_CONTROL_INFO;

typedef struct _PRESET_INFO
{
    char *token;
    char *name;
    float pan;
    float tilt;
    float zoom;
}PRESET_INFO;

typedef struct _PTZConfig_
{
	char *token;
	char *name;
	int usecnt;
	char *nodetoken;
	char *abs_pant_space;
	char *abs_zoom_space;
	char *rel_pant_space;
	char *rel_zoom_space;
	char *cont_pant_space;
	char *cont_zoom_space;
	char *speed_pant_space;
	float  speed_pant_x;
	float  speed_pant_y;
	char *speed_zoom_space;
	float  speed_zoom_x;
	int  timeout;
	char *limit_pant_space;
	floatRange	limit_pant_x;	
	floatRange	limit_pant_y;	
	char *limit_zoom_space;
	floatRange	limit_zoom_x;
	floatRange abs_pant_x;
	floatRange abs_pant_y;
	floatRange abs_zoom_x;
	floatRange rel_pant_x;
	floatRange rel_pant_y;
	floatRange rel_zoom_x;
	floatRange cont_pant_x;
	floatRange cont_pant_y;
	floatRange cont_zoom_x;
	floatRange pant_speed_x;
	floatRange zoom_speed_x;
	int max_presets_num;
	int max_timeout;
	MOTOR_CONTROL_INFO pan;
	MOTOR_CONTROL_INFO tilt;
	MOTOR_CONTROL_INFO zoom;
	PRESET_INFO preset[MAX_PRESET_NUM];
} PTZConfig;

typedef struct {
	int index;
	char Protocol[64];
} STREAMURI_DATA;

//Metadata Configuration
typedef struct _MetaConfig_
{
	char *token;
	char *name;
	int	usecnt;
	char *ptz_status;
	char *ptz_position;
	char *topic_ext;
	char *mesg;
	char *analytics;
	char *net_type;
	char *ipv4;
	char *ipv6;
	int net_port;
	int net_ttl;
	char *autostart;
	char *timeout;
} MetaConfig;

typedef struct {
	const char *userName;
	const char *password;
	const char *noce;
	const char *created;
} AUTH_DATA;

#define MAX_USER_NAME_LEN 32
#define MAX_USER_PASSWD_LEN 32

typedef struct {
	char userName[MAX_USER_NAME_LEN];
	char password[MAX_USER_PASSWD_LEN];
	int userRole;
} ONVIF_USER_INFO;
typedef enum {
	NTP = 0,
	MANUAL = 1
} SYSTEM_DATE_TYPE;


typedef struct {
	SYSTEM_DATE_TYPE type;
	char timeZone[256];
	struct timeval timeValue;
} SYSTEM_DATE_TIME;

int getUserAuthData(XMLN *xmlHeader, AUTH_DATA *authData);
CATEGORY_TYPE getCapabilitiesCategory(XMLN *xmlBody);
int getStreamUriData(XMLN *xmlBody, STREAMURI_DATA *data);
PTZConfig * getPtzCfg(void);
MetaConfig * getMetaCfg(void);
int GetContinuousMoveData(XMLN *xmlBody, float *x, float *y);
int getUserInfoFromxml(XMLN *xmlBody, char *name, ONVIF_USER_INFO *userInfo);
int getVideoEncParamFromXml(XMLN *xmlBody, unsigned short *nFps, unsigned short *nKBps, int *nChn);
int getSystemDateAndTime(XMLN *xmlBody, SYSTEM_DATE_TIME *systemDateAndTime);
time_t getCreatePullPointTime(XMLN *xmlBody);
time_t getRenewTime(XMLN *xmlBody);

#ifdef __cplusplus
}
#endif


#endif
