#ifndef __GB28181_CLIENT_RECEIVER_ADAPTER_H__
#define __GB28181_CLIENT_RECEIVER_ADAPTER_H__

#include <string>

#include "GB28181Defs.h"

namespace protocol
{

class ProtocolManager;

class GB28181ClientReceiverAdapter : public GBClientReceiver
{
public:
    explicit GB28181ClientReceiverAdapter(ProtocolManager* manager = NULL);
    virtual ~GB28181ClientReceiverAdapter();

    void BindManager(ProtocolManager* manager);

    virtual bool OnNotify(NotifyType type, const char* gb_code, void* info);
    virtual bool OnDeviceControl(ResponseHandle handle, const DevControlCmd* cmd);
    virtual bool OnSubscribe(SubscribeHandle handle, SubscribeType type, const char* gb_code, void* info);
    virtual bool OnQuery(ResponseHandle handle, const QueryParam* param);
    virtual bool OnStreamRequest(StreamHandle handle, const char* gb_code, StreamRequestType type, const MediaInfo* input);
    virtual bool OnStreamAck(StreamHandle handle);
    virtual bool OnPlayControl(StreamHandle handle, const PlayCtrlCmd* cmd);

private:
    static std::string SafeStr(const char* s, size_t maxLen);

private:
    ProtocolManager* m_manager;
};

}

#endif



