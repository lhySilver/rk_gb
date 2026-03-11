#include "GB28181ServerSDK.h"
#include "GBServerImpl.h"

GB28181ServerSDK::GB28181ServerSDK()
{
     m_server_Impl = new CGBServerImpl;
}

GB28181ServerSDK::~GB28181ServerSDK()
{
     if(m_server_Impl) {
         delete m_server_Impl;
     }
}

void GB28181ServerSDK::SetGBReceiver(const GBServerReceiver* receiver)
{
    m_server_Impl->SetGBReceiver(receiver);
}

int GB28181ServerSDK::Start(GB28181Version version, TransPorco proco , const char* GBCode, const char* ip , uint16_t port)
{
    if(!GBCode){
        return kGbNullPointer;
    }
    return m_server_Impl->Start(version, proco , GBCode, ip , port);
}

void GB28181ServerSDK::Stop()
{
    m_server_Impl->Stop();
}

void GB28181ServerSDK::FreeQueryParam(QueryType type, void* msg)
{
    m_server_Impl->FreeQueryParam(type,msg);
}

   // device information query interface
int GB28181ServerSDK::QueryCatalog(SessionHandle handle,  const CatalogQueryParam* catalog)
{
    if(  !catalog ){
        return kGbNullPointer;
    }
    return m_server_Impl->QueryCatalog(handle, catalog);
}

// device information query interface
int GB28181ServerSDK::QueryDeviceRecordIndex(SessionHandle handle, const RecordParam*  param, int timeout,  RecordIndex** result)
{
 if(  !param ) {
     return kGbNullPointer;
 }
 return m_server_Impl->QueryDeviceRecord(handle,   param,  timeout,  result);
}


int GB28181ServerSDK::QueryDeviceInfo(SessionHandle handle,  const char* deviceID, int timeout, DeviceInfo** info)
{
    if( !deviceID){
        return kGbNullPointer;
    }
   return m_server_Impl->QueryDeviceInfo(handle,   deviceID,timeout, info);
}


int GB28181ServerSDK::QueryDeviceStatus(SessionHandle handle,const char* deviceID, int  timeout ,  DeviceStatus** output)

{
    if( !deviceID) {
        return kGbNullPointer;
    }
    return m_server_Impl->QueryDeviceStatus(handle, deviceID,timeout, output);
}



int GB28181ServerSDK::QueryDeviceConfig(SessionHandle handle,const ConfigDownloadQuery* param,  int timeout , DeviceConfigDownload** config)
{
    if(  !param ) {
        return kGbNullPointer;
    }
     return m_server_Impl->QueryDeviceConfig(handle,  param, timeout, config);
}

int GB28181ServerSDK::QueryDevicePresetInfo(SessionHandle handle, const char* deviceID,   int timeout, PresetInfo** preset)
{
    if( !deviceID) {
        return kGbNullPointer;
    }
    return m_server_Impl->QueryDevicePresetInfo(handle,  deviceID, timeout, preset);
}

// stream media interface
int GB28181ServerSDK::StartLiveStreamRequest(SessionHandle handle, const MediaInfo* input,  MediaInfo* result, StreamHandle* stream_handle)
{
    if( !input || !result || !stream_handle){
        return kGbNullPointer;
    }
    return m_server_Impl->StartLiveStreamRequest(handle,  input, result,  stream_handle);
}

int GB28181ServerSDK::StartPlaybackRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle)
{
    if( !input || !result || !stream_handle){
        return kGbNullPointer;
    }
    return m_server_Impl->StartLiveStreamRequest(handle,  input, result,  stream_handle);
}

int GB28181ServerSDK::StartDownloadRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle)
{
    if( !input || !result || !stream_handle){
        return kGbNullPointer;
    }
    return m_server_Impl->StartLiveStreamRequest(handle,  input, result,  stream_handle);
}

int GB28181ServerSDK::StartAudioRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle)
{
    if( !input || !result || !stream_handle){
        return kGbNullPointer;
    }
    return m_server_Impl->StartLiveStreamRequest(handle,  input, result,  stream_handle);
}

int GB28181ServerSDK::StopStreamRequest(SessionHandle handle, StreamHandle stream_handle)
{
    return m_server_Impl->StopStreamRequest(handle, stream_handle);
}

int GB28181ServerSDK::PlaybackControl(SessionHandle handle, StreamHandle stream_handle, const PlayCtrlCmd* cmd)
{
    if( !cmd){
        return kGbNullPointer;
    }
     return m_server_Impl->PlaybackControl( handle, stream_handle, cmd);
}

     // device control
int GB28181ServerSDK::DevicePTZControl(SessionHandle handle,const char* deviceID, const PtzCmd* cmd)
{
    if(  !deviceID || !cmd){
        return kGbNullPointer;
    }
    return m_server_Impl->DevicePtzControl(handle,deviceID,  cmd);
}

int GB28181ServerSDK::DeviceRecordControl(SessionHandle handle,const char* deviceID, bool isRecord)
{
    if(  !deviceID ){
        return kGbNullPointer;
    }
     return m_server_Impl->DeviceRecordControl(handle, deviceID, isRecord);
}

    // true Defance   false Cancle
int GB28181ServerSDK::DeviceGurdControl(SessionHandle handle,const char* deviceID,bool opt)
{
    if(  !deviceID ){
        return kGbNullPointer;
    }
    return m_server_Impl->DeviceGurdControl(handle,deviceID,  opt);
}

int GB28181ServerSDK::DeviceAlarmResetControl(SessionHandle handle, const AlarmResetInfo* info,const char* deviceID)
{
    if(  !deviceID ){
        return kGbNullPointer;
    }
    return m_server_Impl->DeviceAlarmResetControl(handle, info ,deviceID);
}

int GB28181ServerSDK::DeviceZoomControl(SessionHandle handle,const char* deviceID,const ZoomCmd* cmd)
{
    if(  !deviceID || !cmd){
        return kGbNullPointer;
    }
    return m_server_Impl->DeviceZoomControl( handle, deviceID, cmd);
}

int GB28181ServerSDK::DeviceConfigSetControl(SessionHandle handle,const char* deviceID,const ConfigSetParam*cmd )
{
    if(  !deviceID || !cmd){
        return kGbNullPointer;
    }
     return m_server_Impl->DeviceConfigSetControl(handle, deviceID, cmd);
}

int GB28181ServerSDK::DeviceKeyFrameControl(SessionHandle handle,const char* deviceID)
{
    if(  !deviceID ) {
        return kGbNullPointer;
    }
     return m_server_Impl->DeviceKeyFrameControl(handle, deviceID);
}

int GB28181ServerSDK::DeviceHomePositionControl(SessionHandle handle,const char* deviceID,const HomePositionCmd* cmd)
{
    if(  !deviceID || !cmd){
        return kGbNullPointer;
    }
    return m_server_Impl->DeviceHomePositionControl(handle,deviceID, cmd);
}

    // subscribe
int GB28181ServerSDK::AlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info,SubscribeHandle* sub_handle)
{
    if( !info){
        return kGbNullPointer;
    }
     return m_server_Impl->AlarmSubcribe( handle, info, sub_handle);
}

int GB28181ServerSDK::CatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info,SubscribeHandle* sub_handle)
{
    if( !info) {
        return kGbNullPointer;
    }
     return m_server_Impl->CatalogSubcribe(handle, info,sub_handle);
}

int GB28181ServerSDK::RefreshCatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info,SubscribeHandle sub_handle)
{
	if( !info) {
		return kGbNullPointer;
	}
	return m_server_Impl->RefreshCatalogSubcribe(handle, info,sub_handle);
}

int GB28181ServerSDK::RefreshAlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info,SubscribeHandle sub_handle)
{
	if( !info) {
		return kGbNullPointer;
	}
	return m_server_Impl->RefreshAlarmSubcribe(handle, info,sub_handle);

}

int GB28181ServerSDK::MobilePositionSubcribe(SessionHandle handle, const MobilePositionSubInfo* info, SubscribeHandle* sub_handle)
{
    if( !info) {
        return kGbNullPointer;
    }
    return m_server_Impl->MobilePositionSubcribe(handle, info);
}

int GB28181ServerSDK::TerminateSubcribe(SessionHandle handle,SubscribeHandle sub_handle )
{
    return m_server_Impl->TermainateSubcribe(handle,sub_handle);
}

