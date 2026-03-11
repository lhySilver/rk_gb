#ifndef CGBSERVERIMPL_H
#define CGBSERVERIMPL_H
#include "GB28181XmlParser.h"
#include "GB28181Defs.h"
#include "SipDefs.h"
#include "Interlocked.h"
#include "SubscribeManager.h"

class SipUserAgentServer;
class CGB28181XmlParser;
class CStreamRequestManager;
class CWaiterManager;
class CResponseWaiter;
class CGBClientInfoManager;

class CGBServerImpl : public SipServerHandler
{
public:
    CGBServerImpl();
    ~CGBServerImpl();
    int Start(GB28181Version version, TransPorco proco , const char* GBCode, const char* ip , uint16_t port);
    void Stop();

    void SetGBReceiver(const GBServerReceiver* receiver);

    // device information query interface
    int   QueryCatalog(SessionHandle handle,  const CatalogQueryParam* catalog);
    int   QueryDeviceInfo(SessionHandle handle,  const char* deviceID, int timeout, DeviceInfo** info);
    int   QueryDeviceStatus(SessionHandle handle,const char* deviceID, int  timeout ,  DeviceStatus** status);
    int   QueryDeviceConfig(SessionHandle handle,const ConfigDownloadQuery* param,  int timeout , DeviceConfigDownload** config );
    int   QueryDevicePresetInfo(SessionHandle handle, const char* deviceID,   int timeout, PresetInfo** preset);
    int   QueryDeviceRecord(SessionHandle handle, const RecordParam*  param, int timeout,  RecordIndex** output);
    void FreeQueryParam(QueryType type,  void* msg);

    // stream media interface
    int StartLiveStreamRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle);
    int StartPlaybackRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle);
    int StartDownloadRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle);
    int StartAudioRequest(SessionHandle handle, const MediaInfo* input,  MediaInfo* result, StreamHandle* stream_handle);
    int StopStreamRequest(SessionHandle handle, StreamHandle stream_handle);
    int PlaybackControl(SessionHandle handle, StreamHandle stream_handle, const PlayCtrlCmd* cmd);

     // device control
    int DevicePtzControl(SessionHandle handle,const char* deviceID, const PtzCmd* cmd);
    int DeviceRecordControl(SessionHandle handle,const char* deviceID, bool start);

    // true Defance   false Cancle
    int DeviceGurdControl(SessionHandle handle,  const char* deviceID,bool opt);
    int DeviceAlarmResetControl(SessionHandle handle,  const AlarmResetInfo* info, const char* deviceID);
    int DeviceZoomControl(SessionHandle handle,const char* deviceID,const ZoomCmd* cmd);
    int DeviceConfigSetControl(SessionHandle handle,const char* deviceID,const ConfigSetParam*cmd );
    int DeviceKeyFrameControl(SessionHandle handle,const char* deviceID);
    int DeviceHomePositionControl(SessionHandle handle,const char* deviceID,const HomePositionCmd* cmd);

	
    // subscribe
    int AlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info,SubscribeHandle* subscribe_handle);
    int CatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info, SubscribeHandle* subscribe_handle);
	int RefreshCatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info, SubscribeHandle subscribe_handle);
	int RefreshAlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info, SubscribeHandle subscribe_handle);
    int MobilePositionSubcribe(SessionHandle handle, const MobilePositionSubInfo* info);
	int TermainateSubcribe(SipSessionHandle handle,SubscribeHandle sub_handle);
	//SubscribeCallback
	static void ExpireMessageSender (SubscribeHandle handle,void* user);

private:
    static void CatalogQueryProcess(LPVOID lpThreadParameter);
    void CatalogQueryTimer(CResponseWaiter* waiter);
    std::string   GenerateSubject( const std::string& devid, int StreamNum );
    virtual int   OnClientRegist(SipSessionHandle handle, const SipRegistParam* regist, const SipConnectParam* param);
    virtual void OnRequest(SipSessionHandle handle,SipMethod  method,const SipData* data);
    virtual void OnResponse(SipSessionHandle handle,SipMethod  method,const SipData* data);

    void  OnMessage(SipSessionHandle handle, const SipData* data);

    // Notify
    void  OnNotify( SipSessionHandle handle,ProtocolType  type, const SipData* data);
    void  OnCatalogNotify(SipSessionHandle handle,const SipData* data);
    void  OnAlarmNotify(SipSessionHandle handle,const SipData* data);
    void  OnKeepaliveNotify(SipSessionHandle handle,const SipData* data);
    void  OnMobilePositionNotify(SipSessionHandle handle,const SipData* data);

   //reponse
    void  OnGBProcoResponse( SipSessionHandle handle,ProtocolType  type, const SipData* data);
    void  OnCatalogResponse(SipSessionHandle handle, const SipData* data);
    void  OnRecordIndexResponse(SipSessionHandle handle, const SipData* data);
    void  OnPresetResponse(SipSessionHandle handle, const SipData* data);
    void  OnDevStatusResponse(SipSessionHandle handle, const SipData* data);
    void  OnDevInfoResponse(SipSessionHandle handle, const SipData* data);
    void  OnDeviceControlResponse(SipSessionHandle handle, const SipData* data);
    void  OnConfingDownLoadResponse(SipSessionHandle handle, const SipData* data);

    int    StreamRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle);
    int    SendSipMessageWithOk(SipSessionHandle handle, const SipData* data);


private:
    std::string                            m_local_code;
    SipUserAgentServer*            m_sip_server;
    CGB28181XmlParser*          m_xml_parser;
    CStreamRequestManager*      m_stream_manager;
    GBServerReceiver*               m_gb_receiver;
    CWaiterManager*                 m_waiter_manager;
    CGBClientInfoManager*       m_client_manager;
	CSubscribeManager*          m_subscribe_manager;    
};

#endif // CGBSERVERIMPL_H
