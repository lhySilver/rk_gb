#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/prctl.h>
#include <hyBaseType.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "onvif_serve.h"
#include "onvif.h"
#include "commdef.h"
#include "pthread.h"
#include "onvif_threadpool.h"

static HBOOL g_bExitProc = HFALSE;
static sem_t g_MdSem;
static HBOOL g_MdFlag = HFALSE;

static pthread_t g_onvifPthreadTid = 0;
static void* OnvifThread(void* args);

/*
 * onvifdiscovery 功能
 */
void StopOnvifDiscoveryPthread(void);
int StartOnvifDiscoveryPthread(void);

int StartOnvifPthread()
{
	int nRet = 0;
	if(g_onvifPthreadTid == 0)
	{
		ERROR_LOG("Call onvif start \n");
		g_bExitProc = HFALSE;
		nRet = pthread_create(&g_onvifPthreadTid, NULL, OnvifThread, (void*)0);
		if (nRet != 0) 
			ERROR_LOG("create onvif pthread error :%d!\n",errno);
		else
			StartOnvifDiscoveryPthread();
	}

	return nRet;
}

static ACCEPT_STRUCT g_acceptInfo;

void StopOnvifPthread(void)
{
	if (g_onvifPthreadTid)  {
		if(g_acceptInfo.nListenFd > 0)
			{
				shutdown(g_acceptInfo.nListenFd,SHUT_RDWR);
				//close(g_acceptInfo.nListenFd);
			}
		if(g_acceptInfo.nSockFd  > 0)
			{
				shutdown(g_acceptInfo.nSockFd,SHUT_RDWR);
				//close(g_acceptInfo.nSockFd);
			}
		g_bExitProc = HTRUE;
		pthread_join(g_onvifPthreadTid,NULL);
		sem_destroy(&g_MdSem);
		StopOnvifDiscoveryPthread();
		g_onvifPthreadTid = 0;
	}
}


static void* OnvifThread(void* args)
{
	//ERROR_LOG("onvif thread create!\n");
	prctl(PR_SET_NAME, "onvifThread");
	int	m, s;
	int nRet;
	pthread_t tid;
	HUINT16 nOvifPort;
	
	nRet = sem_init(&g_MdSem, 0, 0);
	memset(&g_acceptInfo,0,sizeof(g_acceptInfo));
	nOvifPort = getOnvifPort(HFALSE);//hySysCfgGet()->sOnvifParam.nOnvifPort;
	nRet = serveInit(&g_acceptInfo, NULL, nOvifPort);
	if (nRet < 0) {
		ERROR_LOG("server Init Failed :%d,%d \n",nRet,nOvifPort);
		return NULL;
	}

	
	while(g_bExitProc == HFALSE)
	{ 
		g_acceptInfo.nSockFd = serveAccept(&g_acceptInfo);
		if (g_acceptInfo.nSockFd <= 0) continue;
		ERROR_LOG("accept connect,fd:%d\n",g_acceptInfo.nSockFd);
		nRet = serveDataProcess(&g_acceptInfo);
		if (nRet < 0 ) {
			ERROR_LOG("recv data error!\n");
		}
		memset(&g_acceptInfo.sClientAddr, 0, sizeof(g_acceptInfo.sClientAddr));
		g_acceptInfo.nSockFd = -1;
	}
	if(g_acceptInfo.nListenFd > 0)
	{
		close(g_acceptInfo.nListenFd);
		g_acceptInfo.nListenFd = -1;
	}
	if(g_acceptInfo.nSockFd  > 0)
	{
		close(g_acceptInfo.nSockFd);
		g_acceptInfo.nSockFd = -1;

	}
	return NULL;
}

int motor_ctrl(ptzcb cb)
{
	motor_ctrl_cb(cb);
}

HBOOL getMdAlarmStat(time_t seconds)
{
	int nRet;
	struct timespec timeout;

		
	ERROR_LOG(" wait time:%d\n",seconds);
	
	if (clock_gettime(CLOCK_REALTIME, &timeout) == -1) {
		ERROR_LOG("set plan onvif MD timeout failed!\n");
		return g_MdFlag;
	}

	timeout.tv_sec += seconds;
	if (g_MdFlag) return HTRUE;
	else {
		while ((nRet = sem_timedwait(&g_MdSem, &timeout)) == -1 && errno == EINTR)
        	continue;

		// 清除多余的信号量，可能在onvif启动前已经发送了多个信号量，
		while ((nRet =  sem_trywait(&g_MdSem)) == 0) 
			continue;
	}

	if(g_MdFlag) return HTRUE;
	else HFALSE;
}

int setMdAlarmState(HBOOL flag)
{
	if(g_onvifPthreadTid == 0)
		return 0;
	int nRet = 0;
	if (flag != g_MdFlag) {
		g_MdFlag = flag;
		if (g_MdFlag) {
			nRet = sem_post(&g_MdSem);
		}
	}         

	return nRet;
}

char g_onvifPassword[33] = "12345678";

void setOnvifPassword(const char *password)
{
	snprintf(g_onvifPassword, sizeof(g_onvifPassword), password);
}

//3C:7A:AA:9F:65:D4
char g_onvifDeviceSerialNumber[64] = "123456789ABC";

void setDeviceSerialNumber(const char *num)
{
	strncpy(g_onvifDeviceSerialNumber, num,  sizeof(g_onvifDeviceSerialNumber));
}

char g_onvif_auth = 1;

void setOnvifAuth(char value)
{
	g_onvif_auth = value;
}
