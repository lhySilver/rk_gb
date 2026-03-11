#ifndef _ONVIF_THREADPOOL_H
#define _ONVIF_THREADPOOL_H


#include <semaphore.h>
#include "onvif.h"
#ifdef __cplusplus
extern "C" {
#endif

void *threadProcessData(void *arg);

#ifdef __cplusplus
}
#endif

#endif
