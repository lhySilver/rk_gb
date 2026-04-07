#ifndef __GB28181_BROADCAST_BRIDGE_H__
#define __GB28181_BROADCAST_BRIDGE_H__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include <string>
#include <vector>

#include "ProtocolExternalConfig.h"

namespace protocol
{

class GB28181BroadcastBridge
{
public:
    typedef void (*PcmFrameCallback)(const uint8_t* pcm, size_t size, uint64_t timestamp, void* userData);

    GB28181BroadcastBridge();
    ~GB28181BroadcastBridge();

    int StartSession(const GbBroadcastParam& param);
    void StopSession();

    int OnAudioFile(const std::string& filePath);
    int OnAudioFrame(const uint8_t* data, size_t size, uint64_t timestamp);

    int ApplySdpOffer(const std::string& sdp, const std::string& remoteIpHint);
    int ApplyTransportHint(const std::string& remoteIp,
                           int remotePort,
                           int payloadType,
                           const std::string& codec,
                           int transportType);
    std::string BuildLocalAnswerSdp(const std::string& localIp) const;
    int GetNegotiatedTransport(std::string& remoteIp,
                               int& remotePort,
                               int& payloadType,
                               std::string& codec,
                               int& localRecvPort,
                               int& transportType) const;

    void SetPcmFrameCallback(PcmFrameCallback cb, void* userData);

    bool IsRunning() const;

private:
    struct ParsedPacket
    {
        const uint8_t* payload;
        size_t payload_size;
        uint8_t payload_type;
        uint64_t timestamp;
    };

    int EnsureAudioOutputStarted();
    void CloseRawAudioDump();
    void DumpRawAudioFrame(const uint8_t* data, size_t size);
    int DecodeToPcm16(const uint8_t* data, size_t size, std::vector<char>& pcmOut);

    int SetupRecvSocket();
    void CloseRecvSocket();
    void CloseAcceptedSocket();
    int StartRecvThread();
    void StopRecvThread();
    int RunRecvLoop();

    bool TryParseRtpPacket(const uint8_t* data, size_t size, ParsedPacket& out) const;

    static void* RecvThreadEntry(void* arg);

private:
    GbBroadcastParam m_param;
    bool m_running;
    bool m_audio_opened;

    int m_recv_sock;
    int m_listen_sock;
    pthread_t m_recv_thread;
    bool m_recv_thread_started;
    bool m_recv_loop;

    int m_negotiated_payload_type;
    int m_transport_type;
    std::string m_remote_ip;
    int m_remote_port;
    std::vector<uint8_t> m_tcp_recv_buffer;
    FILE* m_raw_dump_fp;
    std::string m_raw_dump_path;
    bool m_raw_dump_open_failed;

    PcmFrameCallback m_pcm_callback;
    void* m_pcm_callback_user;
};

}

#endif
