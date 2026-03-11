#ifndef GB28181SERVERSDK_H
#define GB28181SERVERSDK_H
#include "GB28181Defs.h"

class CGBServerImpl;

class EXPORT_GB28181_SDK GB28181ServerSDK
{
public:
    GB28181ServerSDK();
    ~GB28181ServerSDK();

    int Start(GB28181Version version, TransPorco proco , const char* GBCode, const char* ip , uint16_t port);
    void Stop();
    void SetGBReceiver(const GBServerReceiver* receiver);

   // device information query interface
    int QueryCatalog(SessionHandle handle,  const CatalogQueryParam* catalog);
    int QueryDeviceInfo(SessionHandle handle,  const char* deviceID, int timeout, DeviceInfo** status);
    int QueryDeviceStatus(SessionHandle handle,const char* deviceID, int  timeout ,  DeviceStatus** status);
    int QueryDeviceConfig(SessionHandle handle,const ConfigDownloadQuery* param,  int timeout , DeviceConfigDownload** config);
    int QueryDevicePresetInfo(SessionHandle handle, const char* deviceID,   int timeout, PresetInfo** preset);
    int QueryDeviceRecordIndex(SessionHandle handle, const RecordParam*  param, int timeout,  RecordIndex** result);
    void FreeQueryParam(QueryType type, void* msg);
    // stream media interface
    int StartLiveStreamRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle);
    int StartPlaybackRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle);
    int StartDownloadRequest(SessionHandle handle, const MediaInfo* input, MediaInfo* result, StreamHandle* stream_handle);
    int StartAudioRequest(SessionHandle handle, const MediaInfo* input,  MediaInfo* result, StreamHandle* stream_handle);
    int StopStreamRequest(SessionHandle handle, StreamHandle stream_handle);
    int PlaybackControl(SessionHandle handle, StreamHandle stream_handle, const PlayCtrlCmd* cmd);

     // device control
    int DevicePTZControl(SessionHandle handle,const char* deviceID, const PtzCmd* cmd);
    int DeviceRecordControl(SessionHandle handle,const char* deviceID, bool isRecord);
    int DeviceGurdControl(SessionHandle handle,const char* deviceID,bool opt);
    int DeviceAlarmResetControl(SessionHandle handle,const  AlarmResetInfo* info,const char* deviceID);
    int DeviceZoomControl(SessionHandle handle,const char* deviceID,const ZoomCmd* cmd);
    int DeviceConfigSetControl(SessionHandle handle,const char* deviceID,const ConfigSetParam*cmd );
    int DeviceKeyFrameControl(SessionHandle handle,const char* deviceID);
    int DeviceHomePositionControl(SessionHandle handle,const char* deviceID,const HomePositionCmd* cmd);

    // subscribe
    int AlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info, SubscribeHandle* sub_handle);
    int CatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info,SubscribeHandle* sub_handle);
	int RefreshCatalogSubcribe(SessionHandle handle,const CatalogSubcribeInfo* info,SubscribeHandle sub_handle);
	int RefreshAlarmSubcribe(SessionHandle handle,const AlarmSubcribeInfo* info,SubscribeHandle sub_handle);
    int MobilePositionSubcribe(SessionHandle handle, const MobilePositionSubInfo* info,SubscribeHandle* sub_handle);
    int TerminateSubcribe(SessionHandle handle, SubscribeHandle sub_handle );
private:
    CGBServerImpl* m_server_Impl;
};

#endif // GB28181SERVERSDK_H
