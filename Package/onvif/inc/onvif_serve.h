#ifndef _ONVIF_SERVE_H
#define _ONVIF_SERVE_H

#include <hyBaseType.h>
#include "onvif.h"
#include "onvif_function.h"

#ifdef __cplusplus
extern "C" {
#endif

HINT16 serveInit(ACCEPT_STRUCT *acceptInfo, char * addr, int port);
HINT16 serveAccept(ACCEPT_STRUCT *info);
HINT16 serveDataProcess(ACCEPT_STRUCT *acceptInfo);
HINT16 parseData(ONVIF_STRUCT *onvif);
void onvifDestroy(ONVIF_STRUCT *onvif);
int motor_ctrl_cb(ptzcb cb);
#ifdef __cplusplus
}
#endif

#endif
