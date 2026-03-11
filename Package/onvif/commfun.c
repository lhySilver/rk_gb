#include <netinet/if_ether.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hyBaseType.h>

#include "onvif_serve.h"
#include "commdef.h"
#include "xml_get_data.h"
extern char g_onvifDeviceSerialNumber[64];
// 获取本机ip  
 static int _NetGetLocalIp(const char *eth_inf, char *ip)
{  
	int sd;  
	//char ip[IP_SIZE];  

	struct sockaddr_in sin;  
	struct ifreq ifr;  

	sd = socket(AF_INET, SOCK_DGRAM, 0);  
	if (-1 == sd)  
	{  
		printf("socket error: %s\n", strerror(errno));  
		return -1;        
	}  

	strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ - 1] = 0;  
	
	// if error: No such device  
	if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)  
	{  
		printf("ioctl error: %s\n", strerror(errno));  
		close(sd);  
		return -1;  
	}  

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));  
	snprintf(ip, 16, "%s", inet_ntoa(sin.sin_addr));  

	close(sd);  
	return 0;  
}  

int getLocalIp(HBOOL flag, char* ipaddr)  
{  
	char IP[20] = {0};
	int b = _NetGetLocalIp("eth0",IP);
	if (b != 0)
	{
		_NetGetLocalIp("wlan0",IP);
	}
	strcpy(ipaddr,IP);
	return 0;
}

int GetMacAddress(char* macaddress,const char* localip)
{
	register int fd, intrface; 
	struct ifreq buf[32]; 
	struct arpreq arp; 
	struct ifconf ifc; 
	char ipaddress[32] = {0};
	int ret  = -1;
	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) 
	{ 
		ifc.ifc_len = sizeof buf; 
		ifc.ifc_buf = (caddr_t) buf; 
		if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) { 
				intrface = ifc.ifc_len / sizeof (struct ifreq); 
				while (intrface-- > 0) { 
					if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface]))) { 
						strcpy(ipaddress,inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr)); 
						if(strcmp(ipaddress,localip) == 0) {
							if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface])))   {
								
								memcpy(macaddress,buf[intrface].ifr_hwaddr.sa_data,ETH_ALEN);
								ret = 0;
							}
						}
					} 
				} 
		}
	}
    close (fd); 
    return ret; 
}

int getRtspPort(HBOOL flag)
{
#if 0
	static int rtspPort;
	static HBOOL dataFlag = HFALSE;	
	HINT32 nRet;
	//ERROR_LOG("%s,%d,flag:%d\n",__FUNCTION__, __LINE__,(int)flag);
	if (!dataFlag || flag) {
		rtspPort = hySysCfgGet()->nRtspPort;
		if (rtspPort < 0) return -1;
		dataFlag = HTRUE;
	}
	return rtspPort;
#else
	return 8554;
#endif
}

int GenRtspToken(char* chToken,int nLen)
{
#if 0
	int nRet = 0;
	S_FN_USERINFO *pUserInfos = 0;
	HINT32 nUserNum;
	
	nUserNum = fUMG_QueryUser(-1,&pUserInfos);
	if(nUserNum == 0)return -1;
	char 		chMD5StrVal[64] = {0};
	HUCHAR		chMD5Value[16];
	HCHAR		chSrcString[128] = {0};
	mbedtls_md5_context  ctx;
	int			i = 0;
	//ERROR_LOG("user[%d]username=%s,password:****** \n",i,pUserInfos[i].chUserName);
	sprintf(chSrcString,"%s:%s",pUserInfos[i].chUserName,pUserInfos[i].chPassWord);
	mbedtls_md5_init(&ctx);
	mbedtls_md5_starts(&ctx);
	mbedtls_md5_update(&ctx,(const unsigned char*)chSrcString,strlen(chSrcString));  
	mbedtls_md5_finish(&ctx,chMD5Value);
	mbedtls_md5_free(&ctx);	 
	for(i = 0;i< sizeof(chMD5Value);i++)
		sprintf(chMD5StrVal,"%s%02x",chMD5StrVal,chMD5Value[i]);	
	strncpy(chToken,chMD5StrVal,nLen);
	chToken[nLen -1] = 0;
	return nRet;
#else
	strncpy(chToken,"chMD5StrVal",nLen);
	return 0;
#endif
}

int GetHttpPort(HBOOL flag)
{
#if 0
	static int portNum = 0;
	static HBOOL dataFlag = HFALSE;
	SDK_HTTP_ATTR_GET_RES sRes;
	int nRet;
	//ERROR_LOG("%s,%d,flag:%d\n",__FUNCTION__, __LINE__,(int)flag);
	if (!dataFlag || flag) {
		portNum = hySysCfgGet()->nHttpPort;
		if (portNum < 0) return -1;
		dataFlag = HTRUE;
	}
	return portNum;
#else
	return 8080;
#endif
}

int getOnvifPort(HBOOL flag)
{
	return 835;
}

// int getUserinfo(HBOOL flag,S_FN_USERINFO **pUserInfos)
// {
// 	static S_FN_USERINFO userInfos[8];
// 	static HBOOL dataFlag = HFALSE;
// 	S_FN_USERINFO *getUser = NULL;
// 	int nRet;
// 	HINT32 nUserNum;
// 	ERROR_LOG("%s,%d,flag:%d\n",__FUNCTION__, __LINE__,(int)flag);	
// 	if (!dataFlag || flag){

// 		nUserNum = fUMG_QueryUser(-1,&getUser);
// 		if(nUserNum <= 0)return -1;
		
// 		memset(userInfos, 0, sizeof(userInfos));
// 		memcpy(userInfos, getUser, sizeof(S_FN_USERINFO) * nUserNum);
// 		dataFlag = HTRUE;

// 	}

// 	*pUserInfos = (S_FN_USERINFO *) malloc(sizeof(S_FN_USERINFO) * 8);
// 	memcpy(*pUserInfos, userInfos, sizeof(S_FN_USERINFO) * 8);

// 	return 8;
// }

// int addUserinfo(int userNum, ONVIF_USER_INFO *userInfo)
// {
// 	int nRet;
// 	S_FN_USERINFO userData;
// 	int i;

// 	for (i = 0; i < userNum; i++) {
// 		memset(&userData, 0, sizeof(S_FN_USERINFO));

// 		strcpy(userData.chUserName, userInfo[i].userName);
// 		strcpy(userData.chPassWord, userInfo[i].password);
// 		userData.nRoleId = userInfo[i].userRole;
// 		userData.bEnable = HTRUE;
	
// 		nRet = fUMG_AddUser(&userData);
// 		if (nRet < 0) return nRet;
// 	}

// 	return nRet;
// }

// int delUserinfo(int userNum, ONVIF_USER_INFO *userInfo)
// {
// 	S_FN_USERINFO *getUserInfos;
// 	int nRet;
// 	int delUserId[8];
// 	int nUserNum;
// 	int i,j;
// 	int k=0;

// 	nUserNum = getUserinfo(HTRUE, &getUserInfos);
// 	if (nUserNum < 0) return -1;

// 	for(i = 0; i < nUserNum; i++) {
// 		if (!getUserInfos[i].bEnable) continue;
// 		if (getUserInfos[i].nUserID = ADMIN_USER_ID) continue;
// 		for(j = 0; j < userNum; j++) {
// 			if (strcmp(getUserInfos[i].chUserName, userInfo[j].userName) == 0) {
// 				delUserId[k++] = getUserInfos[i].nUserID;
// 				break;
// 			}
// 		}
// 	}

// 	if (getUserInfos != NULL) free(getUserInfos);
	
// 	for (i = 0; i < k; i++) {
// 		nRet = fUMG_DelUser(delUserId[i]);
// 		if (nRet < 0) return nRet;
// 	}

// 	return nRet;
// }
// int setUserinfo(int userNum, ONVIF_USER_INFO *userInfo)
// {
// 	S_FN_USERINFO *getUserInfos = NULL;
// 	S_FN_USERINFO *setUserInfos;
// 	int nRet;
// 	int nUserNum;
// 	int i,j;
// 	int k=0;
// 	setUserInfos = (S_FN_USERINFO *)malloc(sizeof(S_FN_USERINFO)*userNum);
// 	if (setUserInfos == NULL) return -1;
// 	nUserNum = getUserinfo(HTRUE, &getUserInfos);
// 	if (nUserNum < 0) return -1;

// 	for(i = 0; i < nUserNum; i++) {
// 		if (!getUserInfos[i].bEnable) continue;
// 		for(j = 0; j < userNum; j++) {
// 			if (strcmp(getUserInfos[i].chUserName, userInfo[j].userName) == 0) {
				
// 				if (getUserInfos[i].nUserID = ADMIN_USER_ID) setUserInfos[k++].nRoleId = ADMIN_ROLE_ID;
// 				else setUserInfos[k++].nRoleId = userInfo->userRole;

// 				strcpy(setUserInfos[k++].chUserName, userInfo->userName);
// 				strcpy(setUserInfos[k++].chPassWord, userInfo->password);
// 				setUserInfos[k++].bEnable = HTRUE;
// 				break;
// 			}
// 		}
// 	}
	
// 	if (getUserInfos != NULL) free(getUserInfos);
// 	for (i = 0; i < k; i++) {
// 		nRet = fUMG_ModifyUser(&setUserInfos[i]);
// 		if (nRet < 0) 
// 		{
// 			free(setUserInfos);
// 			return nRet;
// 		}
// 	}
// 	if(setUserInfos != NULL) free(setUserInfos);
// 	return 0;
// }

int getVideoParam(HBOOL flag, SDK_AVPARAM_GET_RES *videoParam)

{
	static SDK_AVPARAM_GET_RES videoParamData;
	static HBOOL dataFlag = HFALSE;
	SDK_AVPARAM_GET_RES videoData;
	int nRet;
	//ERROR_LOG("%s,%d,flag:%d\n",__FUNCTION__, __LINE__,(int)flag);
	if (!dataFlag || flag) {
		#if 0
		nRet = vmsgVideo(1, &videoData, NULL, 0);
		if(nRet < 0) return -1;
		#else
		videoData.nVChnNum = 2;
		videoData.sVideParam[0].nWidth = 3840;//1920;//3840;
		videoData.sVideParam[0].nHeight = 1080;//544;//1080;
		videoData.sVideParam[0].nFps = 15;
		videoData.sVideParam[0].nGop = 60;
		videoData.sVideParam[0].nKBps = 1024;
		videoData.sVideParam[0].bWithAudio = 0;

		videoData.sVideParam[1].nWidth = 1920;
		videoData.sVideParam[1].nHeight = 540;
		videoData.sVideParam[1].nFps = 15;
		videoData.sVideParam[1].nGop = 60;
		videoData.sVideParam[1].nKBps = 512;
		videoData.sVideParam[1].bWithAudio = 0;

		videoData.sAudioParam.nSampleRate = 8000;
        videoData.sAudioParam.nKBPS = 65536;


		#endif
		memset(&videoParamData, 0, sizeof(SDK_AVPARAM_GET_RES));
		memcpy(&videoParamData, &videoData, sizeof(SDK_AVPARAM_GET_RES));

		dataFlag = HTRUE;
	}

	memset(videoParam, 0, sizeof(SDK_AVPARAM_GET_RES));
	memcpy(videoParam, &videoParamData, sizeof(SDK_AVPARAM_GET_RES));

	return 0;
	
}

int getDevParam(HBOOL flag, SDK_SPCDEVPARAM_GET_RES	*devParam)
{
	static SDK_SPCDEVPARAM_GET_RES devParamData;
	static HBOOL dataFlag = HFALSE;
	int nRet;
	//ERROR_LOG("%s,%d,flag:%d\n",__FUNCTION__, __LINE__,(int)flag);
	if (!dataFlag || flag) {
		#if 0
		nRet = devmsgSpcDevParam(1, &devParamData, NULL, 0);
		if (nRet < 0) return -1;
		dataFlag = HTRUE;
		#else
		strcpy(devParamData.stSpecDevParam.chSerialNum,g_onvifDeviceSerialNumber);
		strcpy(devParamData.stSpecDevParam.chVendor,"ipcam");
		strcpy(devParamData.stSpecDevParam.chModel,"ipc");
		#endif
	}
	memset(devParam, 0, sizeof(SDK_SPCDEVPARAM_GET_RES));
	strcpy(devParam->stSpecDevParam.chSerialNum, devParamData.stSpecDevParam.chSerialNum);
	strcpy(devParam->stSpecDevParam.chVendor, devParamData.stSpecDevParam.chVendor);
	strcpy(devParam->stSpecDevParam.chModel, devParamData.stSpecDevParam.chModel);

	return 0;
}

// int setPtz(HINT32 nSpeed, HCHAR bOneStep, E_SDK_PTZ_DIRECT ePtzCmd)
// {
// 	int nRet;
// 	nRet = ptzmsgCtrl(nSpeed, bOneStep, ePtzCmd, 0);
// 	return nRet;
// }

int GetDeviceSerialNumber(char *chSerialNumber,const int nLen)
{
	#if 0
	strncpy(chSerialNumber,hySysCfgGet()->sDevParam.chSerialNum,nLen-1);
	chSerialNumber[nLen-1] = 0;
	return 0;
	#else
	strncpy(chSerialNumber,g_onvifDeviceSerialNumber,nLen-1);
	chSerialNumber[nLen-1] = 0;
	return 0;
	#endif
}


/*
 * 视频编码通道1，通道2的帧率码率限制
 */
static HUINT16 g_nLimitFps[2][2] = {{6,30},{6,30}};
static HUINT16 g_nLimitKBps[2][2] = {{256,2048},{64,1024}};
HINT32 getVideoEncodeParam(HUINT16 *nLimitFps, HUINT16 *nLimitKBps, HINT32 nChn)
{
	nLimitFps[0] = g_nLimitFps[nChn][0];
	nLimitFps[1] = g_nLimitFps[nChn][1];
	nLimitKBps[0] = g_nLimitKBps[nChn][0];
	nLimitKBps[1] = g_nLimitKBps[nChn][1];
	return 0;
}

HINT32 setVideoEncodeParam(HUINT16 nFps, HUINT16 nKBps, HINT32 nChn)
{
	HINT32 nRes;
	S_VIDEO_PARAM sVideoParam;
	HUINT16 nSetFps = 0;
	HUINT16 nSetKBps = 0;
	HINT32 chn = nChn - 1;
	
	if (nFps <= g_nLimitFps[chn][0]) nSetFps = g_nLimitFps[chn][0];
	else if (nFps >= g_nLimitFps[chn][1]) nSetFps = g_nLimitFps[chn][1];
	else nSetFps = nFps;

	if (nKBps <= g_nLimitKBps[chn][0]) nSetKBps = g_nLimitKBps[chn][0];
	else if (nKBps >= g_nLimitKBps[chn][1]) nSetKBps = g_nLimitKBps[chn][1];
	else nSetKBps = nKBps;

	#if  0
	memcpy(&sVideoParam, &hySysCfgGet()->sVideoParam,sizeof(S_VIDEO_PARAM));
	sVideoParam.sVideoChnParam[chn].nFps = nSetFps;
	sVideoParam.sVideoChnParam[chn].nKBps = nSetKBps;

	nRes = IniSetVideoEncParam(sVideoParam, 2);
	if (nRes < 0) ERROR_LOG("set Video encode error!\n");
	#else
	nRes = 0;
	#endif

	return nRes;
}

HINT32 setNtpParam(HCHAR *timeStr)
{
	HINT32 nRes;
	if (timeStr == NULL) return -1;
	/*
	nRes = tzSet(0, 0, timeStr);
	if (nRes < 0) return -1;
	setTimeZone(timeStr);
	*/
	return 0;
}
