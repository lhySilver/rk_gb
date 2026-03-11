
#ifndef _RTP_HTTP_H_
#define _RTP_HTTP_H_

#include "HTTP/HttpProtocol.h"
#include "HTTP/HttpString.h"

class HTTPCommon
{
public:
	HTTPCommon(void);

	virtual ~HTTPCommon(void);

	int fromStream(char * data);
	char* toStream(void);

	char* getBody(void);
	int setBody(const char* body, int len);

	char* getString(void);
	int getLength();

	int getType(void);	//Request or Response
	int getProtocol(void);	//SIP or HTTP
	void setProtocol(PROTOCOL_TYPE type);

	int addRef();
	int release();

	void reset(void);
public:
	int method;
	char from[64];
	char to[64];
	char cseq[64];
	char authorization[128];
	char callID[64];
	int maxForwards;
	char contact[64];
	int contentType;
	int contentLength;
	char url[256];
	char via[128];
	char contentTypeString[64];

	char accept[128];
	char userAgent[64];
	char host[64];
	char xClientAddress[64];
	char xTransactionID[64];
	char setCookie[64];
	char date[64];
	char server[64];
	char cookie[64];
	char acceptEncoding[64];
	char acceptLanguage[64];
	char allow[64];
	int bandwidth;
	int blockSize;
	char scale[64];
	char speed[64];
	char conference[64];
	char connection[64];
	char contentBase[128];
	char contentEncoding[64];
	char contentLanguage[64];
	char range[64];
	char rtpInfo[64];
	char session[64];
	char timestamp[64];
	char transport[128];
	char wwwAuthenticate[64];
	char unsupport[64];
	char vary[64];
	char expires[64];
	char lastModified[64];
	char _x_Cache_Control[32];
	char _x_Accept_Retransmit[32];
	char _x_Accept_Dynamic_Rate[4];
	char _x_Dynamic_Rate[16];
	char _x_transport_options[32];

protected:
	char _body[LEN_HTTP_BODY];
	char _buffer[LEN_HTTP_BUFFER];
	int _reqOrRep;
	int _protocol;
protected:
	virtual int parseHead(const char* data) = 0;
	virtual int packetHead(void) = 0;

	int parseCommon(const char* data);
	int packetCommon(void);
	
	virtual int parseLine(char* data);
	virtual int packetLine(void);

	int _ref;
};

/////////////////////////////////////////////////////////////
class HTTPRequest : public HTTPCommon
{
public:
	HTTPRequest(void);
	virtual ~HTTPRequest(void);

private:
	int parseHead(const char* data);
	int packetHead(void);
};

/////////////////////////////////////////////////////////////
class HTTPResponse : public HTTPCommon
{
public:
	HTTPResponse(void);
	virtual ~HTTPResponse(void);
public:
	int result;
	char message[64];
private:
	int parseHead(const char* data);
	int packetHead(void);
};

////////////////////////////////////////////////////////////////////
class HTTPFactory
{
public:
	static HTTPCommon* createPDUFromStream(char* data);
private:
	static HTTPCommon* obj;
};


#define PARSE_HEAD_FIELD(source, var, title)			\
		else if (String::indexOf(source, title) == 0)	\
		{												\
			char* tt = String::readValue(pStr, ':');	\
			String::trim(tt);							\
			strncpy(var, tt, sizeof(var));				\
		}					
									
#define PARSE_HEAD_FIELD_INT(source, var, title)		\
		else if (String::indexOf(source, title) == 0)	\
		{												\
			char* tt = String::readValue(pStr, ':');	\
			String::trim(tt);							\
			var = String::str2int(tt);\
		}

#define PACKET_HEAD_FIELD_STR(var, title)		\
	if (strlen(var) > 0)						\
	{											\
		memset(str, 0, sizeof(str));			\
		sprintf(str, "%s: %s\r\n", title, var);	\
		strcat(_buffer, str);					\
	}

#define PACKET_HEAD_FIELD_INT(var, title)		\
	if (var > 0)						\
	{											\
		memset(str, 0, sizeof(str));			\
		sprintf(str, "%s: %d\r\n", title, var);	\
		strcat(_buffer, str);					\
	}

#endif //head
