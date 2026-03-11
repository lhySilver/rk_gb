#ifndef CSIPUTIL_H
#define CSIPUTIL_H
#include "SipDefs.h"


typedef struct eXosip_t eXosip_t;
typedef struct eXosip_event eXosip_event_t;
typedef struct osip_message osip_message_t;

class CSipUtil
{
public:
    CSipUtil();
   static void  Init();
   static SipMethod  MethodStringToEnum(const std::string& method);
   static std::string  MethodEnumToString(SipMethod method);
   static std::string  ContentEnumToString(SipContentType content);
   static SipContentType ContentStringToEnum(std::string& content  );
   static bool Unauthorized(eXosip_event_t*event, eXosip_t* context, int expire, const char* wwwauth);
   static bool GetPeerAddress( const osip_message_t *msg, bool bRequest);
   static void memcpy_safe(char** dest, const std::string&src);
};

#endif // CSIPUTIL_H
