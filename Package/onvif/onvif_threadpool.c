#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
////#include <hyErrorCode.h>
#include <hyBaseType.h>
////#include <hyLogManager.h>
#include <string.h>
#include "onvif_threadpool.h"
#include "onvif_serve.h"


void *threadProcessData(void *arg) {
	int nRet;
	ONVIF_STRUCT *onvifData = (ONVIF_STRUCT *)arg;
	ONVIF_STRUCT data;
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	memset(&data, 0, sizeof(ONVIF_STRUCT));

	memcpy(&data, onvifData, sizeof(ONVIF_STRUCT));
	data.cRcvBuff = (char *)malloc(onvifData->nBufflen);
	memcpy(data.cRcvBuff, onvifData->cRcvBuff, onvifData->nBufflen);
	
	nRet = parseData(&data);
	if (nRet < 0) {
		ERROR_LOG("parse Data error!\n");
	}

	onvifDestroy(&data);
	return NULL;
}
