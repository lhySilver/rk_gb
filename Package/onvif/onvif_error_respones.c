#include "onvif_error_respones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int buildErrResopnseXml(char* xmlBuff, int len, const char *code, const char *subcode, const char *subcode_ex, const char * reason)
{
	int nRet;
	char body[1024*8];
	char temp[1024*4];
	nRet = sprintf(body,
		"<s:Fault><s:Code><s:Value>%s</s:Value>"
		"<s:Subcode><s:Value>%s</s:Value>",
		code, subcode);
	if (nRet < 0) return -1;

	if (subcode_ex != NULL) {
		nRet = sprintf(temp,"<s:Subcode><s:Value>%s</s:Value></s:Subcode>", subcode_ex);
		if (nRet < 0) return -1;
		strncat(body, temp, strlen(temp));
	}

	nRet = sprintf(temp,
		"</s:Subcode></s:Code>"
		"<s:Reason><s:Text xml:lang=\"en\">%s</s:Text></s:Reason></s:Fault>",reason);
	if (nRet < 0) return -1;
	strncat(body, temp, strlen(temp));

	nRet = sprintf(xmlBuff,xmlEnvelope,xmlHeader,body);
	if (nRet < 0) return -1;

	return strlen(xmlBuff);
}

int onvifSendErrResponse(int sockFd, ERROR_MSG errMsg, int httpErrcode, const char * httpErrStr, const char *xml, int len)
{	
	int headLen;
	char * buff;
	int i = (int)errMsg.contentType;
	const char *ContentType[]={"application/sdp","application/soap+xml","text/plain","text/html","application/octet-stream"," "};

	buff = (char *)malloc(len+1024);
	if (buff == NULL) return -1;

	headLen = sprintf(buff, "HTTP/1.1 %d %s\r\n"
							"Server: gSoap/2.8\r\n"
							"Content-Type: %s\r\n"
							"Content-Length: %d\r\n"
							"Connection: close\r\n\r\n",
							httpErrcode, httpErrStr , ContentType[i], len);
	if (headLen < 0) return -1;

	memcpy(buff+headLen, xml, len);
	headLen += len;

	buff[headLen] = '\0';
	//printf("\n\n======================err!============================\n\n");
	//printf("%s\n",buff);
	send(sockFd, buff, headLen, 0);
	if (buff) free(buff);
	return headLen;	
}

int onvifBuildErrReponse(int sockFd, ERROR_MSG errMsg, const char * code, const char * subcode, 
                         const char * subcode_ex, const char * reason, int httpErrCode, const char * httpErrStr)
{
	int ret = -1, mlen = 1024*16, xlen;
    char *pXml;
    
	//printf("soap_err_rly\r\n");

	while (1) {
		pXml = (char *)malloc(mlen);
		if (pXml == NULL) break;

		xlen = buildErrResopnseXml(pXml, mlen, code, subcode, subcode_ex, reason);
		if (xlen < 0 || xlen >= mlen) break;

		ret = onvifSendErrResponse(sockFd, errMsg, httpErrCode, httpErrStr, pXml, xlen);
		break;

	}
	if (pXml) {
		free(pXml);
	}
	return ret;
}

int onvifErrordefault(int sockFd, ERROR_MSG errMsg)
{
	return onvifBuildErrReponse(sockFd, errMsg, ERR_RECEIVER, ERR_ACTIONNOTSUPPORTED, NULL, "Action Not Implemented", 400, "Bad Request");
}


int onvifErrorNotAuth(int sockFd, ERROR_MSG errMsg)
{
	return onvifBuildErrReponse(sockFd, errMsg, ERR_SENDER, ERR_NOTAUTHORIZED, NULL, "Sender not Authorized", 401, "Not Authorized");
}

int onviBuildErrReponse(int sockFd, ERROR_MSG errMsg, ONVIF_ERROR_CODE errCode)
{
	int ret = 0;
	switch (errCode) {
		case ONVIF_ERR_SERVICE_NOT_SUPPORT:
			ret = onvifBuildErrReponse(sockFd, errMsg, ERR_SENDER, ERR_ACTION, "ter:ServiceNotSupported","Service Not Supported", 400, "Bad Request");
			break;
		case ONVIF_ERR_NO_TOKEN :
			ret = onvifBuildErrReponse(sockFd, errMsg, ERR_SENDER, ERR_ACTION, "ter:NoToken","The requested token does not exist.", 400, "Bad Request");
			break;
		case ONVIF_ERR_NOT_AUTH :
			ret = onvifErrorNotAuth(sockFd,errMsg);
			break;
		case ONVIF_ERR_MISSINGATTR :
			onvifBuildErrReponse(sockFd, errMsg, ERR_SENDER, ERR_ACTION, NULL,"Missing Attribute", 400, "Bad Request");
			break;
		default :
			ret = onvifErrordefault(sockFd,errMsg);
			break;
	}

	return ret;
}

