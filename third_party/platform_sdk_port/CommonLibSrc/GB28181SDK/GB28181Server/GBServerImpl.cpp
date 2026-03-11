#include "SdpUtil.h"
#include "gbutil.h"
#include "GBServerImpl.h"
#include "SipStackSDK.h"
#include "GB28181XmlParser.h"
#include "StreamRequestManager.h"
#include "GBClientInfoManager.h"
#include "Rtsp/RtspParser.h"
#include "catalog_response.h"
#include "waiter/WaiterManager.h"
#include "ShareSDK.h"


struct CatalogTimerStruct
{
    CGBServerImpl*     server;
    CResponseWaiter* waiter;
};


CGBServerImpl::CGBServerImpl()
{

     m_sip_server = new SipUserAgentServer;
     m_xml_parser = new CGB28181XmlParser;
     m_stream_manager = new CStreamRequestManager;
     m_waiter_manager = new CWaiterManager;
     m_client_manager = new CGBClientInfoManager;
     m_gb_receiver = NULL;
	 m_subscribe_manager = new CSubscribeManager;

}


CGBServerImpl::~CGBServerImpl()
{
    if(m_sip_server) {
        delete m_sip_server;
    }

    if(m_xml_parser) {
        delete m_xml_parser;
    }

    if(m_stream_manager) {
        delete m_stream_manager;
    }

    if(m_waiter_manager) {
        delete m_waiter_manager;
    }

	if (m_subscribe_manager)
	{
		delete m_subscribe_manager;
	}

}

void CGBServerImpl::SetGBReceiver(const GBServerReceiver* receiver)
{
    m_gb_receiver  = (GBServerReceiver*)receiver;
}

int CGBServerImpl::Start(GB28181Version version, TransPorco proco , const char* GBCode, const char* ip , uint16_t port)
{
    SipTransportType type = kSipOverUDP;
   if( proco == kSipProcoOverTCP) {
        type = kSipOverTCP;
   }
   m_local_code = GBCode;
   NetAddress addr;
   addr.ip = (char*)ip;
   addr.port = port;
   m_sip_server->SetHandler(this);

   m_client_manager->m_local_code = GBCode;
   m_client_manager->m_local_ip = ip;
   m_client_manager->m_local_port = port;

   int code = m_sip_server->Start(type ,&addr, GBCode );
   if(code != kSipSuccess) {
       return kGbFail;
   }
       return kGb28181Success;
}

void CGBServerImpl::Stop()
{
    m_sip_server->Stop();
}

void CGBServerImpl::FreeQueryParam(QueryType type,  void* msg)
{
     if(type == kDeviceRecordQuery) {
           RecordIndex* index = (RecordIndex*)msg;
           if(index){
               if(index->record_list){
                   free(index->record_list);
               }
               free(index);
           }
     }
     if(type == kDevicePresetQuery) {
           PresetInfo* info = (PresetInfo*)msg;
           if(info){
               if(info->PresetList){
                   free(info->PresetList);
               }
               free(info);
           }
     }
}

void CGBServerImpl::CatalogQueryProcess(LPVOID lpThreadParameter)
{
       CatalogTimerStruct* handle = (CatalogTimerStruct*)lpThreadParameter;
       if(handle){
          handle->server->CatalogQueryTimer( handle->waiter);
          delete handle;
       }
}

void CGBServerImpl::CatalogQueryTimer(  CResponseWaiter* waiter)
{
    int timeout = 10000000;
    DeviceCatalogList catalog_list;
    if( !m_waiter_manager->WaitResponse(waiter, timeout, &catalog_list )) {
          return ;
    }

    if(  m_gb_receiver ) {
        m_gb_receiver->OnQueryResponse( kDeviceCatalogQuery,   catalog_list.GBCode,  &catalog_list );
    }
    if(catalog_list.catalog){
        free(catalog_list.catalog);
    }
}


   // device information query interface
int CGBServerImpl::QueryCatalog(SessionHandle handle,  const CatalogQueryParam* catalog)
{
    if(  !catalog ) {
        return kGbNullPointer;
    }
    int sn;
    std::string content;
    sn = m_xml_parser->PackCatalogQuery(catalog, content);
    if(content.empty()) {
        return kGbXmlEncodeFail;
    }

    SipMessage message;
    memset(&message,0,  sizeof(message));
    message.content = (char*)content.c_str();
    message.content_type = kSipContentMANSCDP_XML;

    CResponseWaiter* waiter = m_waiter_manager->CreateResponseWaiter(sn ,"Catalog", catalog->DeviceID  );
    CatalogTimerStruct*   catalog_timer = new CatalogTimerStruct;
    catalog_timer->server = this;
    catalog_timer->waiter = waiter;
    SHARESDK_RunOneThread(CatalogQueryProcess,
                                              catalog_timer,
                                              __FILE__,
                                              "",
                                              0,
                                              "query thread" );

   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, 0,NULL )){
         return kGbMessageFail;
   }

         return kGb28181Success;
}

int CGBServerImpl::QueryDeviceRecord(SessionHandle handle, const RecordParam*  param, int timeout,  RecordIndex** result)
{
    int sn;
    std::string content;
    sn = m_xml_parser->PackDeviceRecordQuery(param, content);
    if(content.empty()) {
        return kGbXmlEncodeFail;
    }

    SipMessage message;
    memset(&message,0,sizeof(message));
    message.content = (char*)content.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int sip_timeout = 3000;
    SipData* output = NULL;
   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, sip_timeout,  &output )){
         return false;
    }
   if(!output || output->messgae.code != kSuccessRequest) {
       m_sip_server->FreeSipResult(output);
       return kGbResponseCodeError;
   }
       m_sip_server->FreeSipResult(output);
   if(  timeout < 0 ||  result == NULL) {
       return kGb28181Success;
   }

   RecordIndex*  info =  (RecordIndex*)malloc(sizeof(RecordIndex));
   CResponseWaiter* waiter = m_waiter_manager->CreateResponseWaiter(sn ,"RecordInfo", param->DeviceID  );
   if( m_waiter_manager->WaitResponse(waiter, timeout, info )) {
        *result = info;
         return kGb28181Success;
   }
        *result = NULL;
         return kGbRequestTimeout;

}

int CGBServerImpl::QueryDeviceInfo(SessionHandle handle,  const char* deviceID, int timeout, DeviceInfo** result)
{

    int sn;
    std::string content;
    sn =  m_xml_parser->PackDeviceInfoQuery(deviceID, content);
    if(content.empty()) {
        return kGbXmlEncodeFail;
    }

    SipMessage message;
    memset(&message,0,  sizeof(message));
    message.content = (char*)content.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int sip_timeout = 3000;
    SipData* output= NULL;

   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, sip_timeout,  &output )){
         return kGbMessageFail;
    }

   if( !output || output->messgae.code != kSuccessRequest) {
        m_sip_server->FreeSipResult(output);
        return kGbResponseCodeError;
   }
        m_sip_server->FreeSipResult(output);

   if(  timeout < 0 ||  result == NULL) {
       return kGb28181Success;
   }

   DeviceInfo*  info =  (DeviceInfo*)malloc(sizeof(DeviceInfo));
   CResponseWaiter* waiter = m_waiter_manager->CreateResponseWaiter(sn ,"DeviceInfo", deviceID  );
   if( m_waiter_manager->WaitResponse(waiter, timeout, info )) {
        *result = info;
         return kGb28181Success;
   }
        *result = NULL;
         return kGbRequestTimeout;

}


int CGBServerImpl::QueryDeviceStatus(SessionHandle handle,const char* deviceID, int  timeout ,  DeviceStatus** result)
{
    std::string content;
    int sn =  m_xml_parser->PackDeviceStatusQuery(deviceID, content);
    if(content.empty()) {
        return kGbXmlEncodeFail;
    }

    SipMessage message;
     memset(&message,0,  sizeof(message));
    message.content = (char*)content.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    SipData* output = NULL;

   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, 3000,  &output )) {
         return kGbMessageFail;
    }

   if(output->messgae.code != kSuccessRequest) {
        m_sip_server->FreeSipResult(output);
       return kGbResponseCodeError;
   }
        m_sip_server->FreeSipResult(output);

    if(  timeout < 0 ||  result == NULL) {
        return kGb28181Success;
    }

    DeviceStatus*  status =  (DeviceStatus*)malloc(sizeof(DeviceStatus));
    CResponseWaiter* waiter = m_waiter_manager->CreateResponseWaiter(sn ,"DeviceStatus", deviceID  );
    if( m_waiter_manager->WaitResponse(waiter, timeout, status )) {
        *result = status;
         return kGb28181Success;
   }
        *result = NULL;
         return kGbRequestTimeout;
}



int CGBServerImpl::QueryDeviceConfig(SessionHandle handle,const ConfigDownloadQuery* param,  int timeout , DeviceConfigDownload** result)
{
    int sn;
    std::string content;
    sn =  m_xml_parser->PackConfigDownloadQuery(param, content);

    if(content.empty()) {
         return kGbXmlEncodeFail;
    }

    SipMessage message;
    memset(&message,0,  sizeof(message));
    message.content = (char*)content.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    SipData* output = NULL;


   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, 3000,  &output )) {
          return kGbMessageFail;
    }

   if(!output || output->messgae.code != kSuccessRequest) {
        m_sip_server->FreeSipResult(output);
        return kGbResponseCodeError;
   }
        m_sip_server->FreeSipResult(output);
   if(  timeout < 0 ||  result == NULL) {
       return kGb28181Success;
   }

   DeviceConfigDownload*  info =  (DeviceConfigDownload*)malloc(sizeof(DeviceConfigDownload));
   CResponseWaiter* waiter = m_waiter_manager->CreateResponseWaiter(sn ,"ConfigDownload", param->DeviceID  );
   if( m_waiter_manager->WaitResponse(waiter, timeout, info )) {
       *result = info;
        return kGb28181Success;
  }
       *result = NULL;
        return kGbRequestTimeout;

}

int CGBServerImpl::QueryDevicePresetInfo(SessionHandle handle, const char* deviceID,   int timeout, PresetInfo** result)
{
    int sn;
    std::string content;
    sn = m_xml_parser->PackQueryDevicePresetInfo(deviceID, content);
    if( content.empty()) {
         return kGbXmlEncodeFail;
    }

    SipMessage message;
    memset(&message,0,  sizeof(message));
    message.content = (char*)content.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int sip_timeout = 3000;
    SipData* output = NULL;

   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, sip_timeout,  &output )){
         return kGbMessageFail;
    }
   if(!output || output->messgae.code != kSuccessRequest) {
        m_sip_server->FreeSipResult(output);
        return kGbResponseCodeError;
   }
       m_sip_server->FreeSipResult(output);
   if(  timeout < 0 ||  result == NULL) {
       return kGb28181Success;
   }

   PresetInfo*  info =  (PresetInfo*)malloc(sizeof(PresetInfo));
   CResponseWaiter* waiter = m_waiter_manager->CreateResponseWaiter(sn ,"PresetQuery", deviceID  );
   if( m_waiter_manager->WaitResponse(waiter, timeout, info )) {
       *result = info;
        return kGb28181Success;
  }
       *result = NULL;
        return kGbRequestTimeout;

}

// stream media interface
int CGBServerImpl::StartLiveStreamRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle)
{
   return StreamRequest( handle,  input,  result,  stream_handle);
}

int CGBServerImpl::StartPlaybackRequest(SessionHandle handle, const MediaInfo* input,  MediaInfo* result, StreamHandle* stream_handle)
{
     return StreamRequest( handle,  input,  result,  stream_handle);
}

int CGBServerImpl::StartDownloadRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle)
{
     return StreamRequest( handle,  input,  result,  stream_handle);
}

int CGBServerImpl::StartAudioRequest(SessionHandle handle, const MediaInfo* input,  MediaInfo* result, StreamHandle* stream_handle)
{
     return StreamRequest( handle,  input,  result,  stream_handle);
}

int CGBServerImpl::StopStreamRequest(SessionHandle handle, StreamHandle stream_handle)
{
    if( !m_stream_manager->DeleteStreamHandle(stream_handle)){
        return kGbStreamHandleNotExist   ;
    }

    SipDialogKey* key = (SipDialogKey*)stream_handle;
    int timeout = 3000;
    SipData*  result = NULL;

    bool error_code;
    if(kSipSuccess != m_sip_server->Bye( handle, key, NULL, timeout, &result)) {
        error_code = kGbByeFail;
        goto finally;
    }

    if(  !result || result->messgae.code != kSuccessRequest) {
         error_code = kGbResponseCodeError;
         goto finally;
    }
         error_code = kGb28181Success;
finally:
        if(key)  {
            free(key);
        }
        m_sip_server->FreeSipResult(result);
        return  error_code;
}

int CGBServerImpl::PlaybackControl(SessionHandle handle, StreamHandle stream_handle,const PlayCtrlCmd* cmd)
{

    if( !m_stream_manager->FindStreamHandle(stream_handle)) {
        return kGbStreamHandleNotExist   ;
    }
    std::string res;
    RtspParser::CmdToString(cmd, res);

    SipMessage message;
    memset(&message,0, sizeof(message) );

    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSRTSP;

    SipDialogKey* key = (SipDialogKey*)stream_handle;
    if(kSipSuccess != m_sip_server->Info(handle, key, &message, 0, NULL)) {
       return kGbInfoFail;
    }
       return  kGb28181Success;
}

     // device control
int CGBServerImpl::DevicePtzControl(SessionHandle handle,const char* deviceID, const PtzCmd* cmd)
{
    std::string res;

    m_xml_parser->PackPTZControl(deviceID,  cmd, res);

    SipMessage message;
    memset(&message,0, sizeof(message) );

    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int timeout = 3000;

    SipData*  result = NULL;

    bool error_code;

    if(kSipSuccess != m_sip_server->Message(handle,  &message, timeout, &result)) {
        error_code = false;
        goto finally;
    }

    if( !result ||  result->messgae.code != kSuccessRequest) {
         error_code = false;
         goto finally;
    }
         error_code = true;
finally:
        m_sip_server->FreeSipResult(result);
        return  error_code;
}

int CGBServerImpl::DeviceRecordControl(SessionHandle handle,const char* deviceID, bool isRecord)
{
    if(  !deviceID ){
        return kGbNullPointer;
    }
    std::string res;

    m_xml_parser->PackRecordControl(deviceID,  isRecord, res);

    SipMessage message;
    memset(&message,0, sizeof(message) );

    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int timeout = 3000;

    SipData*  result = NULL;

    bool error_code;

    if(kSipSuccess != m_sip_server->Message(handle,  &message, timeout, &result)) {
        error_code = false;
        goto finally;
    }

    if( !result || result->messgae.code != kSuccessRequest) {
         error_code = false;
         goto finally;
    }
         error_code = true;
finally:
        m_sip_server->FreeSipResult(result);
        return  error_code;
}

    // true Defance   false Cancle
int CGBServerImpl::DeviceGurdControl(SessionHandle handle,const char* deviceID,bool opt)
{
    if(  !deviceID ){
        return kGbNullPointer;
    }
    std::string res;

    m_xml_parser->PackGurdControl(deviceID,  opt, res);

    SipMessage message;
    memset(&message,0, sizeof(message) );

    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int timeout = 3000;

    SipData*  result = NULL;

    bool error_code;

    if(kSipSuccess != m_sip_server->Message(handle,  &message, timeout, &result)) {
        error_code = false;
        goto finally;
    }

    if( !result || result->messgae.code != kSuccessRequest) {
         error_code = false;
         goto finally;
    }
         error_code = true;
finally:
        m_sip_server->FreeSipResult(result);
        return  error_code;

}

int CGBServerImpl::DeviceAlarmResetControl(SessionHandle handle, const AlarmResetInfo* info, const char* deviceID)
{
    if( !deviceID){
        return kGbNullPointer;
    }

    std::string res;
    m_xml_parser->PackAlarmResetControl(deviceID,info, res);
    if(res.empty()) {
        return false;
    }

    SipMessage message;
    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int timeout = 3000;
    SipData* output = NULL;
    int code;
   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, timeout,  &output )){
         return false;
    }
   if(!output || output->messgae.code != kSuccessRequest) {
       code = false;
       goto finally;
   }
       code  = true;
finally:
      m_sip_server->FreeSipResult(output);
      return code;
}

int CGBServerImpl::DeviceZoomControl(SessionHandle handle,const char* deviceID,const ZoomCmd* cmd)
{
    if(  !deviceID || !cmd) {
        return kGbNullPointer;
    }

    std::string res;
    m_xml_parser->PackZoomControl(  deviceID, cmd, res );
    if(res.empty()) {
        return false;
    }

    SipMessage message;
    memset(&message,0, sizeof(message) );
    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int timeout = 3000;
    SipData* output = NULL;
    int code;
   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, timeout,  &output )){
         return false;
    }
   if( !output || output->messgae.code != kSuccessRequest) {
       code = false;
       goto finally;
   }
       code  = true;
finally:
      m_sip_server->FreeSipResult(output);
      return code;
}

int CGBServerImpl::DeviceConfigSetControl(SessionHandle handle,const char* deviceID,const ConfigSetParam*cmd )
{
    if(  !deviceID || !cmd) {
        return kGbNullPointer;
    }
	return kGb28181Success;
}

int CGBServerImpl::DeviceKeyFrameControl(SessionHandle handle,const char* deviceID)
{
    if( !deviceID){
        return kGbNullPointer;
    }

    std::string res;
    m_xml_parser->PackKeyFrameControl(deviceID, res);
    if(res.empty()) {
        return false;
    }

    SipMessage message;
    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int timeout = 3000;
    SipData* output = NULL;
    int code;
   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, timeout,  &output )){
         return false;
    }
   if(!output || output->messgae.code != kSuccessRequest) {
       code = false;
       goto finally;
   }
       code  = true;
finally:
      m_sip_server->FreeSipResult(output);
      return code;
}

int CGBServerImpl::DeviceHomePositionControl(SessionHandle handle,const char* deviceID,const HomePositionCmd* cmd)
{
    if(  !deviceID || !cmd){
        return kGbNullPointer;
    }
    std::string res;
    m_xml_parser->PackHomePositionControl(deviceID,cmd, res);
    if(res.empty()) {
        return false;
    }

    SipMessage message;
    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
    int timeout = 3000;
    SipData* output = NULL;
    int code;
   if( kSipSuccess !=m_sip_server->Message((SipSessionHandle)handle, &message, timeout,  &output )){
         return false;
    }
   if(!output || output->messgae.code != kSuccessRequest) {
       code = false;
       goto finally;
   }
       code  = true;
finally:
      m_sip_server->FreeSipResult(output);
      return code;
}

    // subscribe
int CGBServerImpl::AlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info,SubscribeHandle* subscribe_handle)
{
	std::string res;
	SipDialogKey   *dialog  = NULL;
    std::string AlarmInfo;
    m_xml_parser->PackAlarmSubcribe(info, res);
    if(res.empty()) {
        return false;
    }

    SipMessage message;
	memset(&message, 0, sizeof(message));
    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
	message.Expirse = info->Expires;
    message.Event = (char*)"Alarm";

    int timeout = 3000;
    SipData* output = NULL;
    int code;
   if( kSipSuccess !=m_sip_server->Subcribe((SipSessionHandle)handle, &message, timeout,  &output )){
         return false;
    }
   if(!output || output->messgae.code != kSuccessRequest) {
       code = false;
       goto finally;
   }
   code  = kGb28181Success;
   dialog = (SipDialogKey*)malloc( sizeof(SipDialogKey) );
   memcpy(dialog,   &(output->Dialog),  sizeof(*dialog));
   *subscribe_handle = (SubscribeHandle)dialog;
   m_subscribe_manager->InsertSubscribeHandle(*subscribe_handle,message.Expirse);
   m_subscribe_manager->SetMessage(ExpireMessageSender,  m_gb_receiver  ); //注册回调函数
finally:
      m_sip_server->FreeSipResult(output);
      return code;

}

int CGBServerImpl::CatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info, SubscribeHandle* subscribe_handle)
{  
    std::string res;
	SipDialogKey   *dialog  = NULL;
	std::string CatalogInfo;
    m_xml_parser->PackCatalogSubcribe(info, res);
    if(res.empty()) {
        return false;
    }

    SipMessage message;
	memset(&message, 0, sizeof(message));
    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
	message.Expirse = info->Expires;
    message.Event = (char*)"Catalog";

    int timeout = 3000;
    SipData* output = NULL;
    int code;
	if( kSipSuccess !=m_sip_server->Subcribe((SipSessionHandle)handle, &message, timeout,  &output )){
			*subscribe_handle =NULL;
			return kGbSubScribeFail;
	}
	if( !output || output->messgae.code != kSuccessRequest) {
		*subscribe_handle =NULL;
		code = kGbSubScribeFail;
		goto finally;
	}
    code  = kGb28181Success;
	dialog = (SipDialogKey*)malloc( sizeof(SipDialogKey) );
	memcpy(dialog,   &(output->Dialog),  sizeof(*dialog));
	*subscribe_handle = (SubscribeHandle)dialog;
	m_subscribe_manager->InsertSubscribeHandle(*subscribe_handle,message.Expirse);
	m_subscribe_manager->SetMessage(ExpireMessageSender,  m_gb_receiver  ); //注册回调函数
finally:
      m_sip_server->FreeSipResult(output);
      return code;
}

int CGBServerImpl::RefreshCatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info, SubscribeHandle subscribe_handle)
{
	if (!m_subscribe_manager->FindSubscribeHandle(subscribe_handle)) {
		return kGbSubHandleNotExist;
	}

	std::string res;
	SipDialogKey   *dialog  = NULL;
	m_xml_parser->PackCatalogSubcribe(info, res);
	if(res.empty()) {
		return kGbXmlEncodeFail;
	}
	SipMessage message;
	memset(&message, 0, sizeof(message));
	message.content = (char*)res.c_str();
	message.content_type = kSipContentMANSCDP_XML;
	message.Expirse = info->Expires;
    message.Event = (char*)"Catalog";

	int timeout = 3000;
	SipData* output = NULL;
	int code;
	dialog  = (SipDialogKey*)(subscribe_handle);
	
	if( kSipSuccess !=m_sip_server->RefrsehSubcribe((SipSessionHandle)handle, &message,dialog, timeout, &output )){
		code = kGbSubScribeFail;
		goto finally;
	}

	if( !output || output->messgae.code != kSuccessRequest) {
		code = kGbSubScribeFail;
		goto finally;
	}
	code  = kGb28181Success;
finally:
	m_sip_server->FreeSipResult(output);
	return code;
}

int CGBServerImpl::RefreshAlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info, SubscribeHandle subscribe_handle)
{
	if (!m_subscribe_manager->FindSubscribeHandle(subscribe_handle)) {
		return kGbSubHandleNotExist;
	}

	std::string res;
	SipDialogKey   *dialog  = NULL;
	m_xml_parser->PackAlarmSubcribe(info, res);
	if(res.empty()) {
		return kGbXmlEncodeFail;
	}
	SipMessage message;
	memset(&message, 0, sizeof(message));
	message.content = (char*)res.c_str();
	message.content_type = kSipContentMANSCDP_XML;
	message.Expirse = info->Expires;
    message.Event = (char*)"Alarm";

	int timeout = 3000;
	SipData* output = NULL;
	int code;
	dialog  = (SipDialogKey*)(subscribe_handle);

	if( kSipSuccess !=m_sip_server->RefrsehSubcribe((SipSessionHandle)handle, &message,dialog, timeout, &output )){
		code = kGbSubScribeFail;
		goto finally;
	}

	if( !output || output->messgae.code != kSuccessRequest) {
		code = kGbSubScribeFail;
		goto finally;
	}
	code  = kGb28181Success;
finally:
	m_sip_server->FreeSipResult(output);
	return code;
}

int CGBServerImpl::TermainateSubcribe(SipSessionHandle handle, SubscribeHandle sub_handle)
{
	if(!m_subscribe_manager->FindSubscribeHandle(sub_handle)){
		return kGbSubHandleNotExist;
	} 
	if(!m_subscribe_manager->DeleteSubscribeHandle(sub_handle)){
		return kGbSubHandleNotExist;
	}
	
	SipDialogKey   *dialog  = NULL;
	
	SipMessage message;
	memset(&message,0,sizeof(message));
	message.Expirse = 0;


	int code;
	dialog  = (SipDialogKey*)(sub_handle);
    //SipDialogKey* key = (SipDialogKey*)sub_handle;
	if( kSipSuccess != m_sip_server->RefrsehSubcribe((SipSessionHandle)handle, &message,dialog, 0, NULL)){
		code = kGbFail;
	}else{
	     code  = kGb28181Success;
	}
	
	return code;
}
int CGBServerImpl::MobilePositionSubcribe(SessionHandle handle, const MobilePositionSubInfo* info)
{
    if( !info){
        return kGbNullPointer;
    }

    std::string res;
    m_xml_parser->PackMobilePositionSubcribe(info,res);
    if(res.empty()) {
        return false;
    }

    SipMessage message;
	memset(&message, 0, sizeof(message));
    message.content = (char*)res.c_str();
    message.content_type = kSipContentMANSCDP_XML;
	message.Expirse = info->Expires;
    message.Event = (char*)"MobilePosition";

    int timeout = 3000;
    SipData* output = NULL;
    int code;
   if( kSipSuccess !=m_sip_server->Subcribe((SipSessionHandle)handle, &message, timeout,  &output )){
         return false;
    }
   if(!output || output->messgae.code != kSuccessRequest) {
       code = false;
       goto finally;
   }
       code  = true;
finally:
      m_sip_server->FreeSipResult(output);
      return code;
}

int   CGBServerImpl::OnClientRegist(SipSessionHandle handle, const SipRegistParam* regist, const SipConnectParam* connect)
{
      GBRegistParam param;
      ConnectParam  gbconnect;
      memset(&param,0,sizeof(param));
      memset(&gbconnect,0,sizeof(gbconnect));
      param.expires = regist->expires;

      GBUtil::memcpy_safe( param.password, STR_LEN,regist->password);
      GBUtil::memcpy_safe( param.username, STR_LEN,regist->user_name);
      GBUtil::memcpy_safe(  gbconnect.GBCode ,GB_ID_LEN, connect->sip_code);
      GBUtil::memcpy_safe(  gbconnect.ip, IP_LEN, connect->ip);

      gbconnect.port = connect->port;

      if( m_gb_receiver && !m_gb_receiver->OnRegist((SessionHandle)handle, &param,&gbconnect ) ) {
          return kForbidden;
      }

      if(param.expires > 0) {
          m_client_manager->Insert(handle, &gbconnect);
      }else{
          m_client_manager->DeleteClient(handle);
      }

          return kSuccessRequest;
}


void CGBServerImpl::OnRequest(SipSessionHandle handle, SipMethod  method, const SipData* data)
{

    switch((int)method)
    {
         case kSipNotifyMethod:
         case kSipMessageMethod :
         case kSipSubscribeMethod:  OnMessage(handle,data);     break;
    }

}

void  CGBServerImpl::OnMessage(SipSessionHandle handle, const SipData* data)
{
    SipMessage message;
    memset(&message,0, sizeof(message));
	message.Method = kSipMessageMethod;
    message.code = kBadRequest;
    if( data->messgae.content_type != kSipContentMANSCDP_XML) {
         m_sip_server->Response(handle, &(data->Dialog), &message );
         return ;
    }

     ProtocolType  proco =  (ProtocolType)m_xml_parser->GetProtocolType( data->messgae.content  );
    if( proco == kInvailProco  ) {
          m_sip_server->Response(handle, &(data->Dialog), &message );
          return;
     }

      if( proco > kNotifyProco  && proco < kResponseProco ) {
              OnNotify( handle,proco, data);
      }

      if( proco > kResponseProco ) {
            OnGBProcoResponse( handle,proco, data);
		  
          
      }



}

void  CGBServerImpl::OnNotify( SipSessionHandle handle, ProtocolType  type, const SipData* data)
{
    SipMessage message;
    memset(&message, 0 , sizeof(message));
    message.code = kSuccessRequest;
	
    int sn;
    int result = true;
    NotifyInfo info;
    memset(&info, 0 , sizeof(info));
    info.type  = GetNotifyType(type);
    std::string content;
	StreamHandle stream_handle = NULL;
	SubscribeHandle subscribe_handle = NULL;

    if(!m_xml_parser->UnPackNotifyInfo(data->messgae.content, sn, info )) {
       message.code = kBadRequest;
       goto finally;
    }

	if (info.type == kMediaStatusNotify) {
		stream_handle = m_stream_manager->FindStreamHandle(data->Dialog.CallId, data->Dialog.DialogId);
		if (stream_handle == NULL) {
			message.code = kBadRequest;
			goto finally;
		}
	    info.notify_message.media_status.handle = stream_handle;
	}

	if (info.type == kCatalogNotify) {
		subscribe_handle = m_subscribe_manager->FindSubscribeHandle(data->Dialog.DialogId);
		if (subscribe_handle == NULL) {
			message.code = kBadRequest;
			goto finally;
		}
	}

    if( m_gb_receiver && !m_gb_receiver->OnNotify(info.type, info.GBCode, &info )) {
        message.code = kForbidden;
    }

finally:
    if( message.code != kSuccessRequest  ){
         result = false;
    }

	if (info.type == kMediaStatusNotify) {
		return;
	}
	
    m_xml_parser->PackResponse( "e", sn,  result,  info.GBCode, content  );
	if( info.type == kCatalogNotify || info.type == kAlarmForSubNotify
		|| info.type == kMobilePositionNotify  )
	{
		message.Method = kSipNotifyMethod;
	}else{
		message.Method = kSipMessageMethod;
	}
    message.content  = (char*)content.c_str();
    m_sip_server->Response(handle, &(data->Dialog), &message );

    if(info.type == kCatalogNotify && info.notify_message.catalog_list.catalog) {
        free(info.notify_message.catalog_list.catalog);
    }

    if(info.type == kKeepaliveNotify && info.notify_message.keepalive.fault_device ) {
        free(info.notify_message.keepalive.fault_device);
    }
}

void  CGBServerImpl::OnCatalogNotify(SipSessionHandle handle,const SipData* data)
{
	
}

void  CGBServerImpl::OnAlarmNotify(SipSessionHandle handle,const SipData* data)
{
	
}

void  CGBServerImpl::OnKeepaliveNotify(SipSessionHandle handle,const SipData* data)
{

}

void  CGBServerImpl::OnMobilePositionNotify(SipSessionHandle handle,const SipData* data)
{

}


void  CGBServerImpl::OnGBProcoResponse( SipSessionHandle handle, ProtocolType  type, const SipData* data)
{
    SendSipMessageWithOk(handle, data);
    switch( (int)type) {
       case kCatalogResponseProco:             OnCatalogResponse(handle, data) ;break;
       case kControlResponseProco:              OnDeviceControlResponse(handle, data);          break;
       case kRecordIndexResponseProco:       OnRecordIndexResponse(handle, data);             break;
       case kDevStatusResponseProco:          OnDevStatusResponse(handle, data);                 break;
       case kDevInfoResponseProco:               OnDevInfoResponse(handle, data);                break;
       case kPresetResponseProco:                 OnPresetResponse(handle,data);                   break;
       case kConfingDownloadResponseProco: OnConfingDownLoadResponse(handle, data); break;
    }


}

int    CGBServerImpl::SendSipMessageWithOk(SipSessionHandle handle, const SipData* data)
{
    SipMessage message;
    memset(&message, 0 , sizeof(message));
    message.code = kSuccessRequest;
    return m_sip_server->Response(handle, &(data->Dialog),   &message );
}

void CGBServerImpl::ExpireMessageSender (SubscribeHandle handle, void* user)
{
	if (!user){
		return;
	}
	GBServerReceiver* subscribe_manager = (GBServerReceiver*)user;
	subscribe_manager->OnNotify(kSubscribeExpireNotify,NULL, handle);
}

void  CGBServerImpl::OnCatalogResponse(SipSessionHandle handle, const SipData* data)
{
    int sn ;
    int sum;
    DeviceCatalogList  list;
    memset(&list, 0 , sizeof(list));
    m_xml_parser->UnPackCatalogResponse(data->messgae.content, sn, sum, &list );
    m_waiter_manager->WakeUpWaitResponse(sn ,"Catalog", list.GBCode ,sum, &list );
    if(list.catalog) {
        free(list.catalog);
    }
}


void  CGBServerImpl::OnConfingDownLoadResponse(SipSessionHandle handle, const SipData* data)
{

/*
    int sn ;
    int sum;
    ConfigDownloadQuery  info;
    memset(&info, 0 , sizeof(info));
    m_xml_parser->UnPackConfigDownloadQuery( data->messgae.content,  &info );

    if(  m_gb_receiver ) {
        m_gb_receiver->OnQueryResponse( kDeviceConfigQuery,   info.DeviceID,  &info    );
    }
*/
}


void  CGBServerImpl::OnPresetResponse(SipSessionHandle handle, const SipData* data)
{

    int sn;
    PresetInfo  info;
    memset(&info, 0 , sizeof(info));
    m_xml_parser->UnPackDevicePresetResponse( data->messgae.content, sn, &info );

    if(m_waiter_manager->WakeUpWaitResponse(sn ,"PresetQuery", info.DeviceID , info.PersetListNum, &info )){
        goto finally;
    }

    if(  m_gb_receiver ) {
        m_gb_receiver->OnQueryResponse( kDevicePresetQuery,   info.DeviceID,  &info    );
    }
finally:
     if(info.PresetList){
         free(info.PresetList);
     }
}

void  CGBServerImpl::OnDevInfoResponse(SipSessionHandle handle, const SipData* data)
{
    int sn;
    DeviceInfo  info;
    memset(&info, 0 , sizeof(info));
    m_xml_parser->UnPackDeviceInfoResponse( data->messgae.content, sn, &info );

    if(m_waiter_manager->WakeUpWaitResponse(sn ,"DeviceInfo", info.GBCode , 0, &info )) {
        return;
    }

    if(  m_gb_receiver ) {
        m_gb_receiver->OnQueryResponse( kDeviceInfoQuery,   info.GBCode , &info    );
    }
}

void  CGBServerImpl::OnDevStatusResponse(SipSessionHandle handle, const SipData* data)
{
        SendSipMessageWithOk(handle, data);

        DeviceStatus status;
        memset(&status, 0 , sizeof(status));
        int sn ;
        m_xml_parser->UnPackDeviceStatusResponse( data->messgae.content, sn, &status );

        if(m_waiter_manager->WakeUpWaitResponse(sn ,"DeviceStatus", status.GBCode ,status.AlarmNum, &status )){
             goto finally;
        }

        if(  m_gb_receiver) {
            m_gb_receiver->OnQueryResponse( kDeviceStatusQuery,   status.GBCode,  &status    );
        }
finally:
       if(status.Alarm_Status) {
           free(status.Alarm_Status);
       }
}

void  CGBServerImpl::OnRecordIndexResponse(SipSessionHandle handle, const SipData* data)
{
    int sn;
    int sum;
    RecordIndex index;
    memset(&index, 0 , sizeof(index));
    m_xml_parser->UnPackDeviceRecordIndexResponse(  data->messgae.content, sn, sum, &index );


    if(m_waiter_manager->WakeUpWaitResponse(sn ,"RecordInfo", index.GBCode , sum, &index )){
           goto finally;
    }

    if(  m_gb_receiver) {
          m_gb_receiver->OnQueryResponse( kDeviceRecordQuery,   index.GBCode,  &index    );
    }
finally:
    if(index.record_list) {
        free(index.record_list);
    }

}


void  CGBServerImpl::OnDeviceControlResponse(SipSessionHandle handle, const SipData* data)
{
    SendSipMessageWithOk(handle, data);
}


void CGBServerImpl::OnResponse(SipSessionHandle handle,SipMethod  method,const SipData* data)
{

}

std::string   CGBServerImpl::GenerateSubject( const std::string& devid, int StreamNum )
{
    std::ostringstream os;
    os << devid << ":" << StreamNum << ","<< m_local_code <<":0";
    std::string Subject = os.str();
    return Subject;
}


int    CGBServerImpl::StreamRequest(SessionHandle handle, const MediaInfo* input,  MediaInfo* output, StreamHandle* stream_handle)
{
    std::string res;
    CSdpUtil::ToString(input,res);

    SipMessage message;
    memset(&message, 0, sizeof(message));
    std::string from;
    std::string to;
    std::string route;


    if(m_client_manager->GenerateSipAddress(handle,input->DeviceID, from,to,route   )) {
          message.From = (char*)from.c_str();
          message.To = (char*)to.c_str();
    }

    message.content = (char*)res.c_str();
    message.content_type = kSipContentSDP;
    std::string subject = GenerateSubject(input->DeviceID, input->StreamNum);
    message.Subject = (char*)subject.c_str();

    int timeout = 300000;
    int code;
    SipData* result = NULL;
    SipDialogKey   *dialog  = NULL;
    std::string media_info;

    if(kSipSuccess != m_sip_server->Invite((SipSessionHandle)handle, &message, timeout,  &result)){
        return kGbInviteFail;
    }

    if( !result || result->messgae.code != kSuccessRequest) {
        code = kGbResponseCodeError;
        goto finally;
    }

    media_info = result->messgae.content;
    if( !CSdpUtil::String2MediaInfo(media_info.c_str(), output)) {
        code = kGbSdpParseFail;
        goto finally;
    }

    if(kSipSuccess != m_sip_server->Ack((SipSessionHandle)handle,&(result->Dialog), &message, 0,  NULL)){
        code = kGbAckFail;
        goto finally;
    }

    dialog = (SipDialogKey*)malloc( sizeof(SipDialogKey) );
    memcpy(dialog,   &(result->Dialog),  sizeof(*dialog));
    *stream_handle = (StreamHandle)dialog;
    m_stream_manager->InsertStreamHandle(   *stream_handle     );
        code = kGb28181Success;
finally:
       if (result && code != kGb28181Success) {
		   m_sip_server->Bye((SipSessionHandle)handle, &(result->Dialog), NULL, 0, NULL);
       }
        m_sip_server->FreeSipResult(result);
        return code;
}

