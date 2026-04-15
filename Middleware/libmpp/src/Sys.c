#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>

#include <signal.h>

#include "PAL/MW_Common.h"
#include "PAL/I2c.h"





void SystemBeep(unsigned int dwFrequence, unsigned int dwDuration )
{

} /* end SystemBeep */

#define FB_KEEPALIVE 0
#define FB_REBOOT 1
#define FB_SHUTDOWN 2
void SystemReboot(void)
{
	printf("\033[1;36m  SystemReboot    \033[0m\n");
	FILE *fp = NULL;

	//add on 2025.03.13<±ÜÃâÂß¼­³åÍ»>
	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	//add on 2025.03.13<±ÜÃâÂß¼­³åÍ»>
	
	fp = popen("reboot", "r");
	if(fp == 0)
	{
		EMSG("error:%s  SystemReboot failed\n", __FUNCTION__);
	}
	pclose(fp);
	
	return;
} /* end SystemReboot */

/************************************************************************
 * halt
 ************************************************************************/
void SystemShutdown(void)
{
#if 0
	unsigned char buf[4];
	
	LIBDVR_PRINT("Sys Will ShutDown\n");
	buf[0] = 0xaa;
	buf[1] = FB_KEEPALIVE;
	buf[2] = 0x00;
	buf[3] = 0x00;
	FrontboardWrite(buf, 4);

	buf[0] = 0xaa;
	buf[1] = FB_SHUTDOWN;
	buf[2] = 0x00;
	buf[3] = 0x00;
	FrontboardWrite(buf, 4);
#endif
} /* end SystemShutdown */

static I2cHndl s_handle;
int CryptoCreate(void)
{
	int ret = -1;
	s_handle.fd = 0;
	ret = I2cOpen(&s_handle, 2);
	if (ret < 0)
		EMSG("CryptoCreate error\n");
	return ret;
}

int CryptoDestory(void)
{
	
	int ret = I2cClose(&s_handle);
	if (ret < 0)
		EMSG("CryptoDestory error\n");
	s_handle.fd = 0;
	return ret;
}

int CryptoWrite(int zone, const unsigned char *wdata, int len)
{
#if 0
	int iOff = 0;
	int ret = -1;
	unsigned char regAddr[len];
	for (iOff = 0; iOff < len; iOff ++)
	{
		regAddr[iOff] = zone + iOff;
		EMSG("W: 0x%02x\n",regAddr[iOff]);
	}
	ret = I2cWrite8(&s_handle, 0x50,regAddr, wdata, len);
	if (ret < 0)
		EMSG("CryptoWrite error\n");
	return ret;
#else
	
	/////(I2cHndl *hndl, Uint16 devAddr, Uint8 reg,Uint8 *value, Uint32 count)
	//int ret = I2cRawWrite8(&s_handle, 0x50,zone, wdata, len);
	//if (ret < 0)
	//	EMSG("CryptoWrite error\n");
	//return ret;

	int ret = -1;
	int i = 0;
	int page_size = 8;
	
	for (i = 0; i < len; i+=page_size) {
		usleep(1000*10);
		ret = I2cRawWrite8(&s_handle, 0x50,zone+i, (char *)wdata+i, page_size);
		if (ret < 0)
		{
			EMSG("CryptoWrite error\n");
			break;
		}
	}
	return ret;


	
#endif
}

int CryptoRead(int zone, unsigned char *rdata, int len)
{
#if 0
	int iOff = 0;
	int ret = -1;
	unsigned char regAddr[len];
	for (iOff = 0; iOff < len; iOff ++)
	{
		regAddr[iOff] = zone + iOff;
		EMSG("R: 0x%02x\n",regAddr[iOff]);
	}
	ret = I2cRead8(&s_handle, 0x50,regAddr, rdata, len);
	if (ret < 0)
		EMSG("CryptoRead error\n");
	return ret;
#else
	///(I2cHndl *hndl, Uint16 devAddr, Uint8 *value, Uint32 count)
	int ret = I2cRawRead8(&s_handle, 0x50, zone,rdata, len);
	if (ret < 0)
		EMSG("CryptoRead error\n");
	return ret;

#endif
}

