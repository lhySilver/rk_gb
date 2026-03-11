#ifndef _ONVIF_FUNCTION_H
#define _ONVIF_FUNCTION_H

#include "onvif.h"
#include "xml_node.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
#define ONVIF_SNAP_URI		"api/v1/snap.cgi"
typedef int (*onvifBuildXml)(ONVIF_STRUCT *onvif, char * p_buf, int len, void *arg);

typedef int (*ptzcb)(int dir);

bool onvifUserAuth(ONVIF_STRUCT *onvif,XMLN *data);
int onvif_ctrl_motor(ptzcb cb);

int buildResponseGetDeviceInformation(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetDeviceInformation(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetNetworkInterfaces(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg);
int onvifGetNetworkInterfaces(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetScopes(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg);
int onvifGetScopes(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetSystemDateAndTime(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetSystemDateAndTime(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseSetSystemDateAndTime(ONVIF_STRUCT *onvif, char *xmlBuff, HINT32 len, void *arg);
int onvifSetSystemDateAndTime(ONVIF_STRUCT *onvif,XMLN *data);
int buildResponseGetCapabilities(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetCapabilities(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetProfiles(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetProfiles(ONVIF_STRUCT *onvif,XMLN *data); 

int buildResponseGetServices(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetServices(ONVIF_STRUCT *onvif,XMLN *data); 

int buildResponseGetTdsServiceCapabilities(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetTdsServiceCapabilities(ONVIF_STRUCT *onvif,XMLN *data); 


int buildResponseGetVideoSources(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetVideoSources(ONVIF_STRUCT *onvif,XMLN *data); 

int buildResponseGetSnapshotUri(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetSnapshotUri(ONVIF_STRUCT *onvif,XMLN *data); 

int buildResponseGetProfile(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetProfile(ONVIF_STRUCT *onvif,XMLN *data); 

int buildResponseGetStreamUri(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetStreamUri(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetVideoSourceConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetVideoSourceConfiguration(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetVideoEncoderConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetVideoEncoderConfiguration(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseSetVideoEncoderConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifSetVideoEncoderConfiguration(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetVideoEncoderConfigurations(ONVIF_STRUCT *onvif, char *xmlBuff,HINT32 len, void *arg);
int onvifGetVideoEncoderConfigurations(ONVIF_STRUCT *onvif,XMLN *data);
int buildResponseGetVideoEncoderConfigurationOptions(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetVideoEncoderConfigurationOptions(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetDNS(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg);
int onvifGetDNS(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetGuaranteedNumberOfVideoEncoderInstances(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg);
int onvifGetGuaranteedNumberOfVideoEncoderInstances(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetPTZConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetPTZConfiguration(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetPTZConfigurations(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetPTZConfigurations(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetNode(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetNode(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetStatus(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetStatus(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseStopPtz(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifStopPtz(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseContinuousMove(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifContinuousMove(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetUsers(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetUsers(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseCreateUsers(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifCreateUsers(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseDeleteUsers(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifDeleteUsers(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseSetUser(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifSetUser(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseCreatePullPointSubscription(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifCreatePullPointSubscription(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetEventProperties(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifGetEventProperties(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponsePullMessages(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifPullMessages(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseUnsubscribe(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifUnsubscribe(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseRenew(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg);
int onvifRenew(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetOptions(ONVIF_STRUCT * onvif, char * xmlBuff, int len, void * arg);
int onvifGetOptions(ONVIF_STRUCT *onvif,XMLN *data);

int buildResponseGetVideoSourceConfigurations(ONVIF_STRUCT * onvif, char * xmlBuff, HINT32 len, void * arg);
int onvifGetVideoSourceConfigurations(ONVIF_STRUCT *onvif,XMLN *data);
void sendSnapJpeg(ONVIF_STRUCT *onvif, HUCHAR nChnIndex);

#ifdef __cplusplus
}
#endif


#endif
