#ifndef RTPDEF_H
#define RTPDEF_H

#ifdef WIN32
   #ifdef RTP_EXPORTS
   #define EXPORT_RTP_SDK  __declspec(dllexport)
   #else
   #define EXPORT_RTP_SDK __declspec(dllimport)
   #endif
#else
   #define EXPORT_RTP_SDK  __attribute__ ((visibility("default")))
#endif
#include <stdint.h>
typedef void * SESSION_HANDLE;

enum RtpTranmissionMode
{
    kRTPOverUDP,
    kRTPOverTCPClient,
    kRTPOverTCPServer
};

enum PortType
{
   kAutoPort = 0,
   kManualPort
};

enum PayloadType
{
	 kPcma = 8,
     kPS = 96
};

enum RtpErrorCode
{
   kOk,
   kFail,
   kCreateSocketFail,
   kConnectFail,
   kListenFail,
   kBindIpFail,
   kNULLPointer,
   kInvaildParam,
   kInvaildPacket,
   kIncorrectPayloadType,
   kSessionNotFound,
   kSessionExist,
   kDisconnect,
   kConnectArrive,
   kLossPacket,
   kSendOverFlow,
   kRevOverFlow,
   kTCPClientAlreadyConnect,
   kTCPServerNotAllow,
};

struct RtpPort
{
       PortType type;
       uint16_t min_port;
       uint16_t max_port;
};


struct RtpSessionParam
{
    const char* bind_ip;
    int frequency;
    PayloadType pt;
    RtpTranmissionMode mode;
	uint32_t ssrc;

    RtpSessionParam(){
        bind_ip = "127.0.0.1";                            /* default value: 127.0.0.1*/
        frequency = 90000;                                 /* default value: 90000 for video*/
        pt = kPS;                                         /* default value: 96-> ps    */
        mode = kRTPOverUDP;                               /* default value: UDP         */
		ssrc = 0;
    }
};

enum FrameType
{
    kFrame_P,
    kFrame_I,
    kFrame_B,
    kFrame_AUDIO,
    kFrame_JEPG,
    kFrame_NULL=0xff,/*结束，无数据*/
};

enum MediaType
{
    kVideo,
    kAudio,
    kVideoAndAudio
};

struct RtpFrame
{

    uint8_t*           payload;
    uint32_t           payload_len;
    uint32_t           timestamp;     /*unix timestamp*/
    MediaType          media_type;
    FrameType          frame_flag;
    PayloadType        payload_type;
	uint32_t		  frameInterval;	//rtp时间戳帧间隔
    RtpFrame(){
        payload = 0;
        payload_len = 0;
		frameInterval = 0;
    }

};

class  MediaReciever
{
public:
    virtual int StreamInput( const RtpFrame* frame ) = 0;
};

class  RTPEventReciever
{
public:
    virtual void RTPEventNotify( RtpErrorCode code , const char* description ) = 0;
};


#endif // RTPDEF_H
