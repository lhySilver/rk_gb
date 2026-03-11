#include "onvif_function.h"
#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <hyBaseType.h>

#include "commdef.h"
#include "xml_node.h"
#include "xml_get_data.h"
#include "base64.h"
#include "sha1.h"
#include "onvif_error_respones.h"
#include "onvif_function.h"
static char xmlEnvelope[] = {
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	"<s:Envelope "
    "xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" "
    "xmlns:e=\"http://www.w3.org/2003/05/soap-encoding\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" " 
    "xmlns:xmime=\"http://www.w3.org/2005/05/xmlmime\" "
    "xmlns:tns1=\"http://www.onvif.org/ver10/topics\" "
    "xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" "
    "xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" " 
    "xmlns:tt=\"http://www.onvif.org/ver10/schema\" " 
    "xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" " 
    "xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" " 
    "xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" " 
    "xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" "
    "xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" "
    "xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" "
    "xmlns:timg=\"http://www.onvif.org/ver20/imaging/wsdl\" "
    "xmlns:ter=\"http://www.onvif.org/ver10/error\" >"
	"%s"
	"<s:Body>%s</s:Body>"
	"</s:Envelope>"
};

static char xmlHeaderWithAction[] = {
	"<s:Header>"
    	"<wsa:Action>%s</wsa:Action>"
	"</s:Header>"
};		

static char xmlHeader[] = {
	"<s:Header>"
	"</s:Header>"
};

static int(*ptzctrl)(int);

static char actionFormat[] = {"; action=\"%s\""};

#define COMPARE_FLAOT(x,y) ((x > (y-0.0000001)) && ( x < (y+0.0000001)))
#define SDK_PTZ_SPEED_LOW        1
#define SDK_PTZ_SPEED_MID        2
#define SDK_PTZ_SPEED_HIGH       3
typedef struct {
	HBOOL stat;
	HINT32 count;
	time_t endTime;
	pthread_mutex_t lock;
} MOTION_DETECT_INFO;
MOTION_DETECT_INFO g_pullPointMessageInfo = {
	.stat = HFALSE,
	.count = 0,
	.endTime = 0,
	.lock = PTHREAD_MUTEX_INITIALIZER
};

extern char g_onvifPassword[32];

int soapHttpRly(ONVIF_STRUCT *onvif, const char * xml, int len)
{
    int headLen;
 	char * buff;
	int i = (int)onvif->eDataType;
 	const char *ContentType[]={"application/sdp","application/soap+xml","text/plain","text/html","application/octet-stream"," "};

	buff = (char *)malloc(len + 1024);
	if (buff == NULL)
	{
		return -1;
	}
	headLen = sprintf(buff,	"HTTP/1.1 200 OK\r\n"
							"Server: gSoap/2.8\r\n"
							"Accept-Encoding: gzip, deflate\r\n"
							"Content-Type: %s; charset=utf-8%s\r\n"
							"Content-Length: %d\r\n"
							"Connection: close\r\n\r\n",
							ContentType[i],onvif->action,len);

	memcpy(buff+headLen, xml, len);
	headLen += len;
	buff[headLen] = '\0';
	send(onvif->nSockFd, buff, headLen, 0);
	free(buff);
	return headLen;
}


int buildResponseXml(ONVIF_STRUCT *onvif, onvifBuildXml buildXml, void *arg)
{
	int ret = -1, mlen = 1024*16, xlen;
	ERROR_MSG errMsg;
	char * xml = (char *)malloc(mlen);
	if (NULL == xml)
	{
	    ERROR_LOG("soap_build_send_rly  xml null\r\n");
		return -1;
	}
	
	xlen = buildXml(onvif, xml, mlen, arg);
	if (xlen < 0)
	{
		errMsg.contentType = onvif->eDataType;
		ret = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	}
	else
	{
		ret = soapHttpRly(onvif, xml, xlen);
		//ERROR_LOG("%s\r\n",xml);
	}
	free(xml);

	return ret;
}

/*
 *	onvifUserAuth
 */
static void soap_calc_digest(const char *created, unsigned char *nonce, int noncelen, const char *password, unsigned char hash[20])
{
	sha1_context ctx;

	sha1_starts(&ctx);
	sha1_update(&ctx, (unsigned char *)nonce, noncelen);
	sha1_update(&ctx, (unsigned char *)created, strlen(created));
	sha1_update(&ctx, (unsigned char *)password, strlen(password));
	sha1_finish(&ctx, (unsigned char *)hash);
}
bool onvifUserAuth(ONVIF_STRUCT *onvif,XMLN *data)
{
#if 0
	int nRet;
	AUTH_DATA authData;
	S_FN_USERINFO *pUserInfos = 0;
	int nUserNum;
	int i;
	int nNoceLen;
	unsigned char nonce[200];
	char HABase64[100];
	char cPassword[32];
	unsigned char HA[20];
	nUserNum = getUserinfo(HTRUE ,&pUserInfos);
	if (nUserNum <= 0) return false;

	nRet = getUserAuthData(data, &authData);
	if (nRet < 0) return false;

	strcpy(onvif->cUserName,authData.userName);
	for(i = 0;i < nUserNum;i++)
	{
		ERROR_LOG("user:%s enable:%d \n",pUserInfos[i].chUserName,pUserInfos[i].bEnable);
		if(pUserInfos[i].bEnable && strcmp(pUserInfos[i].chUserName,onvif->cUserName) == 0)
		{
			strncpy(cPassword,pUserInfos[i].chPassWord,sizeof(cPassword));
			onvif->nUserRole = pUserInfos[i].nRoleId;
			ERROR_LOG("Found user:%s \n",onvif->cUserName);
			break;
		}
	}
	
	if (pUserInfos != NULL) free(pUserInfos);

	if (i == nUserNum) {
		ERROR_LOG("user(%s) not found!\n",onvif->cUserName);
		return false;
	}

	//strcpy(onvif->cPassword, cPassword); 
	
	nNoceLen = base64_decode(authData.noce, nonce, sizeof(nonce));
	soap_calc_digest(authData.created, nonce, nNoceLen, cPassword, HA);
	onvif_base64_encode(HA, 20, HABase64, sizeof(HABase64));
	
	if (strcmp(HABase64, authData.password) == 0)
	{
		return true;
	}
	
	return false;
#else
	int nRet;
	AUTH_DATA authData;
	////S_FN_USERINFO *pUserInfos = 0;
	int nUserNum;
	int i;
	int nNoceLen;
	unsigned char nonce[200];
	char HABase64[100];
	char cPassword[32];
	unsigned char HA[20];
	nUserNum = 1;////getUserinfo(HTRUE ,&pUserInfos);
	if (nUserNum <= 0) return false;

	nRet = getUserAuthData(data, &authData);
	if (nRet < 0) return false;

	strcpy(onvif->cUserName,authData.userName);
	for(i = 0;i < nUserNum;i++)
	{
		////ERROR_LOG("user:%s enable:%d \n",pUserInfos[i].chUserName,pUserInfos[i].bEnable);
		////if(pUserInfos[i].bEnable && strcmp(pUserInfos[i].chUserName,onvif->cUserName) == 0)
		{
			strncpy(cPassword, g_onvifPassword, sizeof(cPassword));
			onvif->nUserRole = 1;
			ERROR_LOG("Found user:%s \n",onvif->cUserName);
			break;
		}
	}
	
	////if (pUserInfos != NULL) free(pUserInfos);

	if (i == nUserNum) {
		ERROR_LOG("user(%s) not found!\n",onvif->cUserName);
		return false;
	}

	//strcpy(onvif->cPassword, cPassword); 
	
	nNoceLen = base64_decode(authData.noce, nonce, sizeof(nonce));
	soap_calc_digest(authData.created, nonce, nNoceLen, cPassword, HA);
	onvif_base64_encode(HA, 20, HABase64, sizeof(HABase64));
	
	if (strcmp(HABase64, authData.password) == 0)
	{
		return true;
	}
	
	return false;
#endif
}

/*
 * GetDeviceInformation
 */
static NVT_deviceInfo NVTDeviceInfo;
int buildResponseGetDeviceInformation(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg)
{
	int nRet = 0;
	int nLen = 0;
	char *position = NULL;
	char temp[1024];
	unsigned char chIpAddr[32] = {0},chMacAddress[32] = {0};
	//FILE* fVersionFileHandle = fopen(IPCVERSION_FILE_PATH,"r");
	SDK_SPCDEVPARAM_GET_RES		sSpcDevParamGetRes;

	nRet = sprintf(onvif->action,actionFormat,"http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation");
	if (nRet < 0) return -1;
	
	nRet = getDevParam(HFALSE, &sSpcDevParamGetRes);
	getLocalIp(HFALSE, chIpAddr);
	GetMacAddress(chMacAddress,chIpAddr);
	// if(fVersionFileHandle){
	// 	fgets(NVTDeviceInfo.FirmwareVersion,sizeof(NVTDeviceInfo.FirmwareVersion),fVersionFileHandle);
	// 	fclose(fVersionFileHandle);
	// 	position = strstr(NVTDeviceInfo.FirmwareVersion,"\n");
	// 	*position = '\0';  
	//   } 
	//   else 
	  {
		//strcpy(NVTDeviceInfo.FirmwareVersion, "00.00000.00.00");
		strcpy(NVTDeviceInfo.FirmwareVersion, "2.0.1");
	  }

	sprintf(NVTDeviceInfo.HardwareId,"%02X-%02X-%02X-%02X-%02X-%02X",chMacAddress[0],
		 chMacAddress[1],chMacAddress[2],chMacAddress[3],chMacAddress[4],chMacAddress[5]);

	sprintf(NVTDeviceInfo.Manufacturer,"%s",sSpcDevParamGetRes.stSpecDevParam.chVendor);
	sprintf(NVTDeviceInfo.Model,"%s",sSpcDevParamGetRes.stSpecDevParam.chModel);
	sprintf(NVTDeviceInfo.SerialNumber,"%s",sSpcDevParamGetRes.stSpecDevParam.chSerialNum);
	//填充body
	nRet = sprintf(temp, 
		"<tds:GetDeviceInformationResponse>"
			"<tds:Manufacturer>%s</tds:Manufacturer>"
		 	"<tds:Model>%s</tds:Model>"
			"<tds:FirmwareVersion>%s</tds:FirmwareVersion>"
			"<tds:SerialNumber>%s</tds:SerialNumber>"
		 	"<tds:HardwareId>%s</tds:HardwareId>"
	    "</tds:GetDeviceInformationResponse>", 
	    NVTDeviceInfo.Manufacturer, NVTDeviceInfo.Model, NVTDeviceInfo.FirmwareVersion,
	    NVTDeviceInfo.SerialNumber, NVTDeviceInfo.HardwareId);
	if (nRet < 0) return -1;
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, temp);
	if (nRet < 0) return -1;
	
	return strlen(xmlBuff);
}
int onvifGetDeviceInformation(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	// 该接口接收到的信息中除校验用户名密码的信息外，无其他信息
	return buildResponseXml(onvif, buildResponseGetDeviceInformation, NULL);
}

/*
 * GetNetworkInterfaces
 */
int buildResponseGetNetworkInterfaces(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg)
{
	int nRet;
	char body[1024];

	nRet = sprintf(body,
		"<tds:GetNetworkInterfacesResponse>"
		"<tds:NetworkInterfaces token=\"NetworkInterface_token1\">"
		"<tt:Enabled>false</tt:Enabled>"
		"</tds:NetworkInterfaces>"
		"</tds:GetNetworkInterfacesResponse>");
	
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}

int onvifGetNetworkInterfaces(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	// 该接口接收到的信息中除校验用户名密码的信息外，无其他信息
	return buildResponseXml(onvif, buildResponseGetNetworkInterfaces, NULL);
}

/*
 * GetScopes
 */
int buildResponseGetScopes(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg)
{
	int nRet;
	char body[1024];

	nRet = sprintf(body,"<tds:GetScopesResponse></tds:GetScopesResponse>");
	
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetScopes(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetScopes, NULL);
}
/*
 * GetDNS
 */
int buildResponseGetDNS(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg)
{
	int nRet;
	char body[1024];

	nRet = sprintf(body,"<tds:GetDNSResponse></tds:GetDNSResponse>");
	
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetDNS(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetDNS, NULL);
}
/*
 * GetGuaranteedNumberOfVideoEncoderInstances
 */
int buildResponseGetGuaranteedNumberOfVideoEncoderInstances(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg)
{
	int nRet;
	char body[1024];

	nRet = sprintf(body,
		"<trt:GetGuaranteedNumberOfVideoEncoderInstancesResponse>"
        	"<trt:TotalNumber>0</trt:TotalNumber>"
        "</trt:GetGuaranteedNumberOfVideoEncoderInstancesResponse>");
	
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetGuaranteedNumberOfVideoEncoderInstances(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetGuaranteedNumberOfVideoEncoderInstances, NULL);
}

/*
 *	GetSystemDateAndTime
 */
int buildResponseGetSystemDateAndTime(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg)
{
	int nRet;
	char body[1024];
	char temp[1024];
	time_t uTime = time(0);
	struct tm *UTCtime = gmtime(&uTime);

	nRet = sprintf(onvif->action,actionFormat,"http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime");
	if (nRet < 0) return -1;
	
	//填充body
	nRet = sprintf(body,
			"<tds:GetSystemDateAndTimeResponse>"
			"<tds:SystemDateAndTime>"
			"<tt:DateTimeType>NTP</tt:DateTimeType>"
			"<tt:DaylightSavings>false</tt:DaylightSavings>"
			"<tt:TimeZone>"
				"<tt:TZ>00</tt:TZ>"
			"</tt:TimeZone>");
	if (nRet < 0) return -1;
	nRet = sprintf(temp,
			"<tt:UTCDateTime>"
				"<tt:Time>"
				"<tt:Hour>%d</tt:Hour>"
				"<tt:Minute>%d</tt:Minute>"
				"<tt:Second>%d</tt:Second>"
				"</tt:Time>"
				"<tt:Date>"
				"<tt:Year>%d</tt:Year>"
				"<tt:Month>%d</tt:Month>"
				"<tt:Day>%d</tt:Day>"
				"</tt:Date>"
			"</tt:UTCDateTime>",
			UTCtime->tm_hour, UTCtime->tm_min, UTCtime->tm_sec,	UTCtime->tm_year + 1900,
			UTCtime->tm_mon + 1, UTCtime->tm_mday);
	if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));
	
	nRet = sprintf(temp,
		"<tt:LocalDateTime>"
			"<tt:Time>"
			"<tt:Hour>%d</tt:Hour>"
			"<tt:Minute>%d</tt:Minute>"
			"<tt:Second>%d</tt:Second>"
			"</tt:Time>"
			"<tt:Date>"
			"<tt:Year>%d</tt:Year>"
			"<tt:Month>%d</tt:Month>"
			"<tt:Day>%d</tt:Day>"
			"</tt:Date>"
		"</tt:LocalDateTime>"
		"</tds:SystemDateAndTime>"
		"</tds:GetSystemDateAndTimeResponse>",
		UTCtime->tm_hour, UTCtime->tm_min, UTCtime->tm_sec, UTCtime->tm_year + 1900,
		UTCtime->tm_mon + 1, UTCtime->tm_mday);
	if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));
	
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetSystemDateAndTime(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	// 该接口接收到的信息中除校验用户名密码的信息外，无其他信息
	return buildResponseXml(onvif, buildResponseGetSystemDateAndTime, NULL);
}

/*
 *	setSystemDateAndTime
 */

int buildResponseSetSystemDateAndTime(ONVIF_STRUCT *onvif, HCHAR *xmlBuff, HINT32 len, void *arg)
{
	char body[1024];
	int nRet;

	nRet = sprintf(body,"<tds:SetSystemDateAndTimeResponse></tds:SetSystemDateAndTimeResponse>");
    if (nRet < 0) return -1;

	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}

HINT32 onvifSetSystemDateAndTime(ONVIF_STRUCT *onvif,XMLN *data)
{
	int nRes;
	SYSTEM_DATE_TIME systemDateAndTime;

	nRes = getSystemDateAndTime(data, &systemDateAndTime);
	if (nRes < 0) {
		ERROR_LOG("get system date and time param error!\n");
		return -1;
	}
	#if 0
	if (systemDateAndTime.type == MANUAL) {
		printf("time_T:%u,time_value:%u",time(NULL), systemDateAndTime.timeValue);
		nRes = settimeofday(&systemDateAndTime.timeValue, NULL);
		if (nRes < 0) {
			ERROR_LOG("set date and time by manual error!\n");
			return -1;
		}
	} else {
		nRes = setNtpParam(systemDateAndTime.timeZone);
		if (nRes < 0) {
			ERROR_LOG("set timeZone error!\n");
			return -1;
		}
	}
	#endif
	return buildResponseXml(onvif, buildResponseGetSystemDateAndTime, NULL);
}

 /*	GetCapabilities
 

/*
 * Capabilities中的内容在运行时不会变动，直接写死在该函数中
 * 当onvif需要修改时，请直接在下面函数中改动
 */	
 int buildResponseGetCapabilities(ONVIF_STRUCT *onvif, char *xmlBuff, int len, void *arg)
{	
	char temp[2048];
	char body[1024*8];
	int nRet;
	char localIp[32];
	int portNum;
	char device_xaddr[128];
	CATEGORY_TYPE category = *(CATEGORY_TYPE *) arg;

	//nRet = sprintf(onvif->action,actionFormat,"http://www.onvif.org/ver10/device/wsdl/GetCapabilities");
//	if (nRet < 0) return -1;
	
	nRet = getLocalIp(HFALSE, localIp);
	if (nRet < 0) return -1;
	portNum = getOnvifPort(HFALSE);//hySysCfgGet()->sOnvifParam.nOnvifPort;
	
	sprintf(device_xaddr,"http://%s:%d/onvif/device_service", localIp, portNum);

	nRet = sprintf(body,"<tds:GetCapabilitiesResponse><tds:Capabilities>");
	if (nRet < 0) return -1;

	if (category == CAP_CATEGORY_ALL || category == CAP_CATEGORY_ANALYTICS) {
		nRet = sprintf(temp,
			"<tt:Analytics>"
			"<tt:XAddr>%s</tt:XAddr>"
			"<tt:RuleSupport>false</tt:RuleSupport>"
			"<tt:AnalyticsModuleSupport>true</tt:AnalyticsModuleSupport>"
			"</tt:Analytics>",device_xaddr);
		if (nRet < 0) return -1;	
		strncat(body,temp,strlen(temp));
	}

	if (category == CAP_CATEGORY_ALL || category == CAP_CATEGORY_DEVICE) {
		nRet = sprintf(temp,
			"<tt:Device>"
				"<tt:XAddr>%s</tt:XAddr>"
				"<tt:Network>"
					"<tt:IPFilter>false</tt:IPFilter>"
					"<tt:ZeroConfiguration>false</tt:ZeroConfiguration>"
					"<tt:IPVersion6>false</tt:IPVersion6>"
					"<tt:DynDNS>false</tt:DynDNS>"
					"<tt:Extension>"
						"<tt:Dot11Configuration>false</tt:Dot11Configuration>"
					"</tt:Extension>"
				"</tt:Network>"
				"<tt:System>"
					"<tt:DiscoveryResolve>false</tt:DiscoveryResolve>"
					"<tt:DiscoveryBye>false</tt:DiscoveryBye>"
					"<tt:RemoteDiscovery>false</tt:RemoteDiscovery>"
					"<tt:SystemBackup>false</tt:SystemBackup>"
					"<tt:SystemLogging>true</tt:SystemLogging>"
					"<tt:FirmwareUpgrade>true</tt:FirmwareUpgrade>"
					"<tt:SupportedVersions>"
						"<tt:Major>2</tt:Major>"
						"<tt:Minor>50</tt:Minor>"
					"</tt:SupportedVersions>"
					"<tt:Extension>"
						"<tt:HttpFirmwareUpgrade>true</tt:HttpFirmwareUpgrade>"
						"<tt:HttpSystemBackup>false</tt:HttpSystemBackup>"
						"<tt:HttpSystemLogging>true</tt:HttpSystemLogging>"
						"<tt:HttpSupportInformation>true</tt:HttpSupportInformation>"
					"</tt:Extension>"
				"</tt:System>"
				"<tt:IO>"
					"<tt:InputConnectors>0</tt:InputConnectors>"
					"<tt:RelayOutputs>1</tt:RelayOutputs>"
				"</tt:IO>"
				"<tt:Security>"
					"<tt:TLS1.1>true</tt:TLS1.1>"
					"<tt:TLS1.2>true</tt:TLS1.2>"
					"<tt:OnboardKeyGeneration>false</tt:OnboardKeyGeneration>"
					"<tt:AccessPolicyConfig>false</tt:AccessPolicyConfig>"
					"<tt:X.509Token>false</tt:X.509Token>"
					"<tt:SAMLToken>false</tt:SAMLToken>"
					"<tt:KerberosToken>false</tt:KerberosToken>"
					"<tt:RELToken>false</tt:RELToken>"
				"</tt:Security>"
			"</tt:Device>",device_xaddr);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}

	if (category == CAP_CATEGORY_ALL || category == CAP_CATEGORY_EVENTS) {
		nRet = sprintf(temp,
			"<tt:Events>"
				"<tt:XAddr>%s</tt:XAddr>"
				"<tt:WSSubscriptionPolicySupport>true</tt:WSSubscriptionPolicySupport>"
				"<tt:WSPullPointSupport>true</tt:WSPullPointSupport>"
				"<tt:WSPausableSubscriptionManagerInterfaceSupport>false</tt:WSPausableSubscriptionManagerInterfaceSupport>"
			"</tt:Events>",device_xaddr);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}

	if (category == CAP_CATEGORY_ALL || category == CAP_CATEGORY_IMAGE) {
		nRet = sprintf(temp,
			"<tt:Imaging>"
				"<tt:XAddr>%s</tt:XAddr>"
			"</tt:Imaging>",device_xaddr);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}

	if (category == CAP_CATEGORY_ALL || category == CAP_CATEGORY_MEDIA) {
		nRet = sprintf(temp,
			"<tt:Media>"
				"<tt:XAddr>%s</tt:XAddr>"
				"<tt:StreamingCapabilities>"
					"<tt:RTPMulticast>true</tt:RTPMulticast>"
					"<tt:RTP_TCP>true</tt:RTP_TCP>"
					"<tt:RTP_RTSP_TCP>true</tt:RTP_RTSP_TCP>" 
				"</tt:StreamingCapabilities>" 
			"</tt:Media>",device_xaddr);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}

	if (category == CAP_CATEGORY_ALL || category == CAP_CATEGORY_MEDIA) {
		nRet = sprintf(temp, "<tt:PTZ><tt:XAddr>%s</tt:XAddr></tt:PTZ>",device_xaddr);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}
	
	nRet = sprintf(temp,
		"<tt:Extension>"
			"<tt:DeviceIO>"
				"<tt:XAddr>%s</tt:XAddr>"
				"<tt:VideoSources>1</tt:VideoSources>"
				"<tt:VideoOutputs>1</tt:VideoOutputs>"
				"<tt:AudioSources>1</tt:AudioSources>"
				"<tt:AudioOutputs>1</tt:AudioOutputs>"
				"<tt:RelayOutputs>1</tt:RelayOutputs>"
			"</tt:DeviceIO>"
		"</tt:Extension>",device_xaddr);
	if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));	
	strncat(body,"</tds:Capabilities></tds:GetCapabilitiesResponse>",strlen("</tds:Capabilities></tds:GetCapabilitiesResponse>"));

	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
 int onvifGetCapabilities(ONVIF_STRUCT *onvif,XMLN *data)
{
	CATEGORY_TYPE category;
	ERROR_MSG errMsg;
	//ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
    category = getCapabilitiesCategory(data);
	if (category == CAP_CATEGORY_INVALID)  {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	else ERROR_LOG("category:%d\n",(int) category);
	return buildResponseXml(onvif, buildResponseGetCapabilities, &category);
}

/*
 *	GetProfiles
 */
int buildResponseGetProfiles(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
 {
 	char temp[1024*2];
	char body[1024*16];
	int nRet;
	int i;
	SDK_AVPARAM_GET_RES		sAvParamGetRes;
	const PTZConfig *ptzCfg = NULL;
	const MetaConfig *MetaCfg = NULL;
	
	nRet = sprintf(onvif->action,actionFormat,"http://www.onvif.org/ver10/media/wsdlGetProfile/");
	if (nRet < 0) return -1;
	
	nRet = getVideoParam(HFALSE, &sAvParamGetRes);
	if (nRet < 0) return -1;


	nRet = sprintf(body,"<trt:GetProfilesResponse>");
	if (nRet < 0) return -1;

	for (i=0; i < sAvParamGetRes.nVChnNum; i++)
	{
		nRet = sprintf(temp,
			"<trt:Profiles fixed=\"true\" token=\"Profile_token%d\">"
			"<tt:Name>Profile%d</tt:Name>",
			i+1,i+1);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));

		// VideoSourceConfig
		nRet = sprintf(temp,
			"<tt:VideoSourceConfiguration token=\"VideoSource_cfg_token%d\">"
	        	"<tt:Name>VideoSource_cfg%d</tt:Name>"
	        	"<tt:UseCount>%d</tt:UseCount>"
	        	"<tt:SourceToken>VideoSource_token1</tt:SourceToken>"
	         	"<tt:Bounds height=\"%d\" width=\"%d\" y=\"1\" x=\"1\"></tt:Bounds>"
         	"</tt:VideoSourceConfiguration>",
			i+1, i+1, MAX_ONLINE_USER, sAvParamGetRes.sVideParam[i].nHeight, sAvParamGetRes.sVideParam[i].nWidth);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));

		//AudioSourceConfiguration
		if(sAvParamGetRes.sVideParam[i].bWithAudio) {
			nRet = sprintf(temp,
				"<tt:AudioSourceConfiguration token=\"AudioSource_cfg_token%d\">"
		        	"<tt:Name>AudioSource_cfg%d</tt:Name>"
		        	"<tt:UseCount>%d</tt:UseCount>"
		         	"<tt:SourceToken>VideoSource_token1</tt:SourceToken>"
	        	"</tt:AudioSourceConfiguration>",
				i+1, i+1, MAX_ONLINE_USER);
			if (nRet < 0) return -1;
			strncat(body,temp,strlen(temp));
		}
		
		//VideoEncoderConfiguration	
		nRet = sprintf(temp,
			"<tt:VideoEncoderConfiguration token=\"H264Video_cfg_token%d\">"
        	"<tt:Name>H264Video_cfg%d</tt:Name>"
            "<tt:UseCount>%d</tt:UseCount>"
            "<tt:Encoding>H264</tt:Encoding>"
            "<tt:Resolution>"
	            "<tt:Width>%d</tt:Width>"
	            "<tt:Height>%d</tt:Height>"
            "</tt:Resolution>"
            "<tt:Quality>6</tt:Quality>"
            "<tt:RateControl>"
	            "<tt:FrameRateLimit>%d</tt:FrameRateLimit>"
	            "<tt:EncodingInterval>%d</tt:EncodingInterval>"
	            "<tt:BitrateLimit>%d</tt:BitrateLimit>"
            "</tt:RateControl>"
            "<tt:H264>"
	            "<tt:GovLength>0</tt:GovLength>"
	            "<tt:H264Profile>%s</tt:H264Profile>"
            "</tt:H264>"
            "<tt:Multicast/>"
            "<tt:SessionTimeout>PT0H10M0S</tt:SessionTimeout>"
            "</tt:VideoEncoderConfiguration>",
			i+1, i+1, MAX_ONLINE_USER, sAvParamGetRes.sVideParam[i].nWidth, sAvParamGetRes.sVideParam[i].nHeight,
			sAvParamGetRes.sVideParam[i].nFps, sAvParamGetRes.sVideParam[i].nGop, sAvParamGetRes.sVideParam[i].nKBps,
#ifdef RTS390XN
			"Main");
#else
			"High");
#endif
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));

		//AudioEncoderConfiguration
		nRet = sprintf(temp,
			"<tt:AudioEncoderConfiguration token=\"AACAudio_cfg_token%d\">"
        	"<tt:Name>AACAudio_cfg%d</tt:Name>"
         	"<tt:UseCount>%d</tt:UseCount>"
        	"<tt:Encoding>AAC</tt:Encoding>"
        	"<tt:Bitrate>%d</tt:Bitrate>"
         	"<tt:SampleRate>%d</tt:SampleRate>"
        	"<tt:Multicast/>"
        	"<tt:SessionTimeout>PT0H10M0S</tt:SessionTimeout>"
         	"</tt:AudioEncoderConfiguration>",
			i+1, i+1, MAX_ONLINE_USER, sAvParamGetRes.sAudioParam.nKBPS, sAvParamGetRes.sAudioParam.nSampleRate);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	
		//PTZConfiguration
		ptzCfg = getPtzCfg();
		if (ptzCfg == NULL) return -1;
		nRet = sprintf(temp,
			"<tt:PTZConfiguration token=\"%s\">"
				"<tt:Name>%s</tt:Name>"
				"<tt:UseCount>%d</tt:UseCount>"
				"<tt:NodeToken>%s</tt:NodeToken>"
				"<tt:DefaultAbsolutePantTiltPositionSpace>%s</tt:DefaultAbsolutePantTiltPositionSpace>"
				"<tt:DefaultAbsoluteZoomPositionSpace>%s</tt:DefaultAbsoluteZoomPositionSpace>"
				"<tt:DefaultRelativePanTiltTranslationSpace>%s</tt:DefaultRelativePanTiltTranslationSpace>"
				"<tt:DefaultRelativeZoomTranslationSpace>%s</tt:DefaultRelativeZoomTranslationSpace>"
				"<tt:DefaultContinuousPanTiltVelocitySpace>%s</tt:DefaultContinuousPanTiltVelocitySpace>"
				"<tt:DefaultContinuousZoomVelocitySpace>%s</tt:DefaultContinuousZoomVelocitySpace>"
				"<tt:DefaultPTZSpeed>"
					"<tt:PanTilt space=\"%s\" y=\"%0.1f\" x=\"%0.1f\"></tt:PanTilt>"
					"<tt:Zoom space=\"%s\" x=\"%0.1f\"></tt:Zoom>"
				"</tt:DefaultPTZSpeed>"
				"<tt:DefaultPTZTimeout>PT0H1M0S</tt:DefaultPTZTimeout>"
				"<tt:PanTiltLimits>"
					"<tt:Range>"
					"<tt:URI>%s</tt:URI>"
					"<tt:XRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
					"</tt:XRange>"
					"<tt:YRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
					"</tt:YRange>"
					"</tt:Range>"
				"</tt:PanTiltLimits>"
				"<tt:ZoomLimits>"
				"<tt:Range>"
					"<tt:URI>%s</tt:URI>"
					"<tt:XRange>"
						"<tt:Min>%0.1f</tt:Min>"
						"<tt:Max>%0.1f</tt:Max>"
					"</tt:XRange>"
				"</tt:Range>"
				"</tt:ZoomLimits>"
			"</tt:PTZConfiguration>",
			ptzCfg->token, ptzCfg->name, ptzCfg->usecnt, ptzCfg->nodetoken, ptzCfg->abs_pant_space, ptzCfg->abs_zoom_space,
			ptzCfg->rel_pant_space, ptzCfg->rel_zoom_space, ptzCfg->cont_pant_space, ptzCfg->cont_zoom_space,
			ptzCfg->speed_pant_space, ptzCfg->speed_pant_x, ptzCfg->speed_pant_y, ptzCfg->speed_zoom_space, ptzCfg->speed_zoom_x,
			ptzCfg->limit_pant_space, ptzCfg->limit_pant_x.min, ptzCfg->limit_pant_x.max, ptzCfg->limit_pant_y.min, ptzCfg->limit_pant_y.min,
			ptzCfg->limit_zoom_space, ptzCfg->limit_zoom_x.min, ptzCfg->limit_zoom_x.max);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));

		//MetadataConfiguration
		MetaCfg = getMetaCfg();
		if (MetaCfg == NULL) return -1;
		
		nRet = sprintf(temp,
			"<tt:MetadataConfiguration token=\"%s\">"
            "<tt:Name>%s</tt:Name>"
            "<tt:UseCount>%d</tt:UseCount>"
            "<tt:PTZStatus>"
            "<tt:Status>%s</tt:Status>"
            "<tt:Position>%s</tt:Position>"
            "</tt:PTZStatus>"
            "<tt:Events></tt:Events>"
            "<tt:Multicast/>"
            "<tt:SessionTimeout>%s</tt:SessionTimeout>"
            "</tt:MetadataConfiguration>",
            MetaCfg->token, MetaCfg->name, MetaCfg->usecnt, MetaCfg->ptz_status, MetaCfg->ptz_position, MetaCfg->timeout);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));

		strncat(body,"</trt:Profiles>",strlen("</trt:Profiles>"));
	}

	strncat(body,"</trt:GetProfilesResponse>",strlen("</trt:GetProfilesResponse>"));
	
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
 }
int onvifGetProfiles(ONVIF_STRUCT *onvif,XMLN *data)
{
	//ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetProfiles, NULL);
}

/*
 *	GetServices
 */
int buildResponseGetServices(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char temp[1024];
	char body[1024*8];
	char chIpAddr[32] = {0};
	int nRet;
	int nPort = -1;
	int i;
	bool capabilityFlag = *(bool *)arg;
	ERROR_LOG("flag:%d\n",capabilityFlag);
	const int serviceNum = 1;//6;
	SDK_AVPARAM_GET_RES		sAvParamGetRes;
	const char *xmlServerNum[7] = { "tds", "trt", "tev", "timg", "tmd", "tan", "tptz" };
	const char *xmlServerWsdl[7] = { "http://www.onvif.org/ver10/device/wsdl", 
								  		  "http://www.onvif.org/ver10/media/wsdl",
								          "http://www.onvif.org/ver10/events/wsdl",
								          "http://www.onvif.org/ver20/imaging/wsdl",
								          "http://www.onvif.org/ver10/deviceIO/wsdl",
								          "http://www.onvif.org/ver20/analytics/wsdl",
								          "http://www.onvif.org/ver20/ptz/wsdl"};
	nRet = getLocalIp(HFALSE, chIpAddr);
	if (nRet < 0) return -1;
	nPort = getOnvifPort(HFALSE);// hySysCfgGet()->sOnvifParam.nOnvifPort;
	if (nPort < 0) return -1;
	nRet = getVideoParam(HFALSE, &sAvParamGetRes);
	if (nRet < 0) return -1;
	nRet = sprintf(body,"<tds:GetServicesResponse>");
	if(nRet < 0) return -1;
	for (i=0; i <= serviceNum; i++) {
		nRet = sprintf(temp,
			"<tds:Service>"
    		"<tds:Namespace>%s</tds:Namespace>"
    		"<tds:XAddr>http://%s:%d/onvif/device_service</tds:XAddr>",
    		xmlServerWsdl[i],chIpAddr,nPort);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));

		// if (capabilityFlag) {
		// 	if (i == 0) { // device
		// 		nRet = sprintf(temp,
		// 		    "<tds:Capabilities>"
		// 		    	"<tds:Network NTP=\"3\" HostnameFromDHCP=\"true\" Dot11Configuration=\"true\" DynDNS=\"false\" IPVersion6=\"false\" ZeroConfiguration=\"false\" IPFilter=\"false\"></tds:Network>"
		// 				"<tds:Security RELToken=\"false\" HttpDigest=\"false\" UsernameToken=\"true\" KerberosToken=\"false\" SAMLToken=\"flase\" X.509Token=\"false\" RemoteUserHandling=\"false\" Dot1X=\"false\" DefaultAccessPolicy=\"false\" AccessPolicyConfig=\"false\" OnboardKeyGeneration=\"false\" TLS1.2=\"true\" TLS1.1=\"true\" TLS1.0=\"false\"></tds:Security>"
		// 				"<tds:System HttpSupportInformation=\"false\" HttpSystemLogging=\"false\" HttpSystemBackup=\"false\" HttpFirmwareUpgrade=\"false\" FirmwareUpgrade=\"false\" SystemLogging=\"true\" SystemBackup=\"true\" RemoteDiscovery=\"false\" DiscoveryBye=\"false\" DiscoveryResolve=\"false\"></tds:System>"
		// 			"</tds:Capabilities>");
		// 		if (nRet < 0) return -1;
		// 		strncat(body,temp,strlen(temp));
		// 	} else if (i == 1) { // media
		// 		nRet = sprintf(temp,
		// 		    "<trt:Capabilities>"
		// 				"<trt:ProfileCapabilities MaximumNumberOfProfiles=\"%d\"></trt:ProfileCapabilities>"
		// 				"<trt:StreamingCapabilities RTP_RTSP_TCP=\"true\" RTP_TCP=\"true\" RTPMulticast=\"true\"></trt:StreamingCapabilities>"
		// 			"</trt:Capabilities>",
		// 			sAvParamGetRes.nVChnNum);
		// 		if (nRet < 0) return -1;
		// 		strncat(body,temp,strlen(temp));
		// 	} else if (i == 2) { // event
		// 		nRet = sprintf(temp, "<tev:Capabilities WSPausableSubscriptionManagerInterfaceSupport=\"false\" WSPullPointSupport=\"true\" WSSubscriptionPolicySupport=\"true\"></tev:Capabilities>");
		// 		if (nRet < 0) return -1;
		// 		strncat(body,temp,strlen(temp));
		// 	} else if (i == 5) { // analytics
		// 		nRet = sprintf(temp,
		// 		    "<tds:Capabilities>"
		// 		    	"<tan:Analytics>"
		// 		    	"<tan:RuleSupport>\"false\"</tan:RuleSupport>"
		// 		    	"<tan:AnalyticsModuleSupport>\"false\"</tan:AnalyticsModuleSupport>"
		// 		    	"</tan:Analytics>"
		// 		    "</tds:Capabilities>");
		// 		if (nRet < 0) return -1;
		// 		strncat(body,temp,strlen(temp));
		// 	} else { 
		// 		nRet = sprintf(temp,
		// 			"<%s:Capabilities></%s:Capabilities>",
		// 			xmlServerNum[i],xmlServerNum[i]);
		// 	}
		// } 

		nRet = sprintf(temp,
			"<tds:Version>"
     			"<tt:Major>0</tt:Major>"
     			"<tt:Minor>3</tt:Minor>"
    		"</tds:Version>"
    		"</tds:Service>");
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}

	strncat(body,"</tds:GetServicesResponse>",strlen("</tds:GetServicesResponse>"));
	
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetServices(ONVIF_STRUCT *onvif,XMLN *data)
{
	bool capabilityFlag = false;
	XMLN *includeCapability = NULL;
	ERROR_MSG errMsg;
		
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
	includeCapability = xml_node_soap_get(data, "IncludeCapability");
	if (includeCapability == NULL || includeCapability->data == NULL)  {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	ERROR_LOG("flag:%s\n", includeCapability->data);
	if (strcmp(includeCapability->data, "true") == 0) capabilityFlag = true;
	else capabilityFlag = false;
	
	return buildResponseXml(onvif, buildResponseGetServices, &capabilityFlag);
}

/*
 *	getServiceCapacitties
 */

int buildResponseGetTdsServiceCapabilities(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;
	
	nRet = sprintf(body,
		"<tds:GetServiceCapabilitiesResponse>"
			"<tds:Capabilities>"
				"<tds:Network NTP=\"3\" HostnameFromDHCP=\"true\" Dot11Configuration=\"true\" DynDNS=\"false\" IPVersion6=\"false\" ZeroConfiguration=\"false\" IPFilter=\"false\"></tds:Network>"
				"<tds:Security RELToken=\"false\" HttpDigest=\"false\" UsernameToken=\"true\" KerberosToken=\"false\" SAMLToken=\"false\" X.509Token=\"false\" RemoteUserHandling=\"false\" Dot1X=\"false\" DefaultAccessPolicy=\"false\" AccessPolicyConfig=\"false\" OnboardKeyGeneration=\"false\" TLS1.2=\"true\" TLS1.1=\"true\" TLS1.0=\"false\"></tds:Security>"
				"<tds:System HttpSupportInformation=\"false\" HttpSystemLogging=\"false\" HttpSystemBackup=\"false\" HttpFirmwareUpgrade=\"false\" FirmwareUpgrade=\"true\" SystemLogging=\"true\" SystemBackup=\"false\" RemoteDiscovery=\"false\" DiscoveryBye=\"false\" DiscoveryResolve=\"false\"></tds:System>"
			"</tds:Capabilities>"
		"</tds:GetServiceCapabilitiesResponse>");
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetTdsServiceCapabilities(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);

	return buildResponseXml(onvif, buildResponseGetTdsServiceCapabilities, NULL);
}

/*
 * GetVideoSource
 */

int buildResponseGetVideoSources(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char temp[1024];
	char body[1024*2];
	int i;
	
	SDK_AVPARAM_GET_RES	sAvParamGetRes;
	int nRet;
	//printf("1\n");
	nRet = getVideoParam(HFALSE, &sAvParamGetRes);
	if (nRet < 0) return -1;
	sAvParamGetRes.nVChnNum = 1;
	//printf("3\n");
	nRet = sprintf(body,"<trt:GetVideoSourcesResponse>");
	if(nRet < 0) return -1;
	
	for(i=0; i < sAvParamGetRes.nVChnNum; i++) {
		nRet = sprintf(temp,
			"<trt:VideoSources token=\"VideoSource_token%d\">"
                "<tt:Framerate>%d</tt:Framerate>"
                "<tt:Resolution>"
                    "<tt:Width>%d</tt:Width>"
                    "<tt:Height>%d</tt:Height>"
                "</tt:Resolution>"
            "</trt:VideoSources>",
            i+1, sAvParamGetRes.sVideParam[i].nFps, sAvParamGetRes.sVideParam[i].nWidth, sAvParamGetRes.sVideParam[i].nHeight);
		if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}
	strncat(body,"</trt:GetVideoSourcesResponse>", strlen("</trt:GetVideoSourcesResponse>"));

	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetVideoSources(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetVideoSources, NULL);
}

/*
 *	GetSnapshotUri
 */

#define SNAP_BUFFER_LEN					(1024*100)
#define SNAP_FILE_PATH					"/tmp/onvifsnap.jpg"

int onvifSnapJpeg(int nStreamIndex)
{
	return -1;
	HCHAR	*cSnapBuffer = (HCHAR*)malloc(SNAP_BUFFER_LEN);
	int nPicSize = -1; //coreSnapShotImage(nStreamIndex,cSnapBuffer,SNAP_BUFFER_LEN);//抓拍


	#if 0
	ERROR_LOG("====================get_motion_snapshot=====================\n");
  	int ret = CaptureSnapshotGetBuffer(cSnapBuffer, SNAP_BUFFER_LEN, 5);
	if( ret <= 0 )
	{
		ERROR_LOG("snapshot err!!!\n");
		return nPicSize;
	}
	nPicSize = ret;
	ERROR_LOG("====================get_motion_snapshot ok=====================\n");

	CaptureSnapshotReleaseBuffer();
	#endif


	HINT32 nRet = nPicSize; 
	if(nPicSize < 0)
		ERROR_LOG("Snap pic failed :%d \n",nPicSize);
	else
	{
		FILE * fp = fopen(SNAP_FILE_PATH,"w+b");
		if(!fp)
		{
			ERROR_LOG("open file :%s failed,errno:%d \n",SNAP_FILE_PATH,errno);
			nRet = HY_ErrOpen;
		}
		else
		{
				int nWSize = fwrite(cSnapBuffer,1,nPicSize,fp);
				if(nWSize != nPicSize)
				{
					ERROR_LOG("write file failed:%d,%d,errno:%d \n",nWSize,nPicSize,errno);
					nRet = HY_ErrSize;
				}
				fclose(fp);
		}
	}
	free(cSnapBuffer);
	return nRet;
}
int buildResponseGetSnapshotUri(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	return -1;
	char body[1024];
	int nRet;
	char localIp[32];
	int portNum;
	int nStreamIndex;

	nStreamIndex = *(int *)arg -1;
	//ERROR_LOG("index:%d\n", nStreamIndex);
	nRet = getLocalIp(HFALSE, localIp);
	if (nRet < 0) return -1;
	portNum = GetHttpPort(HFALSE);
	
	nRet = sprintf(body,
		"<trt:GetSnapshotUriResponse>"
    		"<trt:MediaUri>"
#if 1  		
                "<tt:Uri>http://%s:%d/"ONVIF_SNAP_URI"?chn=%d</tt:Uri>"
#else
				"<tt:Uri>http://%s:%d/mjpeg/snap.cgi?chn=%d</tt:Uri>"
#endif
                "<tt:InvalidAfterConnect>false</tt:InvalidAfterConnect>"
                "<tt:InvalidAfterReboot>false</tt:InvalidAfterReboot>"
                "<tt:Timeout>PT0H50M0S</tt:Timeout>"
            "</trt:MediaUri>"
        "</trt:GetSnapshotUriResponse>",
        localIp, portNum, nStreamIndex);
	if (nRet < 0) return -1;
	onvifSnapJpeg(nStreamIndex);
	//sendSnapJpeg(onvif,1);
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetSnapshotUri(ONVIF_STRUCT *onvif,XMLN *data)
{
	return -1;
	int index=0;
	XMLN *token = NULL;
	int tokenLen;
	ERROR_MSG errMsg;
	//ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
	token = xml_node_soap_get(data, "ProfileToken");
	if (token == NULL || token->data == NULL)  {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	//ERROR_LOG("name:%s,token:%s\n", token->name, token->data);
	tokenLen = strlen(token->data);
	index = token->data[tokenLen-1] - '0';
	return buildResponseXml(onvif, buildResponseGetSnapshotUri, &index);
}

/*
 *	GetProfile
 */
int buildResponseGetProfile(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char temp[1024*4];
	char body[1024*8];
	int nRet;
	int index = *(int *)arg;
	int chn;
	SDK_AVPARAM_GET_RES		sAvParamGetRes;
	const PTZConfig *ptzCfg = NULL;
	const MetaConfig *MetaCfg = NULL;
	
	nRet = getVideoParam(HFALSE, &sAvParamGetRes);

	if (nRet < 0) return -1;

	if ((index > 0) && (index <= sAvParamGetRes.nVChnNum)) chn = index-1;
	else return -1;
	
	nRet = sprintf(body,
		"<trt:GetProfileResponse>"
        "<trt:Profile fixed=\"true\" token=\"Profile_token%d\">"
        "<tt:Name>Profile%d</tt:Name>",
        index, index);
	if (nRet < 0) return -1;
	
	// videoSource
	nRet = sprintf(temp,
		"<tt:VideoSourceConfiguration token=\"VideoSource_cfg_token%d\">"
			"<tt:Name>VideoSource_cfg%d</tt:Name>"
			"<tt:UseCount>%d</tt:UseCount>"
			"<tt:SourceToken>VideoSource_token%d</tt:SourceToken>"
			"<tt:Bounds height=\"%d\" width=\"%d\" y=\"1\" x=\"1\"></tt:Bounds>"
		"</tt:VideoSourceConfiguration>",
        index , index, MAX_ONLINE_USER, index,
        sAvParamGetRes.sVideParam[chn].nHeight, sAvParamGetRes.sVideParam[chn].nWidth);
	if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));

	// audioSource
	if (sAvParamGetRes.sVideParam[chn].bWithAudio) {
		nRet = sprintf(temp,
			"<tt:AudioSourceConfiguration token=\"AudioSource_cfg_token%d\">"
				"<tt:Name>AudioSource_cfg%d</tt:Name>"
				"<tt:UseCount>%d</tt:UseCount>"
				"<tt:SourceToken>AudioSource_token%d</tt:SourceToken>"
			"</tt:AudioSourceConfiguration>",
	        index , index, MAX_ONLINE_USER, index);
	        if (nRet < 0) return -1;
		strncat(body,temp,strlen(temp));
	}

	// videoEncode	
	nRet = sprintf(temp,
		"<tt:VideoEncoderConfiguration token=\"H264Video_cfg_token%d\">"
			"<tt:Name>H264Video_cfg%d</tt:Name>"
			"<tt:UseCount>%d</tt:UseCount>"
			"<tt:Encoding>H264</tt:Encoding>"
			"<tt:Resolution>"
				"<tt:Width>%d</tt:Width>"
				"<tt:Height>%d</tt:Height>"
				"</tt:Resolution>"
			"<tt:Quality>6</tt:Quality>"
			"<tt:RateControl>"
				"<tt:FrameRateLimit>%d</tt:FrameRateLimit>"
				"<tt:EncodingInterval>1</tt:EncodingInterval>"
				"<tt:BitrateLimit>%d</tt:BitrateLimit>"
			"</tt:RateControl>"
			"<tt:H264>"
				"<tt:GovLength>%d</tt:GovLength>"
#ifdef RTS390XN
				"<tt:H264Profile>Main</tt:H264Profile>"
#else
				"<tt:H264Profile>High</tt:H264Profile>"
#endif
			"</tt:H264>"
			"<tt:Multicast/>"
			"<tt:SessionTimeout>PT0H20M0S</tt:SessionTimeout>"
		"</tt:VideoEncoderConfiguration>",
        index , index, MAX_ONLINE_USER,
        sAvParamGetRes.sVideParam[chn].nWidth, sAvParamGetRes.sVideParam[chn].nHeight,
        sAvParamGetRes.sVideParam[chn].nFps, sAvParamGetRes.sVideParam[chn].nKBps, sAvParamGetRes.sVideParam[chn].nGop);
	if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));

	// audioEncode	
	nRet = sprintf(temp,
		"<tt:AudioEncoderConfiguration token=\"AACAudio_cfg_token%d\">"
			"<tt:Name>AACAudio_cfg%d</tt:Name>"
			"<tt:UseCount>%d</tt:UseCount>"
			"<tt:Encoding>AAC</tt:Encoding>"
			"<tt:Bitrate>%d</tt:Bitrate>"
			"<tt:SampleRate>%d</tt:SampleRate>"
			"<tt:Multicast/>"
			"<tt:SessionTimeout>PT0H20M0S</tt:SessionTimeout>"
		"</tt:AudioEncoderConfiguration>",
        index , index, MAX_ONLINE_USER,
        sAvParamGetRes.sAudioParam.nKBPS, sAvParamGetRes.sAudioParam.nSampleRate);
	if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));

	// PTZ
	ptzCfg = getPtzCfg();
	if (ptzCfg == NULL) return -1;
	
	nRet = sprintf(temp,
		"<tt:PTZConfiguration token=\"%s\">"
			"<tt:Name>%s</tt:Name>"
			"<tt:UseCount>%d</tt:UseCount>"
			"<tt:NodeToken>%s</tt:NodeToken>"
			"<tt:DefaultAbsolutePantTiltPositionSpace>%s</tt:DefaultAbsolutePantTiltPositionSpace>"
			"<tt:DefaultAbsoluteZoomPositionSpace>%s</tt:DefaultAbsoluteZoomPositionSpace>"
			"<tt:DefaultRelativePanTiltTranslationSpace>%s</tt:DefaultRelativePanTiltTranslationSpace>"
			"<tt:DefaultRelativeZoomTranslationSpace>%s</tt:DefaultRelativeZoomTranslationSpace>"
			"<tt:DefaultContinuousPanTiltVelocitySpace>%s</tt:DefaultContinuousPanTiltVelocitySpace>"
			"<tt:DefaultContinuousZoomVelocitySpace>%s</tt:DefaultContinuousZoomVelocitySpace>"
			"<tt:DefaultPTZSpeed>"
				"<tt:PanTilt space=\"%s\" y=\"%0.1f\" x=\"%0.1f\"></tt:PanTilt>"
				"<tt:Zoom space=\"%s\" x=\"%0.1f\"></tt:Zoom>"
			"</tt:DefaultPTZSpeed>"
			"<tt:DefaultPTZTimeout>PT0H1M0S</tt:DefaultPTZTimeout>"
			"<tt:PanTiltLimits>"
				"<tt:Range>"
					"<tt:URI>%s</tt:URI>"
					"<tt:XRange>"
						"<tt:Min>%0.1f</tt:Min>"
						"<tt:Max>%0.1f</tt:Max>"
					"</tt:XRange>"
					"<tt:YRange>"
						"<tt:Min>%0.1f</tt:Min>"
						"<tt:Max>%0.1f</tt:Max>"
					"</tt:YRange>"
				"</tt:Range>"
			"</tt:PanTiltLimits>"
			"<tt:ZoomLimits>"
				"<tt:Range>"
				"<tt:URI>%s</tt:URI>"
				"<tt:XRange>"
				"<tt:Min>%0.1f</tt:Min>"
				"<tt:Max>%0.1f</tt:Max>"
				"</tt:XRange>"
				"</tt:Range>"
			"</tt:ZoomLimits>"
		"</tt:PTZConfiguration>",
        ptzCfg->token, ptzCfg->name, ptzCfg->usecnt, ptzCfg->nodetoken, ptzCfg->abs_pant_space, ptzCfg->abs_zoom_space,
		ptzCfg->rel_pant_space, ptzCfg->rel_zoom_space, ptzCfg->cont_pant_space, ptzCfg->cont_zoom_space,
		ptzCfg->speed_pant_space, ptzCfg->speed_pant_x, ptzCfg->speed_pant_y, ptzCfg->speed_zoom_space, ptzCfg->speed_zoom_x,
		ptzCfg->limit_pant_space, ptzCfg->limit_pant_x.min, ptzCfg->limit_pant_x.max, ptzCfg->limit_pant_y.min, ptzCfg->limit_pant_y.min,
		ptzCfg->limit_zoom_space, ptzCfg->limit_zoom_x.min, ptzCfg->limit_zoom_x.max);
	if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));

	//MetaData
	MetaCfg = getMetaCfg();
	if (MetaCfg == NULL) return -1;
	
	nRet = sprintf(temp,
		"<tt:MetadataConfiguration token=\"%s\">"
			"<tt:Name>%s</tt:Name>"
			"<tt:UseCount>%d</tt:UseCount>"
			"<tt:PTZStatus>"
				"<tt:Status>%s</tt:Status>"
				"<tt:Position>%s</tt:Position>"
			"</tt:PTZStatus>"
			"<tt:Events></tt:Events>"
			"<tt:Multicast/>"
			"<tt:SessionTimeout>%s</tt:SessionTimeout>"
		"</tt:MetadataConfiguration>",
        MetaCfg->token, MetaCfg->name, MetaCfg->usecnt, MetaCfg->ptz_status, MetaCfg->ptz_position, MetaCfg->timeout);
    if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));

	strncat(body,"</trt:Profile></trt:GetProfileResponse>",strlen("</trt:Profile></trt:GetProfileResponse>"));
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetProfile(ONVIF_STRUCT *onvif,XMLN *data)
{
	int index=0;
	XMLN *token = NULL;
	int tokenLen;
	ERROR_MSG errMsg;
	//ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
	token = xml_node_soap_get(data, "ProfileToken");
	if (token == NULL || token->data == NULL)  {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	//ERROR_LOG("name:%s,token:%s\n", token->name, token->data);
	tokenLen = strlen(token->data);
	index = token->data[tokenLen-1] - '0';
	return buildResponseXml(onvif, buildResponseGetProfile, &index);
}

/*
 *	GetStreamUri
 */
int buildResponseGetStreamUri(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	STREAMURI_DATA *sStreamData = (STREAMURI_DATA *)arg;
	int nRet;
	char body[1024];
	char uriStr[MEDIA_URL_LEN];
	char uriFilePath[32];
	char serveIp[32];
	char token[256];
	int nRtspPort;
	int nStreamIndex = sStreamData->index - 1;

	if (nStreamIndex < 0) return -1;
	nRtspPort = getRtspPort(HTRUE);
	if (nRtspPort < 0) nRtspPort = 554;
	nRet = getLocalIp(HFALSE, serveIp);
	if (nRet < 0) return -1;
	nRet = GenRtspToken(token,sizeof(token));
	if (nRet < 0) return -1;
	
	nRet = sprintf(uriFilePath,"live/ch%d", nStreamIndex);
	if (nRet < 0) return -1;
	//测试rtsp  rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov

	#if 0
	if (strcmp(sStreamData->Protocol,"UDP") == 0) {
		sprintf(uriStr,"rtsp://%s:%d/%s?token=%s",serveIp,nRtspPort, uriFilePath,token);
	} else if (strcmp(sStreamData->Protocol,"TCP") == 0) {
		sprintf(uriStr,"rtsp://%s:%d/%s?token=%s",serveIp,nRtspPort, uriFilePath,token);
	} else if (strcmp(sStreamData->Protocol,"RTSP") == 0) {
		sprintf(uriStr,"rtsp://%s:%d/%s?token=%s",serveIp,nRtspPort, uriFilePath,token);
	} else if (strcmp(sStreamData->Protocol,"HTTP") == 0) {
		sprintf(uriStr,"rtsp://%s:%d/%s?token=%s",serveIp,nRtspPort, uriFilePath,token);
	} else {
		sprintf(uriStr,"rtsp://%s:%d/%s?token=", serveIp, nRtspPort, uriFilePath,token);
	}
	#else
	if (0 == nStreamIndex)
	{
		sprintf(uriStr,"rtsp://%s:8554/main",serveIp);
	}
	else
	{
		sprintf(uriStr,"rtsp://%s:8554/sub",serveIp);
	}
	
	#endif

	nRet = sprintf(body,
		"<trt:GetStreamUriResponse>"
			"<trt:MediaUri>"
				"<tt:Uri>%s</tt:Uri>"
				"<tt:InvalidAfterConnect>false</tt:InvalidAfterConnect>"
				"<tt:InvalidAfterReboot>false</tt:InvalidAfterReboot>"
				"<tt:Timeout>PT0H50M0S</tt:Timeout>"
			"</trt:MediaUri>"
		"</trt:GetStreamUriResponse>",
        uriStr);
    if (nRet < 0) return -1;

	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetStreamUri(ONVIF_STRUCT *onvif,XMLN *data)
{
	STREAMURI_DATA sStreamData;
	int nRet;
	ERROR_MSG errMsg;
	//ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);

	memset(&sStreamData, 0, sizeof(sStreamData));

	nRet = getStreamUriData(data, &sStreamData);
	if (nRet < 0)  {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	//ERROR_LOG("index:%d,protocol:%s\n", sStreamData.index, sStreamData.Protocol);

	return buildResponseXml(onvif, buildResponseGetStreamUri, &sStreamData);
}
/*
 * GetVideoSourceConfiguration
 */
 int buildResponseGetVideoSourceConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;

	nRet = sprintf(body,
		"<trt:GetVideoSourceConfigurationResponse>"
        	"<trt:Configuration/>"
    	"</trt:GetVideoSourceConfigurationResponse>");
    if (nRet < 0) return -1;

	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetVideoSourceConfiguration(ONVIF_STRUCT *onvif,XMLN *data)
{
	return buildResponseXml(onvif, buildResponseGetVideoSourceConfiguration, NULL);
}
/*
 *	GetVideoEncoderConfiguration
 */

int buildResponseGetVideoEncoderConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char temp[1024*2];
	char body[1024*8];
	int chn = *(int *)arg;
	int nRet;
	SDK_AVPARAM_GET_RES		sAvParamGetRes;
	ERROR_MSG errMsg;

	do {
		nRet = getVideoParam(HTRUE, &sAvParamGetRes);
		if (nRet < 0) break;
		chn--;
		if (chn < 0 || chn >= sAvParamGetRes.nVChnNum) break;

		nRet = sprintf(body,
		"<trt:GetVideoEncoderConfigurationResponse>"
		"<trt:Configuration token=\"H264Video_cfg_token%d\">"
			"<tt:Name>H264Video_cfg%d</tt:Name>"
				"<tt:UseCount>4</tt:UseCount>"
				"<tt:Encoding>H264</tt:Encoding>"
			"<tt:Resolution>"
				"<tt:Width>%d</tt:Width>"
				"<tt:Height>%d</tt:Height>"
			"</tt:Resolution>"
				"<tt:Quality>6</tt:Quality>"
				"<tt:RateControl>"
				"<tt:FrameRateLimit>%d</tt:FrameRateLimit>"
				"<tt:EncodingInterval>1</tt:EncodingInterval>"
				"<tt:BitrateLimit>%d</tt:BitrateLimit>"
			"</tt:RateControl>"
			"<tt:H264>"
				"<tt:GovLength>%d</tt:GovLength>"
				"<tt:H264Profile>High</tt:H264Profile>"
			"</tt:H264>"
			"<tt:Multicast/>"
			"<tt:SessionTimeout>PT0H20M0S</tt:SessionTimeout>"
		"</trt:Configuration>"
		"</trt:GetVideoEncoderConfigurationResponse>",
		chn+1, chn+1, sAvParamGetRes.sVideParam[chn].nWidth, sAvParamGetRes.sVideParam[chn].nHeight,
		sAvParamGetRes.sVideParam[chn].nFps, sAvParamGetRes.sVideParam[chn].nKBps, sAvParamGetRes.sVideParam[chn].nGop);

		//填充报文
		nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
	} while(0);

	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifGetVideoEncoderConfiguration(ONVIF_STRUCT *onvif,XMLN *data)
{
	int index=0;
	XMLN *token = NULL;
	int tokenLen;
	ERROR_MSG errMsg;

	//ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
	token = xml_node_soap_get(data, "ConfigurationToken");
	if (token == NULL || token->data == NULL)  {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}

	//ERROR_LOG("name:%s,token:%s\n", token->name, token->data);
	tokenLen = strlen(token->data);
	index = token->data[tokenLen-1] - '0';
	return buildResponseXml(onvif, buildResponseGetVideoEncoderConfiguration, &index);
}
/*
 *	GetVideoEncoderConfigurations
 */
int buildResponseGetVideoEncoderConfigurations(ONVIF_STRUCT *onvif, HCHAR *xmlBuff,HINT32 len, void *arg)
{
	
	char temp[1024*2];
	char body[1024*8];
	int chn = *(HINT32 *)arg;
	int nRet;
	SDK_AVPARAM_GET_RES		sAvParamGetRes;
	ERROR_MSG errMsg;
	do {
		nRet = getVideoParam(HTRUE, &sAvParamGetRes);
		if (nRet < 0) break;

		nRet = sprintf(body,
			"<trt:GetVideoEncoderConfigurationsResponse>"
			"<trt:Configurations token=\"H264Video_cfg_token1\">"
			"<tt:Name>H264Video_cfg1</tt:Name>"
			"<tt:UseCount>4</tt:UseCount>"
			"<tt:Encoding>H264</tt:Encoding>"
			"<tt:Resolution>"
			"<tt:Width>%d</tt:Width>"
			"<tt:Height>%d</tt:Height>"
			"</tt:Resolution>"
			"<tt:Quality>6</tt:Quality>"
			"<tt:RateControl>"
			"<tt:FrameRateLimit>%d</tt:FrameRateLimit>"
			"<tt:EncodingInterval>1</tt:EncodingInterval>"
			"<tt:BitrateLimit>%d</tt:BitrateLimit>"
			"</tt:RateControl>"
			"<tt:H264>"
			"<tt:GovLength>%d</tt:GovLength>"
			"<tt:H264Profile>High</tt:H264Profile>"
			"</tt:H264>"
			"<tt:Multicast/>"
			"<tt:SessionTimeout>PT0H20M0S</tt:SessionTimeout>"
			"</trt:Configurations>"
			"<trt:Configurations token=\"H264Video_cfg_token2\">"
			"<tt:Name>H264Video_cfg2</tt:Name>"
			"<tt:UseCount>4</tt:UseCount>"
			"<tt:Encoding>H264</tt:Encoding>"
			"<tt:Resolution>"
			"<tt:Width>%d</tt:Width>"
			"<tt:Height>%d</tt:Height>"
			"</tt:Resolution>"
			"<tt:Quality>6</tt:Quality>"
			"<tt:RateControl>"
			"<tt:FrameRateLimit>%d</tt:FrameRateLimit>"
			"<tt:EncodingInterval>1</tt:EncodingInterval>"
			"<tt:BitrateLimit>%d</tt:BitrateLimit>"
			"</tt:RateControl>"
			"<tt:H264>"
			"<tt:GovLength>%d</tt:GovLength>"
			"<tt:H264Profile>Main</tt:H264Profile>"
			"</tt:H264>"
			"<tt:Multicast/>"
			"<tt:SessionTimeout>PT0H20M0S</tt:SessionTimeout>"
			"</trt:Configurations>"
			"</trt:GetVideoEncoderConfigurationsResponse>",
			sAvParamGetRes.sVideParam[0].nWidth, sAvParamGetRes.sVideParam[0].nHeight, sAvParamGetRes.sVideParam[0].nFps, 
			sAvParamGetRes.sVideParam[0].nKBps, sAvParamGetRes.sVideParam[0].nGop,
			sAvParamGetRes.sVideParam[1].nWidth, sAvParamGetRes.sVideParam[1].nHeight, sAvParamGetRes.sVideParam[1].nFps, 
			sAvParamGetRes.sVideParam[1].nKBps, sAvParamGetRes.sVideParam[1].nGop);
		//填充报文
		nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
	} while (0);
	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifGetVideoEncoderConfigurations(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetVideoEncoderConfigurations, NULL);
}
 
int buildResponseSetVideoEncoderConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;

	nRet = sprintf(body,"<trt:SetVideoEncoderConfigurationResponse></trt:SetVideoEncoderConfigurationResponse>");
    if (nRet < 0) return -1;

	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifSetVideoEncoderConfiguration(ONVIF_STRUCT *onvif,XMLN *data)
{
	int nRes;
	HUINT16 nFps;
	HUINT16 nKBps;
	int nChn;
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);

	nRes = getVideoEncParamFromXml(data, &nFps, &nKBps, &nChn);
	if (nRes < 0) {
		ERROR_LOG("get param of video encode error!\n");
		return -1;
	}

	nRes = setVideoEncodeParam(nFps,nKBps,nChn);
	if (nRes < 0) {
		ERROR_LOG("set param of video encode error!\n");
		return -1;
	}
	return buildResponseXml(onvif, buildResponseSetVideoEncoderConfiguration, NULL);
}

/*
 *	 GetVideoEncoderConfigurationOptions
 */
int buildResponseGetVideoEncoderConfigurationOptions(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char temp[1024*2];
	char body[1024*8];
	int chn = *(int *)arg;
	int nRet;
	HUINT16 nLimitFps[2] = {0,0};
	HUINT16 nLimitKBps[2] = {0,0};
	SDK_AVPARAM_GET_RES		sAvParamGetRes;

	nRet = getVideoParam(HTRUE, &sAvParamGetRes);
	if (nRet < 0) return -1;
	
	chn--;
	if (chn < 0 || chn >= sAvParamGetRes.nVChnNum) return -1;

	getVideoEncodeParam(nLimitFps, nLimitKBps, chn);

	nRet = sprintf(body, "<trt:GetVideoEncoderConfigurationOptionsResponse><trt:Options>");
    if (nRet < 0) return -1;

	// QualityRange
	nRet = sprintf(temp,
		"<tt:QualityRange>"
	        "<tt:Min>0</tt:Min>"
	        "<tt:Max>60</tt:Max>"
        "</tt:QualityRange>");
    if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));
	
	// H264
	nRet = sprintf(temp,
		"<tt:H264>"
			"<tt:ResolutionsAvailable>"
				"<tt:Width>%d</tt:Width>"
				"<tt:Height>%d</tt:Height>"
			"</tt:ResolutionsAvailable>"
			"<tt:GovLengthRange>"
				"<tt:Min>1</tt:Min>"
				"<tt:Max>120</tt:Max>"
			"</tt:GovLengthRange>"
			"<tt:FrameRateRange>"
				"<tt:Min>%d</tt:Min>"
				"<tt:Max>%d</tt:Max>"
			"</tt:FrameRateRange>"
			"<tt:EncodingIntervalRange>"
				"<tt:Min>1</tt:Min>"
				"<tt:Max>1</tt:Max>"
			"</tt:EncodingIntervalRange>"
			"<tt:H264ProfilesSupported>Baseline</tt:H264ProfilesSupported>"
			"<tt:H264ProfilesSupported>Main</tt:H264ProfilesSupported>"
			"<tt:H264ProfilesSupported>High</tt:H264ProfilesSupported>"
		"</tt:H264>",
		sAvParamGetRes.sVideParam[chn].nWidth, sAvParamGetRes.sVideParam[chn].nHeight,
		nLimitFps[0], nLimitFps[1]);
    if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));

	// Extension
	nRet = sprintf(temp,
		"<tt:Extension>"
			"<tt:H264>"
				"<tt:ResolutionsAvailable>"
					"<tt:Width>%d</tt:Width>"
					"<tt:Height>%d</tt:Height>"
				"</tt:ResolutionsAvailable>"
				"<tt:GovLengthRange>"
					"<tt:Min>1</tt:Min>"
					"<tt:Max>120</tt:Max>"
				"</tt:GovLengthRange>"
				"<tt:FrameRateRange>"
					"<tt:Min>%d</tt:Min>"
					"<tt:Max>%d</tt:Max>"
				"</tt:FrameRateRange>"
				"<tt:EncodingIntervalRange>"
					"<tt:Min>1</tt:Min>"
					"<tt:Max>1</tt:Max>"
				"</tt:EncodingIntervalRange>"
				"<tt:H264ProfilesSupported>Baseline</tt:H264ProfilesSupported>"
				"<tt:H264ProfilesSupported>Main</tt:H264ProfilesSupported>"
				"<tt:H264ProfilesSupported>High</tt:H264ProfilesSupported>"
				"<tt:BitrateRange>"
					"<tt:Min>16</tt:Min>"
					"<tt:Max>4096</tt:Max>"
				"</tt:BitrateRange>"
			"</tt:H264>"
		"</tt:Extension>",
		sAvParamGetRes.sVideParam[chn].nWidth, sAvParamGetRes.sVideParam[chn].nHeight, 
		nLimitFps[0], nLimitFps[1],nLimitKBps[0], nLimitKBps[1]);
    if (nRet < 0) return -1;
	strncat(body,temp,strlen(temp));

	strncat(body,"</trt:Options></trt:GetVideoEncoderConfigurationOptionsResponse>",strlen("</trt:Options></trt:GetVideoEncoderConfigurationOptionsResponse>"));
	//填充报文
	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetVideoEncoderConfigurationOptions(ONVIF_STRUCT *onvif,XMLN *data)
{
	int index=0;
	XMLN *token = NULL;
	int tokenLen;
	ERROR_MSG errMsg;

	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
	token = xml_node_soap_get(data, "ConfigurationToken");
	if (token == NULL || token->data == NULL)  {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}

	ERROR_LOG("name:%s,token:%s\n", token->name, token->data);
	tokenLen = strlen(token->data);
	index = token->data[tokenLen-1] - '0';
	return buildResponseXml(onvif, buildResponseGetVideoEncoderConfigurationOptions, &index);
}

/*
 *	GetPTZConfiguration
 */
int buildResponseGetPTZConfiguration(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024*8];
	int nRet;
	const PTZConfig *ptzCfg = NULL;


	ptzCfg = getPtzCfg();
	if (ptzCfg == NULL) return -1;

	nRet = sprintf(body,
		"<tptz:GetConfigurationResponse>"
		"<tptz:PTZConfiguration token=\"%s\">"
			"<tt:Name>%s</tt:Name>"
			"<tt:UseCount>%d</tt:UseCount>"
			"<tt:NodeToken>%s</tt:NodeToken>"
			"<tt:DefaultAbsolutePantTiltPositionSpace>%s</tt:DefaultAbsolutePantTiltPositionSpace>"
			"<tt:DefaultAbsoluteZoomPositionSpace>%s</tt:DefaultAbsoluteZoomPositionSpace>"
			"<tt:DefaultRelativePanTiltTranslationSpace>%s</tt:DefaultRelativePanTiltTranslationSpace>"
			"<tt:DefaultRelativeZoomTranslationSpace>%s</tt:DefaultRelativeZoomTranslationSpace>"
			"<tt:DefaultContinuousPanTiltVelocitySpace>%s</tt:DefaultContinuousPanTiltVelocitySpace>"
			"<tt:DefaultContinuousZoomVelocitySpace>%s</tt:DefaultContinuousZoomVelocitySpace>"
			"<tt:DefaultPTZSpeed>"
				"<tt:PanTilt space=\"%s\" y=\"%0.1f\" x=\"%0.1f\"></tt:PanTilt>"
				"<tt:Zoom space=\"%s\" x=\"%0.1f\"></tt:Zoom>"
			"</tt:DefaultPTZSpeed>"
			"<tt:DefaultPTZTimeout>PT0H1M0S</tt:DefaultPTZTimeout>"        //此处参数根据 ptzCfg.timeout(单位s)
			"<tt:PanTiltLimits>"
				"<tt:Range>"
				"<tt:URI>%s</tt:URI>"
				"<tt:XRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
				"</tt:XRange>"
				"<tt:YRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
				"</tt:YRange>"
				"</tt:Range>"
			"</tt:PanTiltLimits>"
			"<tt:ZoomLimits>"
				"<tt:Range>"
				"<tt:URI>%s</tt:URI>"
				"<tt:XRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
				"</tt:XRange>"
				"</tt:Range>"
			"</tt:ZoomLimits>"
		"</tptz:PTZConfiguration>"
		"</tptz:GetConfigurationResponse>",
		ptzCfg->token, ptzCfg->name, ptzCfg->usecnt, ptzCfg->nodetoken, ptzCfg->abs_pant_space, ptzCfg->abs_zoom_space,
		ptzCfg->rel_pant_space, ptzCfg->rel_zoom_space, ptzCfg->cont_pant_space, ptzCfg->cont_zoom_space,
		ptzCfg->speed_pant_space, ptzCfg->speed_pant_x, ptzCfg->speed_pant_y, ptzCfg->speed_zoom_space, ptzCfg->speed_zoom_x,
		ptzCfg->limit_pant_space, ptzCfg->limit_pant_x.min, ptzCfg->limit_pant_x.max, ptzCfg->limit_pant_y.min, ptzCfg->limit_pant_y.min,
		ptzCfg->limit_zoom_space, ptzCfg->limit_zoom_x.min, ptzCfg->limit_zoom_x.max);


	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetPTZConfiguration(ONVIF_STRUCT *onvif,XMLN *data)
{
	const PTZConfig *ptzCfg = NULL;
	XMLN *token = NULL;
	ERROR_MSG errMsg;
	
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);	
	token = xml_node_soap_get(data, "PTZConfigurationToken");
	if (token == NULL || token->data == NULL) return -1;
	
	ptzCfg = getPtzCfg();
	if (ptzCfg == NULL) {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
		return -1;
	}
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);	
	
	token = xml_node_soap_get(data, "PTZConfigurationToken");
	if (strcmp(ptzCfg->token,token->data)) {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	
	return buildResponseXml(onvif, buildResponseGetPTZConfiguration, NULL);
	
}
/*
 * GetPTZConfigurations
 */
int buildResponseGetPTZConfigurations(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024*8];
	int nRet;
	const PTZConfig *ptzCfg = (PTZConfig *)arg;

	nRet = sprintf(body,
		"<tptz:GetConfigurationResponse>"
		"<tptz:PTZConfiguration token=\"%s\">"
			"<tt:Name>%s</tt:Name>"
			"<tt:UseCount>%d</tt:UseCount>"
			"<tt:NodeToken>%s</tt:NodeToken>"
			"<tt:DefaultAbsolutePantTiltPositionSpace>%s</tt:DefaultAbsolutePantTiltPositionSpace>"
			"<tt:DefaultAbsoluteZoomPositionSpace>%s</tt:DefaultAbsoluteZoomPositionSpace>"
			"<tt:DefaultRelativePanTiltTranslationSpace>%s</tt:DefaultRelativePanTiltTranslationSpace>"
			"<tt:DefaultRelativeZoomTranslationSpace>%s</tt:DefaultRelativeZoomTranslationSpace>"
			"<tt:DefaultContinuousPanTiltVelocitySpace>%s</tt:DefaultContinuousPanTiltVelocitySpace>"
			"<tt:DefaultContinuousZoomVelocitySpace>%s</tt:DefaultContinuousZoomVelocitySpace>"
			"<tt:DefaultPTZSpeed>"
				"<tt:PanTilt space=\"%s\" y=\"%0.1f\" x=\"%0.1f\"></tt:PanTilt>"
				"<tt:Zoom space=\"%s\" x=\"%0.1f\"></tt:Zoom>"
			"</tt:DefaultPTZSpeed>"
			"<tt:DefaultPTZTimeout>PT0H1M0S</tt:DefaultPTZTimeout>"        //此处参数根据 ptzCfg.timeout(单位s)
			"<tt:PanTiltLimits>"
				"<tt:Range>"
				"<tt:URI>%s</tt:URI>"
				"<tt:XRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
				"</tt:XRange>"
				"<tt:YRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
				"</tt:YRange>"
				"</tt:Range>"
			"</tt:PanTiltLimits>"
			"<tt:ZoomLimits>"
				"<tt:Range>"
				"<tt:URI>%s</tt:URI>"
				"<tt:XRange>"
					"<tt:Min>%0.1f</tt:Min>"
					"<tt:Max>%0.1f</tt:Max>"
				"</tt:XRange>"
				"</tt:Range>"
			"</tt:ZoomLimits>"
		"</tptz:PTZConfiguration>"
		"</tptz:GetConfigurationResponse>",
		ptzCfg->token, ptzCfg->name, ptzCfg->usecnt, ptzCfg->nodetoken, ptzCfg->abs_pant_space, ptzCfg->abs_zoom_space,
		ptzCfg->rel_pant_space, ptzCfg->rel_zoom_space, ptzCfg->cont_pant_space, ptzCfg->cont_zoom_space,
		ptzCfg->speed_pant_space, ptzCfg->speed_pant_x, ptzCfg->speed_pant_y, ptzCfg->speed_zoom_space, ptzCfg->speed_zoom_x,
		ptzCfg->limit_pant_space, ptzCfg->limit_pant_x.min, ptzCfg->limit_pant_x.max, ptzCfg->limit_pant_y.min, ptzCfg->limit_pant_y.min,
		ptzCfg->limit_zoom_space, ptzCfg->limit_zoom_x.min, ptzCfg->limit_zoom_x.max);


	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetPTZConfigurations(ONVIF_STRUCT *onvif,XMLN *data)
{
	PTZConfig *ptzCfg = NULL;
	XMLN *token = NULL;
	ERROR_MSG errMsg;

	
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);	
	token = xml_node_soap_get(data, "PTZConfigurationToken");
	if (token == NULL || token->data == NULL) return -1;
	
	ptzCfg = getPtzCfg();
	if (ptzCfg == NULL) {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
		return -1;
	}
	
	token = xml_node_soap_get(data, "PTZConfigurationToken");
	if (strcmp(ptzCfg->token,token->data)) {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	
	return buildResponseXml(onvif, buildResponseGetPTZConfigurations, ptzCfg);
}
/*
 *	GetNode
 */
int buildResponseGetNode(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024*8];
	int nRet;
	const PTZConfig *ptzCfg = NULL;


	ptzCfg = getPtzCfg();
	if (ptzCfg == NULL) return -1;

	nRet = sprintf(body,
		"<tptz:GetNodeResponse>"
		"<tptz:PTZNode token=\"%s\"><tt:Name>%s</tt:Name>"
		"<tt:SupportedPTZSpaces>"
			"<tt:AbsolutePanTiltPositionSpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
				"<tt:YRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:YRange>"
			"</tt:AbsolutePanTiltPositionSpace>"
			"<tt:AbsoluteZoomPositionSpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
			"</tt:AbsoluteZoomPositionSpace>"
			"<tt:RelativePanTiltTranslationSpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
				"<tt:YRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:YRange>"
			"</tt:RelativePanTiltTranslationSpace>"
			"<tt:RelativeZoomTranslationSpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
			"</tt:RelativeZoomTranslationSpace>"
			"<tt:ContinuousPanTiltVelocitySpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
				"<tt:YRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:YRange>"
			"</tt:ContinuousPanTiltVelocitySpace>"
			"<tt:ContinuousZoomVelocitySpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
			"</tt:ContinuousZoomVelocitySpace>"
				"<tt:PanTiltSpeedSpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
			"</tt:PanTiltSpeedSpace>"
			"<tt:ZoomSpeedSpace><tt:URI>%s</tt:URI>"
				"<tt:XRange><tt:Min>%0.1f</tt:Min><tt:Max>%0.1f</tt:Max></tt:XRange>"
			"</tt:ZoomSpeedSpace>"
		"</tt:SupportedPTZSpaces>"
		"<tt:MaximumNumberOfPresets>0</tt:MaximumNumberOfPresets>"
		"<tt:HomeSupported>false</tt:HomeSupported>"
		"<tt:AuxiliaryCommands></tt:AuxiliaryCommands>"
		"</tptz:PTZNode></tptz:GetNodeResponse>",
		ptzCfg->nodetoken, ptzCfg->name, 
		ptzCfg->abs_pant_space, ptzCfg->abs_pant_x.min, ptzCfg->abs_pant_x.max, ptzCfg->abs_pant_y.min, ptzCfg->abs_pant_y.max,
		ptzCfg->abs_zoom_space, ptzCfg->abs_zoom_x.min, ptzCfg->abs_zoom_x.max,
		ptzCfg->rel_pant_space, ptzCfg->rel_pant_x.min, ptzCfg->rel_pant_x.max, ptzCfg->rel_pant_y.min, ptzCfg->rel_pant_y.max,
		ptzCfg->rel_zoom_space, ptzCfg->rel_zoom_x.min, ptzCfg->rel_zoom_x.max,
		ptzCfg->cont_pant_space, ptzCfg->cont_pant_x.min, ptzCfg->cont_pant_x.max, ptzCfg->cont_pant_y.min, ptzCfg->cont_pant_y.max,
		ptzCfg->cont_zoom_space, ptzCfg->cont_zoom_x.min, ptzCfg->cont_zoom_x.max,
		ptzCfg->speed_pant_space, ptzCfg->pant_speed_x.min, ptzCfg->pant_speed_x.max,
		ptzCfg->speed_zoom_space, ptzCfg->zoom_speed_x.min, ptzCfg->zoom_speed_x.max);
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetNode(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetNode, NULL);
}

/*
 * GetStatus
 */
int buildResponseGetStatus(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;
	const PTZConfig *ptzCfg = NULL;
	time_t seconds;
	struct tm* utcTime = NULL;

	ptzCfg = getPtzCfg();
	if (ptzCfg == NULL) return -1;

	seconds = time(NULL);
	utcTime = gmtime(&seconds);
	
	nRet = sprintf(body,
		"<tptz:GetStatusResponse>"
			"<tptz:PTZStatus>"
			"<tt:Position>"
				"<tt:PanTilt space=\"%s\" y=\"%0.1f\" x=\"%0.1f\"></tt:PanTilt>"
				"<tt:Zoom space=\"%s\" x=\"%0.1f\"></tt:Zoom>"
			"</tt:Position>"
			"<tt:MoveStatus>"
				"<tt:PanTilt>IDLE</tt:PanTilt>"
				"<tt:Zoom>IDLE</tt:Zoom>"
			"</tt:MoveStatus>"
			"<tt:UtcTime>%04d-%02d-%02dT%02d:%02d:%02dZ</tt:UtcTime>"
			"</tptz:PTZStatus>"
		"</tptz:GetStatusResponse>",
		ptzCfg->abs_pant_space, ptzCfg->pan.pos, ptzCfg->tilt.pos,
		ptzCfg->abs_zoom_space, ptzCfg->zoom.pos,
		utcTime->tm_year+1900, utcTime->tm_mon+1, utcTime->tm_mday, utcTime->tm_hour, utcTime->tm_min, utcTime->tm_sec);
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifGetStatus(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetStatus, NULL);
}

/*
 *	StopPTz
 */
int buildResponseStopPtz(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;
	
	nRet = sprintf(body,"<tptz:StopResponse></tptz:StopResponse>");
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}

int onvifStopPtz(ONVIF_STRUCT *onvif,XMLN *data)
{
	XMLN * panilt = NULL;
	int nRet;
	HINT32 nClientSpeed = SDK_PTZ_SPEED_LOW;
	//E_SDK_PTZ_DIRECT eDirect = SDK_PTZ_STOP;
	ERROR_MSG errMsg;


	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
	panilt = xml_node_soap_get(data, "PanTilt");
	if (panilt == NULL || panilt->data == NULL)  {	
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}

	if (strcmp(panilt->data, "true") == 0) {
		//ERROR_LOG("Speed:%d,direact:%d\n",nClientSpeed,(int)eDirect);
		// nRet = setPtz(nClientSpeed, 0, eDirect);
		// if (nRet < 0) {
		// 	errMsg.contentType = onvif->eDataType;
		// 	onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
		// 	return -1;
		// }
		(*ptzctrl)(0);
	}
	
	return buildResponseXml(onvif, buildResponseStopPtz, NULL);
}
/*
 * ContinuousMove
 */
int buildResponseContinuousMove(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;
	
	nRet = sprintf(body,"<tptz:ContinuousMoveResponse></tptz:ContinuousMoveResponse>");
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}


int onvifContinuousMove(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	int nRet;
	float x,y;
	HINT32 nClientSpeed = SDK_PTZ_SPEED_LOW;
	////E_SDK_PTZ_DIRECT eDirect = SDK_PTZ_STOP;
	ERROR_MSG errMsg;

	
	nRet = GetContinuousMoveData(data, &x, &y);
	if (nRet < 0) {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	
	if (!COMPARE_FLAOT(x, 0)) {
		if(x<0)
		{
			(*ptzctrl)(1);
		}
		else if(x>0)
		{
			(*ptzctrl)(2);
		}
			
	} else if (!COMPARE_FLAOT(y, 0)) {
		if(y<0)
		{
			(*ptzctrl)(4);
		}
		else if(y>0)
		{
			(*ptzctrl)(3);
		}
	} else {
		nClientSpeed = SDK_PTZ_SPEED_LOW;
		////eDirect = SDK_PTZ_STOP;
	}

	//ERROR_LOG("Speed:%d,direact:%d\n",nClientSpeed,(int)eDirect);
	// nRet = setPtz(nClientSpeed, 0, eDirect);
	// if (nRet < 0) {
	// 	errMsg.contentType = onvif->eDataType;
	// 	onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	// 	return -1;
	// }
	return buildResponseXml(onvif, buildResponseContinuousMove, NULL);
}

int onvif_ctrl_motor(ptzcb cb)
{
	ptzctrl = cb;
}

/*
 *	GetUsers
 */
 typedef struct {
 	char cUserName[32];
	char cPassword[32];
	int nUserLevle;
 } XML_USER_INFO;
 /*
  * 设备的用户角色和onvif用户角色需要按照下面关系重新匹�?可查看定�?
  *
  *      设备            |       onvif
  * ADMIN_ROLE_ID (1)  |    Administrator
  * USER_ROLE_ID  (2)  |    Operator
  * GUEST_ROLE_ID (3)  |    User
  */
const char * userTable[] = {"Administrator", "Operator", "User"};
 int buildResponseGetUsers(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
#if 0
	int nRet;
	S_FN_USERINFO *pUserInfos = NULL;
	int nUserNum;
	int i;
	int j = 0;
    char cPassword[32];
	int userIdCurrent = GUEST_ROLE_ID;
	XML_USER_INFO user[8];
	char body[1024*2];
	char temp[1024];

	memset(user, 0, sizeof(user));
	
	nUserNum = getUserinfo(HTRUE, &pUserInfos);
	if (nUserNum < 0 ) return -1;
	for(i = 0;i < nUserNum;i++)
	{
		if (pUserInfos[i].bEnable == HFALSE || strcmp(pUserInfos[i].chUserName,"") == 0) continue;
		if (strcmp(pUserInfos[i].chUserName, onvif->cUserName) == 0) userIdCurrent = pUserInfos[i].nUserID;
		strcpy(user[j].cUserName, pUserInfos[i].chUserName);
		strcpy(user[j].cPassword, pUserInfos[i].chPassWord);
		if (pUserInfos[i].nRoleId >=1 && pUserInfos[i].nRoleId <=3)
			user[j++].nUserLevle = pUserInfos[i].nRoleId -1;
		else
		   user[j++].nUserLevle = 3;
		ERROR_LOG("name:%s\tpasswd:%s\n",pUserInfos[i].chUserName, pUserInfos[i].chPassWord);
	}

	nRet = sprintf(body, "<tds:GetUsersResponse>");
	if (nRet < 0) return -1;

	for (i = 0; i < j; i++) {
		if (!((userIdCurrent == ADMIN_USER_ID) || (strcmp(user[i].cUserName, onvif->cUserName) == 0))) sprintf(user[i].cPassword, "******");
		nRet = sprintf(temp,
			"<tds:User>"
			"<tt:Username>%s</tt:Username>"
    		"<tt:Password>%s</tt:Password>"
    		"<tt:UserLevel>%s</tt:UserLevel>"
    		"</tds:User>",
    		user[i].cUserName, user[i].cPassword, userTable[user[i].nUserLevle]);
		strncat(body, temp, strlen(temp));
	}

	if (pUserInfos != NULL) free(pUserInfos);
	
	strncat(body, "</tds:GetUsersResponse>", strlen("</tds:User></tds:GetUsersResponse>"));

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
#else
	int nRet;
	////S_FN_USERINFO *pUserInfos = NULL;
	int nUserNum;
	int i;
	int j = 0;
    char cPassword[32];
	////int userIdCurrent = GUEST_ROLE_ID;
	XML_USER_INFO user[8];
	char body[1024*2];
	char temp[1024];

	memset(user, 0, sizeof(user));
	
	nUserNum = 1;//getUserinfo(HTRUE, &pUserInfos);
	if (nUserNum < 0 ) return -1;
	for(i = 0;i < nUserNum;i++)
	{
		////if (pUserInfos[i].bEnable == HFALSE || strcmp(pUserInfos[i].chUserName,"") == 0) continue;
		////if (strcmp(pUserInfos[i].chUserName, onvif->cUserName) == 0) userIdCurrent = pUserInfos[i].nUserID;
		strcpy(user[j].cUserName, "admin");
		strcpy(user[j].cPassword, g_onvifPassword);
		user[j++].nUserLevle = 1;//pUserInfos[i].nRoleId -1;
		ERROR_LOG("name:%s\tpasswd:%s\n",user[j].cUserName,user[j].cPassword);
	}

	nRet = sprintf(body, "<tds:GetUsersResponse>");
	if (nRet < 0) return -1;

	for (i = 0; i < j; i++) 
	{
		////if (!((userIdCurrent == ADMIN_USER_ID) || (strcmp(user[i].cUserName, onvif->cUserName) == 0))) sprintf(user[i].cPassword, "******");
		nRet = sprintf(temp,
			"<tds:User>"
			"<tt:Username>%s</tt:Username>"
    		"<tt:Password>%s</tt:Password>"
    		"<tt:UserLevel>%s</tt:UserLevel>"
    		"</tds:User>",
    		user[i].cUserName, user[i].cPassword, userTable[user[i].nUserLevle]);
		strncat(body, temp, strlen(temp));
	}

	////if (pUserInfos != NULL) free(pUserInfos);
	
	strncat(body, "</tds:GetUsersResponse>", strlen("</tds:User></tds:GetUsersResponse>"));

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
#endif
}

int onvifGetUsers(ONVIF_STRUCT *onvif,XMLN *data)
{	
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);	
 	return buildResponseXml(onvif, buildResponseGetUsers, NULL);
}
/*
 *	CreateUsers
 */
int buildResponseCreateUsers(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;

	nRet = sprintf(body,"<tds:CreateUsersResponse></tds:CreateUsersResponse>");	
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifCreateUsers(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);	
	ONVIF_USER_INFO userInfo[8];
	int nUserNum;
	int i;
	int nRet;
	ERROR_MSG errMsg;
	char tag[]= {"admin"};
	
	nUserNum = getUserInfoFromxml(data, tag, userInfo);
	if (nUserNum < 0)  {
		
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}

	for (i = 0;i < nUserNum; i++)
		ERROR_LOG("name:%s\tpasswd:%s\tlevel:%s\n",userInfo[i].userName,userInfo[i].password,userTable[userInfo[i].userRole -1]);

	nRet = 1;//addUserinfo(nUserNum, userInfo); 
	if (nRet < 0)
	{
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}

	return buildResponseXml(onvif, buildResponseCreateUsers, NULL);
}

/*
 * DeleteUsers
 */
int buildResponseDeleteUsers(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;

	nRet = sprintf(body,"<tds:DeleteUsersResponse></tds:DeleteUsersResponse>");	
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifDeleteUsers(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	ONVIF_USER_INFO userInfo[8];
	int nRet;
	int nUserNum;
	int i;
	ERROR_MSG errMsg;
	char tag[]={"DeleteUsers"};
	
	nUserNum = getUserInfoFromxml(data, tag, userInfo);
	if (nUserNum < 0)  {
		
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}

	for (i = 0;i < nUserNum; i++)
		ERROR_LOG("name:%s\n",userInfo[i].userName);

	nRet = 1;////delUserinfo(nUserNum, userInfo);
	if (nRet < 0) {
		
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
	
 	return buildResponseXml(onvif, buildResponseDeleteUsers, NULL);
}

/*
 * SetUser
 */
int buildResponseSetUser(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	int nRet;

	nRet = sprintf(body,"<tds:SetUserResponse></tds:SetUserResponse>");	
	if (nRet < 0) return -1;

	nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
	if (nRet < 0) return -1;
	return strlen(xmlBuff);
}
int onvifSetUser(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	ONVIF_USER_INFO userInfo[8];
	int nUserNum;
	int i;
	int nRet;
	ERROR_MSG errMsg;
	char tag[]={"admin"};
	
	nUserNum = getUserInfoFromxml(data, tag, userInfo);
	if (nUserNum < 0) {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}

	for (i = 0;i < nUserNum; i++)
		ERROR_LOG("name:%s\tpasswd:%s\tlevel:%s\n",userInfo[i].userName,userInfo[i].password,userTable[userInfo[i].userRole -1]);

	nRet = 1;////setUserinfo(nUserNum, userInfo);
	if (nRet < 0) {
		errMsg.contentType = onvif->eDataType;
		onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_MISSINGATTR);
		return -1;
	}
 	return buildResponseXml(onvif, buildResponseSetUser, NULL);
}

/*
 * CreatePullPointSubscription
 */
int buildResponseCreatePullPointSubscription(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024*2];
	char header[1024];
	char localIP[32];
	int nRet;
	ERROR_MSG errMsg;
	int port;
	time_t timeSecond;
	time_t endTime = *(time_t *)arg;
	struct tm *pUtc = NULL;
	struct tm utcTimeNow;
	struct tm utcTimeEnd;
	int min ,hour;

	do {
		nRet = getLocalIp(HFALSE, localIP);
		if (nRet < 0) break;

		port = getOnvifPort(HFALSE);//hySysCfgGet()->sOnvifParam.nOnvifPort;
		if (port < 0) break;

		nRet = sprintf(header,xmlHeaderWithAction,"http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionResponse");
		if (nRet < 0) break;

		timeSecond = time(0);
		pUtc = gmtime(&timeSecond);
		if (pUtc == NULL) break;
		memcpy(&utcTimeNow, pUtc, sizeof(struct tm));
	
		pUtc = gmtime(&endTime);
		if (pUtc == NULL) break;
		memcpy(&utcTimeEnd, pUtc, sizeof(struct tm));

		nRet = sprintf(body, 
			"<tev:CreatePullPointSubscriptionResponse>"
			"<tev:SubscriptionReference>"
			"<wsa:Address>http://%s:%d%s</wsa:Address>"
			"</tev:SubscriptionReference>"
			"<wsnt:CurrentTime>%04d-%02d-%02dT%02d:%02d:%02dZ</wsnt:CurrentTime>"
			"<wsnt:TerminationTime>%04d-%02d-%02dT%02d:%02d:%02dZ</wsnt:TerminationTime>"
			"</tev:CreatePullPointSubscriptionResponse>",
			localIP, port, URL_EVENT, 
			utcTimeNow.tm_year+1900, utcTimeNow.tm_mon+1, utcTimeNow.tm_mday, utcTimeNow.tm_hour, utcTimeNow.tm_min, utcTimeNow.tm_sec,
	        utcTimeEnd.tm_year+1900, utcTimeEnd.tm_mon+1, utcTimeEnd.tm_mday, utcTimeEnd.tm_hour, utcTimeEnd.tm_min, utcTimeEnd.tm_sec);
		if (nRet < 0) break;
		nRet = sprintf(xmlBuff, xmlEnvelope, header, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
		
	} while(1);


	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifCreatePullPointSubscription(ONVIF_STRUCT *onvif,XMLN *data)
{
	time_t durationTime;
	time_t endTime;
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	durationTime = getCreatePullPointTime(data);
	if (durationTime <= 0) durationTime = 600;
	endTime = time(NULL);
	endTime += durationTime;
	if (durationTime > 0) {
		pthread_mutex_lock(&g_pullPointMessageInfo.lock);
		g_pullPointMessageInfo.stat = HTRUE;
		g_pullPointMessageInfo.count++;
		if (endTime > g_pullPointMessageInfo.endTime)
			g_pullPointMessageInfo.endTime = endTime;
		pthread_mutex_unlock(&g_pullPointMessageInfo.lock);
	}

	return buildResponseXml(onvif, buildResponseCreatePullPointSubscription, (void *)&endTime);
}

/*
 * GetEventProperties
 */
int buildResponseGetEventProperties(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024*8];
	int nRet;
	ERROR_MSG errMsg;

	do {

		nRet = sprintf(body,
			"<tev:GetEventPropertiesResponse>"
			"<tev:TopicNamespaceLocation>http://www.onvif.org/onvif/ver10/topics/topicns.xml</tev:TopicNamespaceLocation>"
			"<wsnt:FixedTopicSet>true</wsnt:FixedTopicSet>"
			"<wstop:TopicSet>"
			"<wstop:documentation>"
			"<tns1:MediaControl wstop:topic=\"true\">"
			"<tns1:VideoSourceConfiguration wstop:topic=\"true\">"
			"<tt:MessageDescription IsProperty=\"true\">"
			"<tt:Source>"
			"<tt:SimpleItemDescription Name=\"VideoSourceConfigurationToken\" Type=\"tt:ReferenceToken\"/>"
			"</tt:Source>"
			"<tt:Data>"
			"<tt:SimpleItemDescription Name=\"Config\" Type=\"tt:VideoSourceConfiguration\"/>"
			"</tt:Data>"
			"</tt:MessageDescription>"
			"</tns1:VideoSourceConfiguration>"
			"</tns1:MediaControl>"
			"<tns1:ActionEngine>"
			"<Completed wstop:topic=\"true\">"
			"<tt:MessageDescription IsProperty=\"false\">"
			"<tt:Source>"
			"<tt:SimpleItemDescription Name=\"Token\" Type=\"tt:ReferenceToken\"/>"
			"<tt:SimpleItemDescription Name=\"ActionName\" Type=\"xsd:string\"/>"
			"</tt:Source>"
			"<tt:Data>"
			"<tt:ElementItemDescription Name=\"RequestResponse\" Type=\"tt:ActionEngineEventPayload\"/>"
			"</tt:Data>"
			"</tt:MessageDescription>"
			"</Completed>"
			"</tns1:ActionEngine>"
			"<tns1:ActionEngine>"
			"<Failed wstop:topic=\"true\">"
			"<tt:MessageDescription IsProperty=\"false\">"
			"<tt:Source>"
			"<tt:SimpleItemDescription Name=\"Token\" Type=\"tt:ReferenceToken\"/>"
			"<tt:SimpleItemDescription Name=\"ActionName\" Type=\"xsd:string\"/>"
			"</tt:Source>"
			"<tt:Data>"
			"<tt:ElementItemDescription Name=\"RequestResponse\" Type=\"tt:ActionEngineEventPayload\"/>"
			"</tt:Data>"
			"</tt:MessageDescription>"
			"</Failed>"
			"</tns1:ActionEngine>"
			"<tns1:VideoSource>"
			"</tns1:VideoSource>"
			"<tns1:Device>"
			"<Trigger>"
			"<DigitalInput wstop:topic=\"true\">"
			"<tt:MessageDescription IsProperty=\"true\">"
			"<tt:Source>"
			" <tt:SimpleItemDescription Name=\"IOAlarm\" Type=\"tt:ReferenceToken\"/>"
			"</tt:Source>"
			"<tt:Data>"
			" <tt:SimpleItemDescription Name=\"AlarmState\" Type=\"xsd:boolean\"/>"
			"</tt:Data>"
			"</tt:MessageDescription>"
			"</DigitalInput>"
			"</Trigger>"
			"</tns1:Device>"
			"</wstop:documentation>"
			"<tns1:RuleEngine wstop:topic=\"true\">"
			"<CellMotionDetector wstop:topic=\"true\">"
			"<Motion wstop:topic=\"true\">"
			"<tt:MessageDescription IsProperty=\"true\">"
			"<tt:Source>"
			"<tt:SimpleItemDescription Name=\"VideoSourceConfigurationToken\" Type=\"tt:ReferenceToken\"/>"
			"<tt:SimpleItemDescription Name=\"VideoAnalyticsConfigurationToken\" Type=\"tt:ReferenceToken\"/>"
			"<tt:SimpleItemDescription Name=\"Rule\" Type=\"xs:string\"/>"
			"</tt:Source>"
			"<tt:Data>"
			"<tt:SimpleItemDescription Name=\"IsMotion\" Type=\"xs:boolean\"/>"
			"</tt:Data>"
			"</tt:MessageDescription>"
			"</Motion>"
			"</CellMotionDetector>"
			"</tns1:RuleEngine>"
			"</wstop:TopicSet>"
			"<wsnt:TopicExpressionDialect>http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet</wsnt:TopicExpressionDialect>"
			"<wsnt:TopicExpressionDialect>http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete</wsnt:TopicExpressionDialect>"
			"<tev:MessageContentFilterDialect>http://www.onvif.org/ver10/tev/messageContentFilter/ItemFilter</tev:MessageContentFilterDialect>"
			"<tev:MessageContentSchemaLocation>http://www.onvif.org/onvif/ver10/schema/onvif.xsd</tev:MessageContentSchemaLocation>"
			"</tev:GetEventPropertiesResponse>");
		if (nRet < 0) break;
		nRet = sprintf(xmlBuff, xmlEnvelope, xmlHeader, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);		
	} while(0);


	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifGetEventProperties(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetEventProperties, NULL);
}

/*
 *	PullMessages
 */
int buildResponsePullMessages(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024*2];
	char header[1024];
	int nRet;
	ERROR_MSG errMsg;
	int port;
	time_t timeSecond;
	struct tm *pUtc = NULL;
	struct tm utcTimeNow;
	struct tm utcTimeEnd;
	HBOOL alarmFlag = *(HBOOL *)arg;

	do {
		nRet = sprintf(header,xmlHeaderWithAction,"http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesResponse");
		if (nRet < 0) break;

		timeSecond = time(0);
		pUtc = gmtime(&timeSecond);
		if (pUtc == NULL) break;
		memcpy(&utcTimeNow, pUtc, sizeof(struct tm));

		timeSecond += 60;		
		pUtc = gmtime(&timeSecond);
		if (pUtc == NULL) break;
		memcpy(&utcTimeEnd, pUtc, sizeof(struct tm));

		nRet = sprintf(body,
			"<tev:PullMessagesResponse>"
			"<tev:CurrentTime>%04d-%02d-%02dT%02d:%02d:%02dZ</tev:CurrentTime>"
			"<tev:TerminationTime>%04d-%02d-%02dT%02d:%02d:%02dZ</tev:TerminationTime>"
			"<wsnt:NotificationMessage>"
			"<wsnt:Topic Dialect=\"http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete\">tns1:RuleEngine/MotionRegionDetector/Motion</wsnt:Topic>"
			"<wsnt:Message>"
			"<tt:Message UtcTime=\"%04d-%02d-%02dT%02d:%02d:%02dZ\">"
			"<tt:Source>"
			"<tt:SimpleItem Value=\"000\" Name=\"VideoSourceConfigurationToken\"></tt:SimpleItem>"
			"<tt:SimpleItem Value=\"000\" Name=\"VideoAnalyticsConfigurationToken\"></tt:SimpleItem>"
			"<tt:SimpleItem Value=\"000\" Name=\"Rule\"></tt:SimpleItem>"
			"</tt:Source>"
			"<tt:Data>"
			"<tt:SimpleItem Value=\"%s\" Name=\"IsMotion\"></tt:SimpleItem>"
			"</tt:Data>"
			"</tt:Message>"
			"</wsnt:Message>"
			"</wsnt:NotificationMessage>"
			"</tev:PullMessagesResponse>",
			utcTimeNow.tm_year+1900, utcTimeNow.tm_mon+1, utcTimeNow.tm_mday, utcTimeNow.tm_hour, utcTimeNow.tm_min, utcTimeNow.tm_sec,
	        utcTimeEnd.tm_year+1900, utcTimeEnd.tm_mon+1, utcTimeEnd.tm_mday, utcTimeEnd.tm_hour, utcTimeEnd.tm_min, utcTimeEnd.tm_sec,
			utcTimeNow.tm_year+1900, utcTimeNow.tm_mon+1, utcTimeNow.tm_mday, utcTimeNow.tm_hour, utcTimeNow.tm_min, utcTimeNow.tm_sec,
	        alarmFlag?"true":"false");

		if (nRet < 0) break;
		nRet = sprintf(xmlBuff, xmlEnvelope, header, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
		
	}while(0);

	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifPullMessages(ONVIF_STRUCT *onvif,XMLN *data)
{
	time_t currentTime;
	HBOOL exitFlag = HFALSE;
	HBOOL alarmFlag = HFALSE;
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);

	currentTime = time(NULL);
	pthread_mutex_lock(&g_pullPointMessageInfo.lock);
	if (currentTime > g_pullPointMessageInfo.endTime || g_pullPointMessageInfo.count == 0 || g_pullPointMessageInfo.stat == HFALSE) {
		g_pullPointMessageInfo.count = 0;
		g_pullPointMessageInfo.endTime = 0;
		g_pullPointMessageInfo.stat = HFALSE;
		exitFlag = HTRUE;
	}
	ERROR_LOG("current time:%d, end time:%d, client count:%d, stat:%d, result:%d\n",currentTime, g_pullPointMessageInfo.endTime, 
	          g_pullPointMessageInfo.count, g_pullPointMessageInfo.stat, exitFlag);
	pthread_mutex_unlock(&g_pullPointMessageInfo.lock);

	
	if (exitFlag) return -1;

	alarmFlag = getMdAlarmStat(60);
	return buildResponseXml(onvif, buildResponsePullMessages, &alarmFlag);
}

/*
 *	UnsubscribeResponse
 */
int buildResponseUnsubscribe(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	char header[1024];
	int nRet;
	ERROR_MSG errMsg;
	do {
		
		nRet = sprintf(header,xmlHeaderWithAction,"http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/UnsubscribeResponse");
		if (nRet < 0) break;
		nRet = sprintf(body,"<wsnt:UnsubscribeResponse></wsnt:UnsubscribeResponse>");
		if (nRet < 0) break;

		if (nRet < 0) break;
		nRet = sprintf(xmlBuff, xmlEnvelope, header, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
	} while(0);

	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifUnsubscribe(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	pthread_mutex_lock(&g_pullPointMessageInfo.lock);
	if (g_pullPointMessageInfo.count > 0) g_pullPointMessageInfo.count--;
	if (g_pullPointMessageInfo.count == 0) {
		g_pullPointMessageInfo.stat = HFALSE;
		g_pullPointMessageInfo.endTime = 0;
	}
	pthread_mutex_unlock(&g_pullPointMessageInfo.lock);
	return buildResponseXml(onvif, buildResponseUnsubscribe, NULL);
}

/*
 *	Renew
 */
int buildResponseRenew(ONVIF_STRUCT *onvif, char *xmlBuff,int len, void *arg)
{
	char body[1024];
	char header[1024];
	int nRet;
	ERROR_MSG errMsg;
	time_t timeSecond;
	time_t endTime = *(time_t *)arg;
	struct tm *pUtc = NULL;
	struct tm utcTimeNow;
	struct tm utcTimeEnd;
	int min ,hour;
	do {
		
		nRet = sprintf(header,xmlHeaderWithAction,"http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/RenewResponse");
		if (nRet < 0) break;

		timeSecond = time(0);
		pUtc = gmtime(&timeSecond);
		if (pUtc == NULL) break;
		memcpy(&utcTimeNow, pUtc, sizeof(struct tm));
 	
		pUtc = gmtime(&endTime);
		if (pUtc == NULL) break;
		memcpy(&utcTimeEnd, pUtc, sizeof(struct tm));

		
		nRet = sprintf(body,
			"<wsnt:RenewResponse>"
  			"<wsnt:TerminationTime>%04d-%02d-%02dT%02d:%02d:%02dZ</wsnt:TerminationTime>"
   			"<wsnt:CurrentTime>%04d-%02d-%02dT%02d:%02d:%02dZ</wsnt:CurrentTime>"
   			"</wsnt:RenewResponse>",
   			utcTimeEnd.tm_year+1900, utcTimeEnd.tm_mon+1, utcTimeEnd.tm_mday, utcTimeEnd.tm_hour, utcTimeEnd.tm_min, utcTimeEnd.tm_sec,
	        utcTimeNow.tm_year+1900, utcTimeNow.tm_mon+1, utcTimeNow.tm_mday, utcTimeNow.tm_hour, utcTimeNow.tm_min, utcTimeNow.tm_sec);
		if (nRet < 0) break;

		if (nRet < 0) break;
		nRet = sprintf(xmlBuff, xmlEnvelope, header, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
	} while(0);

	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifRenew(ONVIF_STRUCT *onvif,XMLN *data)
{
	time_t durationTime;
	time_t endTime;
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	
	durationTime = getRenewTime(data);
	if (durationTime <= 0) durationTime = 600;

	endTime = time(NULL);
	endTime += durationTime;

	pthread_mutex_lock(&g_pullPointMessageInfo.lock);
	if (endTime > g_pullPointMessageInfo.endTime) g_pullPointMessageInfo.endTime = endTime;
	pthread_mutex_unlock(&g_pullPointMessageInfo.lock);

	return buildResponseXml(onvif, buildResponseRenew, (void *)&endTime);
}

/*
 * GetOptions
 */
int buildResponseGetOptions(ONVIF_STRUCT * onvif, char * xmlBuff, int len, void * arg)
{
	char body[1024];
	char header[1024];
	int nRet;
	ERROR_MSG errMsg;

	do {

		nRet = sprintf(body,
			"<timg:GetOptionsResponse>"
			"<timg:ImagingOptions>"
				"<tt:BacklightCompensation>"
				"<tt:Mode>OFF</tt:Mode>"
				"<tt:Mode>ON</tt:Mode>"
				"</tt:BacklightCompensation>"
				"<tt:Brightness>"
					"<tt:Min>0</tt:Min>"
					"<tt:Max>255</tt:Max>"
				"</tt:Brightness>"
				"<tt:ColorSaturation>"
					"<tt:Min>0</tt:Min>"
					"<tt:Max>255</tt:Max>"
				"</tt:ColorSaturation>"
				"<tt:Contrast>"
					"<tt:Min>0</tt:Min>"
					"<tt:Max>255</tt:Max>"
				"</tt:Contrast>"
				"<tt:IrCutFilterModes>ON</tt:IrCutFilterModes>"
				"<tt:IrCutFilterModes>OFF</tt:IrCutFilterModes>"
				"<tt:IrCutFilterModes>AUTO</tt:IrCutFilterModes>"
				"<tt:Sharpness>"
					"<tt:Min>0</tt:Min>"
					"<tt:Max>255</tt:Max>"
				"</tt:Sharpness>"
				"<tt:WhiteBalance>"
					"<tt:Mode>AUTO</tt:Mode>"
					"<tt:Mode>MANUAL</tt:Mode>"
				"</tt:WhiteBalance>"
			"</timg:ImagingOptions>"
			"</timg:GetOptionsResponse>");
		if (nRet < 0) break;
			
		if (nRet < 0) break;
		nRet = sprintf(xmlBuff, xmlEnvelope, header, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
	} while(0);

	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifGetOptions(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetOptions, NULL);
}

/*
 *	GetVideoSourceConfigurations
 */

int buildResponseGetVideoSourceConfigurations(ONVIF_STRUCT * onvif, char * xmlBuff, int len, void * arg)
{
	char body[1024];
	char header[1024];
	int nRet;
	ERROR_MSG errMsg;

	do {

		nRet = sprintf(body,"<trt:GetVideoSourceConfigurationsResponse></trt:GetVideoSourceConfigurationsResponse>");
		if (nRet < 0) break;
		nRet = sprintf(xmlBuff, xmlEnvelope, header, body);
		if (nRet < 0) break;
		return strlen(xmlBuff);
	} while(0);

	errMsg.contentType = onvif->eDataType;
	nRet = onviBuildErrReponse(onvif->nSockFd, errMsg, ONVIF_ERR_DEFAULT);
	return -1;
}
int onvifGetVideoSourceConfigurations(ONVIF_STRUCT *onvif,XMLN *data)
{
	ERROR_LOG("%s,%d\n",__FUNCTION__, __LINE__);
	return buildResponseXml(onvif, buildResponseGetVideoSourceConfigurations, NULL);
}

static HINT32 GetSnapInfo(const HUCHAR nChnIndex,HCHAR **chDataBuffer)
{
#if 0
	HINT32 nRes;

	*chDataBuffer = (HCHAR *) malloc(SNAP_BUFFER_LEN);
	if (*chDataBuffer == NULL) return -1;
	nRes =-1;// coreSnapShotImage(nChnIndex,*chDataBuffer,SNAP_BUFFER_LEN);

	#if 1
	ERROR_LOG("====================get_motion_snapshot=====================\n");
	int ret = CaptureSnapshotGetBuffer(*chDataBuffer, SNAP_BUFFER_LEN, 5);
	if( ret <= 0 )
	{
		ERROR_LOG("snapshot err!!!\n");
		free(*chDataBuffer);
		*chDataBuffer = NULL;
		return nRes;
	}
	nRes = ret;
	ERROR_LOG("====================get_motion_snapshot ok=====================\n");

	CaptureSnapshotReleaseBuffer();
	#endif
	return nRes;
#else
	return 0;
#endif
}

void sendSnapJpeg(ONVIF_STRUCT *onvif, HUCHAR nChnIndex)
{
#if 0
	char *buff = 0;
	HINT32 nSize;
	HINT32 nLen;
	HINT32 nTotalSize = 0;
	char sendData[512*1024];
	const char head[] = { 
	"HTTP/1.1 200 OK\r\n"
		"Content-Type: image/jpeg\r\n"
		"Content-Length: %d\r\n"
		"Connection: close\r\n"
		"Server: lighttpd/1.4.35\r\n\r\n"
	} ;
	//ERROR_LOG("%s,%d,index:%d\n",__FUNCTION__, __LINE__, nChnIndex);

	memset(sendData, 0, sizeof(sendData));

	nSize = GetSnapInfo(nChnIndex, &buff);
	if (nSize < 0 || buff == 0) {
	ERROR_LOG("get snap error!\n");
	return;
	}

	sprintf(sendData,head,strlen(head));
	nLen = strlen(sendData);
	memcpy(sendData+nLen, buff, nSize);
	nTotalSize = nSize + nLen;
	nSize = 0;
	while (nSize < nTotalSize) {
	nLen = send(onvif->nSockFd, sendData+nSize, nTotalSize-nSize, 0);
	if (nLen < 0) continue;
	nSize += nLen;  
	}
	free(buff);
#else
	return;
#endif
}

