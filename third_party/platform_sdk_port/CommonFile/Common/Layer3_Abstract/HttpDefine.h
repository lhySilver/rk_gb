#ifndef HTTPCMDDEFINE_HEAD
#define HTTPCMDDEFINE_HEAD

#include <string>
#include <map>
#include "base_type_define.h"

using namespace std;

namespace HTTP_DEFINE
{
	static const char CONTENT_LENGTH[] = "Content-Length:";
	static const char CONTENT_LENGTH_EX[] = "content-length:"; //兼容谷歌插件Postman
	static const char DOUBLE_ENTER_LINE[] = "\r\n\r\n";
	static const char AUTHORIZATION_FILED[] = "Authorization: Basic ";
    static const char SESSION[] = "session: ";
	static const int HTTP_HEADER_LEN = 1024;
	static const char CHECK_AUTHORIZATION[] = "Authorization";

	// HTTP 状态码************************************************************************
	typedef enum
	{
		HTTP_OK = 200,
		HTTP_MOVED_TEMPORARILY = 302,
		HTTP_BAD_REQUEST = 400,			// malformed syntax 
		HTTP_UNAUTHORIZED = 401,			// authentication needed, respond with auth hdr
		HTTP_NOT_FOUND = 404,
		HTTP_FORBIDDEN = 403,
		HTTP_REQUEST_TIMEOUT = 408,
		HTTP_NOT_IMPLEMENTED = 501,			// used for unrecognized requests 
		HTTP_INTERNAL_SERVER_ERROR = 500,
	} HttpStatusCode;
	//////////////////////////////////////////////////////////////////////////

	// HTTP协议内容********************************************************************
	typedef struct _HTTP_PROTOCOL_
	{
		_HTTP_PROTOCOL_(): m_nPayLoadLen(0), m_nDataLen(0), m_pBuff(0){}
		~_HTTP_PROTOCOL_()
		{
			if (m_pBuff != 0)
			{
				delete []m_pBuff;
				m_pBuff = 0;
			}
		}
		_HTTP_PROTOCOL_ &operator=(const _HTTP_PROTOCOL_ &other)
		{
			if (this != &other)
			{
				this->m_strURL = other.m_strURL;
				this->m_nPayLoadLen = other.m_nPayLoadLen;
				this->m_nDataLen = other.m_nDataLen;

				if (this->m_pBuff != 0)
				{
					delete []this->m_pBuff;
					this->m_pBuff = 0;
				}

				if (other.m_pBuff != 0 && other.m_nDataLen > 0)
				{
					this->m_pBuff = new char[this->m_nDataLen+1];
					memset(this->m_pBuff, 0, this->m_nDataLen+1);
					memcpy(this->m_pBuff, other.m_pBuff, this->m_nDataLen);
				}
			}

			return *this;
		}

		tuint32 m_nDataLen;								// 数据包总长度
		tuint32 m_nPayLoadLen;								// 负载数据长度
		std::string m_strURL;								// URL
		char *m_pBuff;									// HTTP数据包
	}*P_HTTP_PROTOCOL, HTTP_PROTOCOL;
	//////////////////////////////////////////////////////////////////////////

	// HTTP状态响应信息*************************************************************************************
	typedef struct _HTTP_STATUS_INFO_
	{
		_HTTP_STATUS_INFO_(string strKey, string strInfo): strStatusKey(strKey), strStatusInfo(strInfo){}
		const string strStatusKey;
		const string strStatusInfo;
	} HTTP_STATUS_INFO;

	static const std::map<int, HTTP_STATUS_INFO>::value_type HTTPStatusValue[] =
	{
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_OK, HTTP_STATUS_INFO(string("OK"), string(""))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_MOVED_TEMPORARILY, HTTP_STATUS_INFO(string("Found"), string("Directories must end with a slash."))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_REQUEST_TIMEOUT, HTTP_STATUS_INFO(string("Request Timeout"), string("No request appeared within a reasonable time period."))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_NOT_IMPLEMENTED, HTTP_STATUS_INFO(string("Not Implemented"), string("The requested method is not recognized by this server."))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_UNAUTHORIZED, HTTP_STATUS_INFO(string("Unauthorized"), string("The requested method is not recognized by this server."))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_NOT_FOUND, HTTP_STATUS_INFO(string("Not Found"), string("The requested URL was not found on this server."))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_BAD_REQUEST, HTTP_STATUS_INFO(string("Bad Request"), string("Unsupported method."))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_FORBIDDEN, HTTP_STATUS_INFO(string("Forbidden"), string(""))),
		std::map<int, HTTP_STATUS_INFO>::value_type( HTTP_INTERNAL_SERVER_ERROR, HTTP_STATUS_INFO(string("Internal Server Error"), string("Internal Server Error"))),
	};

	static const std::map<int, HTTP_STATUS_INFO> g_HTTPStatusMap(HTTPStatusValue, HTTPStatusValue+9);
	//////////////////////////////////////////////////////////////////////////

	// HTTP 请求/响应文档类型**************************************************************************
	static const std::map<string, string>::value_type ContentType[] =
	{
		std::map<string, string>::value_type(".htm.html", "text/html"),
		std::map<string, string>::value_type(".xml", "text/xml"),
		std::map<string, string>::value_type(".jpg.jpeg", "image/jpeg"),
		std::map<string, string>::value_type(".gif", "image/gif"),
		std::map<string, string>::value_type(".png", "image/png"),
		std::map<string, string>::value_type(".txt.h.c.cc.cpp", "text/plain"),
		std::map<string, string>::value_type(".css", "text/css"),
		std::map<string, string>::value_type(".wav", "audio/wav"),
		std::map<string, string>::value_type(".avi", "video/x-msvideo"),
		std::map<string, string>::value_type(".qt.mov", "video/quicktime"),
		std::map<string, string>::value_type(".mpe.mpeg", "video/mpeg"),
		std::map<string, string>::value_type(".mid.midi", "audio/midi"),
		std::map<string, string>::value_type(".mp3", "audio/mpeg"),
		std::map<string, string>::value_type(".cod", "application/vnd.rim.cod"),
		std::map<string, string>::value_type(".jad", "text/vnd.sun.j2me.app-descriptor"),
		std::map<string, string>::value_type("", "application/octet-stream"),
	};

	static const std::map<string, string> g_ContentTypeMap(ContentType, ContentType+15);
    typedef enum 
    {
        HTTP_REQUEST_POST = 1,                     //请求类型--创建
        HTTP_REQUEST_PUT,                          //请求类型--更新
        HTTP_REQUEST_GET,                          //请求类型--获取
        HTTP_REQUEST_DELETE,                       //请求类型--删除                          
    }HTTP_MSG_TYPE_H;

}
#endif //HTTPCMDDEFINE_HEAD
