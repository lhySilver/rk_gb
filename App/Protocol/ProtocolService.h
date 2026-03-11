#ifndef __PROTOCOL_SERVICE_H__
#define __PROTOCOL_SERVICE_H__

#include <string>

namespace protocol
{

class ProtocolService
{
public:
    virtual ~ProtocolService() {}

    virtual int Init(const std::string& configEndpoint) = 0;
    virtual void UnInit() = 0;

    virtual int Start() = 0;
    virtual void Stop() = 0;

    virtual int ReloadExternalConfig() = 0;
};

}

#endif
