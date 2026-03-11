# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include "SipUtil.h"
#include "eXosip2/eXosip.h"
#include <map>
#include <algorithm>


void CSipUtil::memcpy_safe(char** dest, const std::string&src)
{
    *dest = (char*)malloc(sizeof(char)*(src.size()+1));
    memset(*dest, 0,src.size()+1  );
    memcpy(*dest,src.c_str(),src.size());
}

std::map<std::string ,SipMethod>   g_method_string_map;
std::map<SipMethod,std::string>    g_method_enum_map;

std::map<SipContentType,std::string> g_content_enum_map;
std::map<std::string,SipContentType> g_content_string_map;

void  CSipUtil::Init()
{
       g_method_string_map.insert(  std::make_pair("REGISRE",kSipRegisterMethod) );
       g_method_string_map.insert(  std::make_pair("INVITE",kSipInviteMethod)  );
       g_method_string_map.insert(  std::make_pair("SUBSCRIBE", kSipSubscribeMethod)  );
       g_method_string_map.insert(  std::make_pair("NOTIFY",kSipNotifyMethod)  );
       g_method_string_map.insert(  std::make_pair("INFO",kSipInfoMethod)  );
       g_method_string_map.insert(  std::make_pair("MESSAGE",kSipMessageMethod)  );
       g_method_string_map.insert(  std::make_pair("BYE", kSipByeMethod)  );
       g_method_string_map.insert(  std::make_pair("CANCLE",kSipCancleMethod)  );
       g_method_string_map.insert(  std::make_pair("ACK",kSipAckMethod)  );

        g_method_enum_map.insert(  std::make_pair(kSipRegisterMethod,"REGISRE")  );
        g_method_enum_map.insert(  std::make_pair(kSipInviteMethod,"INVITE")  );
        g_method_enum_map.insert(  std::make_pair( kSipSubscribeMethod,"SUBSCRIBE")  );
        g_method_enum_map.insert(  std::make_pair(kSipNotifyMethod,"NOTIFY")  );
        g_method_enum_map.insert(  std::make_pair(kSipInfoMethod,"INFO")  );
        g_method_enum_map.insert(  std::make_pair(kSipMessageMethod,"MESSAGE")  );
		g_method_enum_map.insert(std::make_pair(kSipMessageWithCallMethod, "MESSAGE"));
        g_method_enum_map.insert(  std::make_pair(kSipByeMethod,"BYE")  );
        g_method_enum_map.insert(  std::make_pair(kSipCancleMethod,"CANCLE")  );
        g_method_enum_map.insert(  std::make_pair(kSipAckMethod,"ACK")  );

        g_content_enum_map.insert(  std::make_pair(kSipContentMANSCDP_XML,"Application/MANSCDP+xml")  );
        g_content_enum_map.insert(  std::make_pair(kSipContentMANSRTSP,"Application/MANSRTSP")  );
        g_content_enum_map.insert(  std::make_pair(kSipContentRTSP,"Application/RTSP")  );
        g_content_enum_map.insert(  std::make_pair(kSipContentSDP,"Application/sdp")  );
        g_content_enum_map.insert(  std::make_pair(kSipContentUnknown,"Application/unkonw")  );

        g_content_string_map.insert(  std::make_pair("application/manscdp+xml",kSipContentMANSCDP_XML)  );
        g_content_string_map.insert(  std::make_pair("application/mansrtsp",kSipContentMANSRTSP)  );
        g_content_string_map.insert(  std::make_pair("application/rtsp",kSipContentRTSP)  );
        g_content_string_map.insert(  std::make_pair("application/sdp",kSipContentSDP)  );
        g_content_string_map.insert(  std::make_pair("application/unkonw",kSipContentUnknown)  );


}




CSipUtil::CSipUtil()
{

}

 SipMethod  CSipUtil::MethodStringToEnum(const std::string& method)
{

      std::map<std::string ,SipMethod>::iterator iter  =   g_method_string_map.find(method);
      if(iter != g_method_string_map.end()){
            return iter->second;
      }
            return kSipInvaildMethod;

}

std::string CSipUtil::MethodEnumToString(SipMethod method)
{

      std::map<SipMethod,std::string>::iterator iter  =   g_method_enum_map.find(method);
      if(iter != g_method_enum_map.end()){
            return iter->second;
      }
            return "";

}

std::string CSipUtil::ContentEnumToString(SipContentType content)
{

      std::map<SipContentType,std::string>::iterator iter  =   g_content_enum_map.find(content);
      if(iter != g_content_enum_map.end()){
            return iter->second;
      }
            return "";

}

SipContentType CSipUtil::ContentStringToEnum( std::string& content  )
{
      if(content.empty()){
           return kSipContentEmpty;
      }
	  transform(content.begin(), content.end(), content.begin(), ::tolower);
      std::map<std::string,SipContentType>::iterator iter  =   g_content_string_map.find(content);
      if(iter != g_content_string_map.end()){
            return iter->second;
      }
            return kSipContentUnknown;

}

 bool CSipUtil::Unauthorized( eXosip_event_t* event,  eXosip_t* context, int expire, const char* wwwauth)
 {

     if ( !event ||  !context  ||  !wwwauth  ||  strlen(wwwauth) <= 0)  {
            return false;
     }

     osip_message_t* pMsg = NULL;
     char temp[100]={0};
     sprintf(temp,"%d",expire);
     eXosip_lock(context);
     if (0 != eXosip_message_build_answer(context, event->tid, kUnauthorized,  &pMsg)) {
         goto Error;
     }

     if (0 != osip_message_set_www_authenticate(pMsg, wwwauth)) {
         goto Error;
     }

      if (0 != osip_message_set_expires(pMsg,   temp )) {
           goto Error;
      }

      if (0 != eXosip_message_send_answer(context,event->tid, kUnauthorized, pMsg)) {
          goto Error;
      }

         eXosip_unlock(context);
         return true;
Error:
         eXosip_unlock(context);
         return false;
 }

