#include "onvif_serve.h"
#include "xml_node.h"
#include <pthread.h>
//#include <hyLogManager.h>
//#include <hyErrorCode.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "onvif_function.h"
#include "commdef.h"
#include "onvif_threadpool.h"
#include "onvif_error_respones.h"
#include <poll.h>
//#include "libwebsockets.h"

#define SERVE_BACKLOG 100

extern char g_onvif_auth;

HINT16 recvData(ONVIF_STRUCT *onvif);
HINT16 getHttpInfo(ONVIF_STRUCT *onvif);
HINT16 porcessRequest(ONVIF_STRUCT *onvif,XMLN *header, XMLN *body);

HINT16 serveInit(ACCEPT_STRUCT *acceptInfo, char * addr, int port)
{
	int nSockkFd;

	int nReuseVal = 1;
    int nRes;
    struct sockaddr_in addrInfo;

	errno = 0;
	acceptInfo->nListenFd = socket(AF_INET,SOCK_STREAM,0);
	if(acceptInfo->nListenFd < 0)
	{
		ERROR_LOG("create socket failed:%s\n",strerror(errno));
		acceptInfo->nListenFd = -1;
		return -1;
	}
	
	nRes = setsockopt(acceptInfo->nListenFd, SOL_SOCKET, SO_REUSEADDR, (char *)&nReuseVal, 4);
	if(nRes < 0)
	{
		ERROR_LOG("set socket reuse failed:%s\n",strerror(errno));
	}

	addrInfo.sin_family = AF_INET;
	addrInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	addrInfo.sin_port = htons(port);

	errno = 0;
	nRes = bind(acceptInfo->nListenFd,(struct sockaddr *)&addrInfo,sizeof(addrInfo));
	if(nRes < 0)
	{
		ERROR_LOG("bind tcp socket failed:%s\n",strerror(errno));
		close(acceptInfo->nListenFd);
		acceptInfo->nListenFd = -1;
		return -1;
	}

	errno = 0;
	nRes = listen(acceptInfo->nListenFd, SERVE_BACKLOG);
	if(nRes < 0)
	{
		ERROR_LOG("listen tcp socket failed:%s\n",strerror(errno));
		close(acceptInfo->nListenFd);
		acceptInfo->nListenFd = -1;
		return -1;
	}
	return 0;
}

HINT16 serveAccept(ACCEPT_STRUCT *info)
{
	HINT32 nSockFd = -1;
	HINT32 nRet = -1;
	HINT32 nPollTimeOut = 1000*3;
	static struct pollfd sFdWait[2];
	struct timeval timeout={0,500000};
	socklen_t len = (socklen_t)sizeof(info->sClientAddr);

	if (sFdWait[0].fd != info->nListenFd) {
		sFdWait[0].fd = info->nListenFd;
		sFdWait[0].events = POLLIN | POLLERR;
		sFdWait[0].revents = 0;
	}
	nRet = poll(sFdWait, 1, nPollTimeOut);
	if (nRet > 0 && sFdWait[0].fd == info->nListenFd && sFdWait[0].revents && POLLIN) {
		errno = 0;
		nSockFd = accept(info->nListenFd,(struct sockaddr *)&info->sClientAddr,&len);
		if (nSockFd < 0) {
			ERROR_LOG("accept tcp socket failed:%s\n",strerror(errno));
			return -1;
		}

		errno = 0;
		nRet = setsockopt(nSockFd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
		if (nRet < 0) {
			nRet = setsockopt(nSockFd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
			if (nRet < 0)ERROR_LOG("set socket send and recv timeout error:%s\n",strerror(errno));
		}
	} else {
		ERROR_LOG("wait client!\n");
		return nRet;
	}
	return nSockFd;
}

HINT16 serveDataProcess(ACCEPT_STRUCT *acceptInfo)
{
	HINT16 nRet;
	pthread_t tid;
	ONVIF_STRUCT onvifData;
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);

	memset(&onvifData, 0, sizeof(ONVIF_STRUCT));
	
	onvifData.nSockFd = acceptInfo->nSockFd;

	//memcpy(&onvifData.sClientAddr, &acceptInfo->sClientAddr, sizeof(onvifData.sClientAddr));
	
	nRet = recvData(&onvifData);
	if (nRet < 0) {
		ERROR_LOG("recv data error!\n");
		return -1;
	}

	ERROR_LOG("client url:%s\n", onvifData.cUrl);
	if (strcmp(onvifData.cUrl, URL_EVENT) == 0) {
	    nRet = pthread_create(&tid, NULL,threadProcessData, (void*)&onvifData);
		if (nRet >= 0) pthread_detach(tid);
		else ERROR_LOG("create pthread error:%d",nRet);
		// 增加延时目的是线程创建后将onvifData中的数据拷贝到自己的存储??
		// 此处不可对onvifData调用onvifDestroy锢?毁变量（该函数会关闭当前使用的sock 句柄），
		usleep(500*1000);
		onvifData.nSockFd = -1;
		free(onvifData.cRcvBuff);
	}
	else if(strstr(onvifData.cUrl, ONVIF_SNAP_URI) != 0)
	{
			/*解析抓拍*/
			char * pTmp = onvifData.cUrl + strlen(ONVIF_SNAP_URI);
			pTmp  += strlen("?chn=");
			//ERROR_LOG("onvifData.cUrl = ***%s***,pTmp = ***%s*** \n",onvifData.cUrl,pTmp);
			sendSnapJpeg(&onvifData,atoi(pTmp));
			onvifDestroy(&onvifData);
	}
	else {
		nRet = parseData(&onvifData);
		if (nRet < 0) {
			ERROR_LOG("parse Data error!\n");
			onvifDestroy(&onvifData);
			return -1;
		}
		onvifDestroy(&onvifData);
	}

	return 0;
}

int check_malloc_free_sum = 0;

HINT16 recvData(ONVIF_STRUCT *onvif)
{
	char temp[2048];
	char *position = NULL;
	HINT16 nHeadLen;
	HINT32 nContentLen = 0;
	HINT32 nLen;
	HINT16 nRet;

	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	

	/*
	 * 从接收缓冲区读取数据副本，确定http头部长度 http头部和正文用"\r\n\r\n"隔开
	 * 但是报文中存??\r\n\r\n"的不丢?定是http报文，在获取报文类型时会进一步判??
	 */
	nHeadLen = recv(onvif->nSockFd, temp, sizeof(temp), MSG_PEEK);
	if(nHeadLen < 0) {
		ERROR_LOG("recv the copy of data error!\n");
		close(onvif->nSockFd);
		onvif->nSockFd = -1;
		return -1;
	}

	position = strstr(temp,"\r\n\r\n");
	if ((position - temp+4) > HTTP_HEAD_MAX_LEN) {
		ERROR_LOG("head data too large!\n");
		close(onvif->nSockFd);
		onvif->nSockFd = -1;
		return -1;
	}
	
	// 接收HTTP头部
	nHeadLen = recv(onvif->nSockFd, onvif->cHttpHead, (position - temp+4), 0);
	if (nHeadLen < (position - temp+4)) {
		ERROR_LOG("recv http head data error!\n");
		close(onvif->nSockFd);
		onvif->nSockFd = -1;
		return -1;
	}
	
	nRet = getHttpInfo(onvif);
	if (nRet < 0) {
		ERROR_LOG("get http type error!\n");
		close(onvif->nSockFd);
		onvif->nSockFd = -1;
		return -1;
	}
	//ERROR_LOG("Http Head data\n%s\n",onvif->cHttpHead);
	// 确定报文正文长度 可以没有报文正文，请求抓拍图片时没有报文正文
	position = strstr(onvif->cHttpHead,"Content-Length:");
	if (position == NULL ) {
		position = strstr(onvif->cHttpHead,"content-length:");
		if (position == NULL) {
			ERROR_LOG("can't get data length\n");
			onvif->cRcvBuff = NULL;
			return 0;
		} else {
			sscanf(position,"content-length:%d", &nContentLen);
		}
	} else {
		sscanf(position,"Content-Length:%d", &nContentLen);
	}

	onvif->cRcvBuff = (char *)malloc(nContentLen+1);
	if (NULL == onvif->cRcvBuff) {
		ERROR_LOG("malloc error!\n");
		close(onvif->nSockFd);
		onvif->nSockFd = -1;
		return -1;
	}
	// 接收报文正文
	onvif->nBufflen = 0;
	do {
		 nLen = recv(onvif->nSockFd, onvif->cRcvBuff+onvif->nBufflen, nContentLen, 0);
		 if (nLen <= 0) break;
		 onvif->nBufflen += nLen;
		 nContentLen -= nLen;
		 if (nLen <= 0) break;
	} while(1);

	if (onvif->nBufflen < nContentLen) {
		ERROR_LOG("recv body data error!recv data:%d(%d)\n",onvif->nBufflen,nContentLen);
		free(onvif->cRcvBuff);
		onvif->cRcvBuff = NULL;
		close(onvif->nSockFd);
		onvif->nSockFd = -1;
		return -1;
	}
//  ERROR_LOG("Http data\n%s\n",onvif->cRcvBuff);
//	printf("data\n%s\n",onvif->cRcvBuff);

	return 0;
}

// 获取http报文类型,报文中的url等，获取前会验证是否为http报文
HINT16 getHttpInfo(ONVIF_STRUCT *onvif)
{
	char *position = NULL;
	int nRet;
	char temp[512];

	// 棢?查是否是HTTP报文
	do {
		position = strstr(onvif->cHttpHead,"HTTP/1.0");
		if (position != NULL) break;
		position = strstr(onvif->cHttpHead,"HTTP/1.1");
		if (position != NULL) break;
		else {
			ERROR_LOG("recv data is not HTTP data!\n");
			return -1;
		}
	} while(0);

	position = NULL;

	nRet  = sscanf(onvif->cHttpHead,"%s %s %s\r\n",temp,onvif->cUrl,temp);
	if (nRet < 3) {
		ERROR_LOG("get http url error!\n");
		return -1;
	}

	do {
		position = strstr(onvif->cHttpHead,"Content-Type:");
		if (position != NULL) break;
		position = strstr(onvif->cHttpHead,"content-type:");
		if (position == NULL) {
			ERROR_LOG("recv data no type!\n");
			onvif->eDataType = TYPE_NULL;
			return 0;
		}
	} while(1);
	
	position = NULL;
	do {
		position = strstr(onvif->cHttpHead,"application/sdp");
		if (position != NULL) {
			onvif->eDataType = TYPE_SDP;
			break;
		}
		position = strstr(onvif->cHttpHead,"application/soap+xml");
		if (position != NULL) {
			onvif->eDataType = TYPE_TXT;
			break;
		}
		position = strstr(onvif->cHttpHead,"text/plain");
		if (position != NULL) {
			onvif->eDataType = TYPE_HTM;
			break;
		}
		position = strstr(onvif->cHttpHead,"text/html");
		if (position != NULL) {
			onvif->eDataType = TYPE_XML;
			break;
		}
		position = strstr(onvif->cHttpHead,"application/octet-stream");
		if (position != NULL) {
			onvif->eDataType = TYPE_BIN;
			break;
		} else {
			onvif->eDataType = TYPE_NULL;
		}
	}while(0);

	return 0;
}

HINT16 parseData(ONVIF_STRUCT *onvif)
{
	XMLN *sXmlHeader = NULL;
	XMLN *sXmlBody = NULL;
	HINT16 nRet;
	HBOOL bAuthFlag = HFALSE;
	ERROR_MSG errMsg;

	if (onvif->cRcvBuff == NULL) return -1;
	
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	onvif->sXmlData = xxx_hxml_parse(onvif->cRcvBuff,onvif->nBufflen);
	if (onvif->sXmlData == NULL || onvif->sXmlData->name == NULL) {
		ERROR_LOG("get xml data error!\n");
		xml_node_del(onvif->sXmlData);
		onvif->sXmlData = NULL;
		return -1;
	}

	sXmlHeader = xml_node_soap_get(onvif->sXmlData, "Header");
	if (sXmlHeader == NULL) {
		//if (g_onvif_auth)
		// {
		// 	xml_node_del(onvif->sXmlData);
		// 	onvif->sXmlData = NULL;
		// 	return -1;
		// }
		// else
		// {
		// 	onvif->bAuthFlag = HTRUE;
		// 	strcpy(onvif->cUserName, "admin");
		// }
		onvif->bAuthFlag = HFALSE;
	}
	else
	{
		#if 1
			bAuthFlag = g_onvif_auth;//HTRUE;//hySysCfgGet()->sOnvifParam.bOnvifAuth;通过配置文件强制不校验
			if (bAuthFlag) {
				if (sXmlHeader != NULL) {
					onvif->bAuthFlag = onvifUserAuth(onvif, sXmlHeader);
				} else 
					onvif->bAuthFlag = HFALSE;
				ERROR_LOG("user auth flag:%d result:%d\n", bAuthFlag, onvif->bAuthFlag);    
			} else {
				// 当不需要验证时，使用无效用户名，用以生成rtsp uri
				onvif->bAuthFlag = HTRUE;
				strcpy(onvif->cUserName, "admin");
				//strcpy(onvif->cPassword, "passwd");
			}
		#else
			onvif->bAuthFlag = HTRUE;
			strcpy(onvif->cUserName, "user");
		#endif
	}

	if(onvif->bAuthFlag == HFALSE)
	{
		ERROR_LOG("user auth error!\n");
		// xml_node_del(onvif->sXmlData);
		// onvif->sXmlData = NULL;
		// return -1;

		ERROR_MSG errMsg;
		errMsg.contentType = onvif->eDataType;
		int nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_NOT_AUTH);
		xml_node_del(onvif->sXmlData);
		onvif->sXmlData = NULL;
		return nRet;
	}

	sXmlBody = xml_node_soap_get(onvif->sXmlData, "Body");
	if (sXmlBody == NULL) {
		ERROR_LOG("get xml body error!\n");
		xml_node_del(onvif->sXmlData);
		onvif->sXmlData = NULL;
		return -1;
	}
	if (strlen(sXmlBody->f_child->name) > HTTP_TAG_MAX_LEN-1) {
		ERROR_LOG("tag too long\n");
	} else {
		strcpy(onvif->cTag,sXmlBody->f_child->name);
	}


	nRet = porcessRequest(onvif, sXmlHeader, sXmlBody);
	if (nRet < 0) {
		ERROR_LOG("process request error!\n");
		xml_node_del(onvif->sXmlData);
		onvif->sXmlData = NULL;
		return -1;
	}
	return 0;
}

void onvifDestroy(ONVIF_STRUCT *onvif)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	if (onvif->nSockFd != -1) {
		close(onvif->nSockFd);
		onvif->nSockFd = -1;
	}

	if (strlen(onvif->cHttpHead) != 0) {
		memset(onvif->cHttpHead, 0, sizeof(onvif->cHttpHead));
	}

	if (strlen(onvif->cUrl) != 0) {
		memset(onvif->cUrl, 0, sizeof(onvif->cUrl));
	}

	if (onvif->sXmlData != NULL) {
		xml_node_del(onvif->sXmlData);
		onvif->sXmlData = NULL;
	}

	if (onvif->cRcvBuff != NULL) {
		free(onvif->cRcvBuff);
		onvif->cRcvBuff = NULL;	
	}

	if (strlen(onvif->cTag) != 0) {
		memset(onvif->cTag, 0, sizeof(onvif->cTag));
	}

	onvif->nBufflen = 0;
	onvif->eDataType = TYPE_NULL;
	onvif->bAuthFlag = 0;
	onvif->nUserRole = -1;

	memset(onvif->cUserName, 0, sizeof(onvif->cUserName));
	//memset(onvif->cPassword, 0, sizeof(onvif->cPassword));
	memset(&onvif->sClientAddr,0,sizeof(onvif->sClientAddr));
	memset(onvif->action,0,sizeof(onvif->action));	
}

HINT16 porcessRequest(ONVIF_STRUCT *onvif,XMLN *header, XMLN *body)
{ 
	int nRet;
	XMLN *bodyData = NULL;
	ERROR_MSG errMsg;
	
	ERROR_LOG("=============>tag:\t%s\n",onvif->cTag);
	
	if (soap_strcmp(onvif->cTag, "GetSystemDateAndTime") == 0)
		return onvifGetSystemDateAndTime(onvif,body);
	if (soap_strcmp(onvif->cTag, "GetDeviceInformation") == 0)		
		return onvifGetDeviceInformation(onvif,body);
	if (soap_strcmp(onvif->cTag, "GetNetworkInterfaces") == 0)
		return onvifGetNetworkInterfaces(onvif,body);
	if (soap_strcmp(onvif->cTag, "GetScopes") == 0)
		return onvifGetScopes(onvif,body);
	if (soap_strcmp(onvif->cTag, "GetDNS") == 0)
		return onvifGetDNS(onvif,body);

	if (!onvif->bAuthFlag) {
		errMsg.contentType = onvif->eDataType;
		nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_NOT_AUTH);
		return nRet;
	}

	if (soap_strcmp(onvif->cTag, "GetGuaranteedNumberOfVideoEncoderInstances") == 0)
		return onvifGetGuaranteedNumberOfVideoEncoderInstances(onvif,body);

	if (soap_strcmp(onvif->cTag, "SetSystemDateAndTime") == 0) {
		bodyData = xml_node_soap_get(body, "SetSystemDateAndTime");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifSetSystemDateAndTime(onvif, bodyData);
	}	

	if (soap_strcmp(onvif->cTag, "GetCapabilities") == 0) {
		bodyData = xml_node_soap_get(body, "GetCapabilities");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetCapabilities(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetProfiles") == 0) {
		bodyData = xml_node_soap_get(body, "GetProfiles");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetProfiles(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetServices") == 0) {
		bodyData = xml_node_soap_get(body, "GetServices");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetServices(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetServiceCapabilities") == 0) {		
		bodyData = xml_node_soap_get(body, "GetServiceCapabilities");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		#if 0
		ERROR_LOG("data:%s\n",bodyData->f_attrib->data);
		if (strstr(bodyData->f_attrib->data,"device")) {
			return onvifGetTdsServiceCapabilities(onvif, bodyData);
		}
		#else
		else return onvifGetTdsServiceCapabilities(onvif, bodyData);
		#endif
	}
	if (soap_strcmp(onvif->cTag, "GetVideoSources") == 0) {
		bodyData = xml_node_soap_get(body, "GetVideoSources");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetVideoSources(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetSnapshotUri") == 0) {
		bodyData = xml_node_soap_get(body, "GetSnapshotUri");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetSnapshotUri(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetProfile") == 0) {
		bodyData = xml_node_soap_get(body, "GetProfile");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetProfile(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetStreamUri") == 0) {
		bodyData = xml_node_soap_get(body, "GetStreamUri");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetStreamUri(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetVideoSourceConfiguration") == 0) {
		bodyData = xml_node_soap_get(body, "GetVideoSourceConfiguration");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetVideoSourceConfiguration(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetVideoEncoderConfiguration") == 0) {
		bodyData = xml_node_soap_get(body, "GetVideoEncoderConfiguration");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetVideoEncoderConfiguration(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetVideoEncoderConfigurations") == 0) {
		bodyData = xml_node_soap_get(body, "GetVideoEncoderConfigurations");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetVideoEncoderConfigurations(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "SetVideoEncoderConfiguration") == 0) {
		bodyData = xml_node_soap_get(body, "SetVideoEncoderConfiguration");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifSetVideoEncoderConfiguration(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetVideoEncoderConfigurationOptions") == 0) {
		bodyData = xml_node_soap_get(body, "GetVideoEncoderConfigurationOptions");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetVideoEncoderConfigurationOptions(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetConfiguration") == 0) {
		bodyData = xml_node_soap_get(body, "GetConfiguration");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetPTZConfiguration(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetConfigurations") == 0) {
		bodyData = xml_node_soap_get(body, "GetConfigurations");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetPTZConfigurations(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetNode") == 0) {
		bodyData = xml_node_soap_get(body, "GetNode");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetNode(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetStatus") == 0) {
		bodyData = xml_node_soap_get(body, "GetStatus");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetStatus(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "Stop") == 0) {
		bodyData = xml_node_soap_get(body, "Stop");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifStopPtz(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "ContinuousMove") == 0) {
		bodyData = xml_node_soap_get(body, "ContinuousMove");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifContinuousMove(onvif, bodyData);
	}	
	if (soap_strcmp(onvif->cTag, "GetUsers") == 0) {
		bodyData = xml_node_soap_get(body, "GetUsers");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetUsers(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "CreateUsers") == 0) {
		bodyData = xml_node_soap_get(body, "CreateUsers");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifCreateUsers(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "DeleteUsers") == 0) {
		bodyData = xml_node_soap_get(body, "DeleteUsers");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifDeleteUsers(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "SetUser") == 0) {
		bodyData = xml_node_soap_get(body, "SetUser");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifSetUser(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "CreatePullPointSubscription") == 0) {
		bodyData = xml_node_soap_get(body, "CreatePullPointSubscription");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifCreatePullPointSubscription(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetEventProperties") == 0) {
		bodyData = xml_node_soap_get(body, "GetEventProperties");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetEventProperties(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "PullMessages") == 0) {
		bodyData = xml_node_soap_get(body, "PullMessages");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifPullMessages(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "Unsubscribe") == 0) {
		bodyData = xml_node_soap_get(body, "Unsubscribe");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifUnsubscribe(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "Renew") == 0) {
		bodyData = xml_node_soap_get(body, "Renew");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifRenew(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetOptions") == 0) {
		bodyData = xml_node_soap_get(body, "GetOptions");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetOptions(onvif, bodyData);
	}
	if (soap_strcmp(onvif->cTag, "GetVideoSourceConfigurations") == 0) {
		bodyData = xml_node_soap_get(body, "GetVideoSourceConfigurations");
		ERROR_LOG("data:%s\n",bodyData->name);
		if (bodyData == NULL) return -1;
		else return onvifGetVideoSourceConfigurations(onvif, bodyData);
	}
	
	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_SERVICE_NOT_SUPPORT);

	return nRet;
	
}
int motor_ctrl_cb(ptzcb cb)
{
	onvif_ctrl_motor(cb);
}


