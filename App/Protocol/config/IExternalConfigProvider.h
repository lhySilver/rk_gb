#ifndef __I_EXTERNAL_CONFIG_PROVIDER_H__
#define __I_EXTERNAL_CONFIG_PROVIDER_H__

#include <string>
#include "ProtocolExternalConfig.h"

namespace protocol
{

class IExternalConfigProvider
{
public:
    virtual ~IExternalConfigProvider() {}

    virtual int PullLatest(ProtocolExternalConfig& out) = 0;
    virtual int PushApply(const ProtocolExternalConfig& cfg) = 0;
    virtual int Validate(const ProtocolExternalConfig& cfg) = 0;
    virtual int QueryCapabilities(std::string& outJson) = 0;
    virtual void SubscribeChange() = 0;
};

}

#endif
