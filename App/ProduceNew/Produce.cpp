#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <Types/Defs.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include <sys/prctl.h>

#include "Common.h"

#include "Produce.h"
#include "NetWifi.h"
#include "xop/RtspServer.h"

static xop::MediaSessionId session_id_main = 0;
static xop::RtspServer* rtsp_server = NULL;
static xop::MediaSessionId session_id_sub = 0;

static pthread_t g_rtspPthreadTid = 0;
static bool s_bRtspStart = false;
static bool s_rtspThreadQuit = true;

typedef struct
{
    uint32_t len;                     //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
    int32_t forbidden_bit;            //! should be always FALSE
    int32_t nal_reference_idc;        //! NALU_PRIORITY_xxxx
    int32_t nal_unit_type;            //! NALU_TYPE_xxxx
    char *buf;                        //! contains the first byte followed by the EBSP
} PD_NALU_H264_t;

typedef struct
{
    uint32_t len;                     //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
    int32_t forbidden_bit;            //! should be always FALSE
    int32_t naltype;                  //! NALTYPE
    int32_t layerid;                  //! LAYERID
	int32_t tid;                      //! TID
    char *buf;                        //! contains the first byte followed by the EBSP
} PD_NALU_H265_t;

static int32_t FindStartCode(const char *buf)
{
    if(buf[0] != 0x0 || buf[1] != 0x0 ){
        return 0;
    }

    if(buf[2] == 0x1){
        return 3;
    } else if(buf[2] == 0x0){
        if(buf[3] == 0x1){
            return 4;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 0;
}

static int32_t ReadOneNaluH265FromBuf(const char *buffer, uint32_t nBufferSize, uint32_t offSet, PD_NALU_H265_t* pNalu)
{
    uint32_t start = 0;
    if( offSet < nBufferSize) {
        start = FindStartCode(buffer + offSet);
        if(start != 0) {
            uint32_t pos = start;
            while (offSet + pos < nBufferSize) {
                if(buffer[offSet + pos++] == 0x00 &&
                    buffer[offSet + pos++] == 0x00 &&
                    buffer[offSet + pos++] == 0x00 &&
                    buffer[offSet + pos++] == 0x01
                    ) {
                    break;
                }
            }

            if(offSet + pos == nBufferSize){
                pNalu->len = pos - start;
            } else {
                pNalu->len = (pos - 4) - start;
            }

            pNalu->buf =(char*)&buffer[offSet + start];
            pNalu->forbidden_bit = pNalu->buf[0] & 0x80;
            pNalu->naltype = pNalu->buf[0] & 0x7e; // 6 bit


            return (pNalu->len + start);
        }
    }

    return 0;
}

static int32_t ReadOneNaluFromBuf(const char *buffer, uint32_t nBufferSize, uint32_t offSet, PD_NALU_H264_t* pNalu)
{
    uint32_t start = 0;
    if( offSet < nBufferSize) {
        start = FindStartCode(buffer + offSet);
        if(start != 0) {
            uint32_t pos = start;
            while (offSet + pos < nBufferSize) {
                if(buffer[offSet + pos++] == 0x00 &&
                    buffer[offSet + pos++] == 0x00 &&
                    buffer[offSet + pos++] == 0x00 &&
                    buffer[offSet + pos++] == 0x01
                    ) {
                    break;
                }
            }

            if(offSet + pos == nBufferSize){
                pNalu->len = pos - start;
            } else {
                pNalu->len = (pos - 4) - start;
            }

            pNalu->buf =(char*)&buffer[offSet + start];
            pNalu->forbidden_bit = pNalu->buf[0] & 0x80;
            pNalu->nal_reference_idc = pNalu->buf[0] & 0x60; // 2 bit
            pNalu->nal_unit_type = (pNalu->buf[0]) & 0x1f;// 5 bit

            return (pNalu->len + start);
        }
    }

    return 0;
}
static int SIGN_BIT = 0x80;
static int QUANT_MASK = 0xf;
static int SEG_SHIFT = 4;
static int SEG_MASK = 0x70;
static int BIAS = 0x84;
static int CLIP = 8159;

static short seg_end[] = {0xFF, 0x1FF, 0x3FF, 0x7FF,0xFFF, 0x1FFF, 0x3FFF, 0x7FFF};

static short search(short val,short table[],short size){

	for (short i = 0 ; i < size; i++) {
		if(val <= table[i]){
			return i;
		}
	}
	return size;
}

static unsigned char linear2alaw(short pcm_val){
	short mask;
	short seg;
	unsigned char aval;
	if(pcm_val >= 0){
		mask = 0xD5;
	}else{
		mask = 0x55;
		pcm_val = (short) (-pcm_val - 1);
		if(pcm_val < 0){
			pcm_val = 32767;
		}
	}

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_end, (short) 8);

	/* Combine the sign, segment, and quantization bits. */

	if (seg >= 8)       /* out of range, return maximum value. */
		return (unsigned char) (0x7F ^ mask);
	else {
		aval = (unsigned char) (seg << SEG_SHIFT);
		if (seg < 2)
			aval |= (pcm_val >> 4) & QUANT_MASK;
		else
			aval |= (pcm_val >> (seg + 3)) & QUANT_MASK;
		return (unsigned char) (aval ^ mask);
	}
}
static void *rtspThread(void *data)
{
	char ip[20] = {0};
	
	{
		NetGetLocalIp("wlan0",ip);//产测只用wifi
	}
	
	char url[100] = {0};
	snprintf(url,sizeof(url),"rtsp://%s:8554/stream_main",ip);

	char url_sub[100] = {0};
	snprintf(url_sub,sizeof(url_sub),"rtsp://%s:8554/stream_sub",ip);

	std::string rtsp_url = url;            //"rtsp://192.168.110.228:8554/main";
	std::string rtsp_url_sub = url_sub;    //"rtsp://192.168.110.228:8554/sub";

	std::shared_ptr<xop::EventLoop> event_loop(new xop::EventLoop());  
	std::shared_ptr<xop::RtspServer> server = xop::RtspServer::Create(event_loop.get());
	if (!server->Start(ip, 8554)) 
	{
		s_rtspThreadQuit = true;
		return NULL;
	}
	
// #ifdef AUTH_CONFIG
// 	server->SetAuthConfig("-_-", "admin", "12345678");
// #endif

	{
		xop::MediaSession *session = xop::MediaSession::CreateNew("main"); // url: rtsp://ip/live
		session->AddSource(xop::channel_0, xop::H265Source::CreateNew(15)); 
		session->AddSource(xop::channel_1, xop::G711ASource::CreateNew());
		//session->AddSource(xop::channel_1, xop::AACSource::CreateNew(8000,1));
		//session->StartMulticast(); /* 开启组播(ip,端口随机生成), 默认使用 RTP_OVER_UDP, RTP_OVER_RTSP */

		session->AddNotifyConnectedCallback([] (xop::MediaSessionId sessionId, std::string peer_ip, uint16_t peer_port){
			printf("RTSP client connect, ip=%s, port=%hu \n", peer_ip.c_str(), peer_port);
		});
	
		session->AddNotifyDisconnectedCallback([](xop::MediaSessionId sessionId, std::string peer_ip, uint16_t peer_port) {
			printf("RTSP client disconnect, ip=%s, port=%hu \n", peer_ip.c_str(), peer_port);
		});

		std::cout << "URL: " << rtsp_url << std::endl;
		
		//xop::MediaSessionId 
		session_id_main = server->AddSession(session);
	}

	{
		xop::MediaSession *session = xop::MediaSession::CreateNew("sub"); // url: rtsp://ip/live
		session->AddSource(xop::channel_0, xop::H265Source::CreateNew(15)); 
		session->AddSource(xop::channel_1, xop::G711ASource::CreateNew());
		//session->AddSource(xop::channel_1, xop::AACSource::CreateNew(8000,1));
		//session->StartMulticast(); /* 开启组播(ip,端口随机生成), 默认使用 RTP_OVER_UDP, RTP_OVER_RTSP */

		session->AddNotifyConnectedCallback([] (xop::MediaSessionId sessionId, std::string peer_ip, uint16_t peer_port){
			printf("RTSP client sub connect, ip=%s, port=%hu \n", peer_ip.c_str(), peer_port);
		});
	
		session->AddNotifyDisconnectedCallback([](xop::MediaSessionId sessionId, std::string peer_ip, uint16_t peer_port) {
			printf("RTSP client sub disconnect, ip=%s, port=%hu \n", peer_ip.c_str(), peer_port);
		});

		std::cout << "URL: " << rtsp_url_sub << std::endl;
		
		//xop::MediaSessionId 
		session_id_sub = server->AddSession(session); 
	}

	rtsp_server = server.get();

	s_rtspThreadQuit = false;
	while(s_bRtspStart)
	{
		xop::Timer::Sleep(1000);
	}
	s_rtspThreadQuit = true;

	return NULL;
}

void PD_StopRtspPthread(void)
{
	s_bRtspStart = false;
	if (g_rtspPthreadTid)
	{
		int nRet = pthread_join(g_rtspPthreadTid,NULL);
		if (nRet != 0) 
		{
			printf("PD_StopRtspPthread stop rtsp pthread error :%d!\n",errno);
		}
		else
		{
			printf("PD_StopRtspPthread stop rtsp pthread ok!\n");
		}
	}
	g_rtspPthreadTid = 0;
	s_rtspThreadQuit = true;
}

int PD_StartRtspPthread(void)
{
	int nRet;
	s_bRtspStart = true;
	
	if( (g_rtspPthreadTid == 0) && s_rtspThreadQuit)
	{
		nRet = pthread_create(&g_rtspPthreadTid, NULL,rtspThread, (void *)NULL);
		if (nRet != 0) 
		{
			printf("StartRtspPthread create rtsp pthread error :%d!\n",errno);
			return -1;
		}
	}
	else
	{
		printf("StartRtspPthread error!\n");
		return -1;
	}

	return 0;
}

int PD_RtspPutFrame_Main(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
{
	//获取一帧 H265, 打包 去掉startcode 00 00 00 01
	PD_NALU_H265_t nalu;
    int32_t pos = 0, len = 0;
    
    while (pos<iDataLen) 
	{
        len = ReadOneNaluH265FromBuf(pData, iDataLen, pos, &nalu);
		//printf("len(%d)\n", len);
		if (0==len)
		{
			break;
		}
		pos += len;
		//printf("pos(%d)\n", pos);
		//解析出nalu
		//printf("## nalu type(%d)\n", nalu.naltype);
        //printf("NALU size:%8d\n",nalu.len);

		xop::AVFrame videoFrame = {0};
		videoFrame.type = (1 == iFrmType) ? xop::VIDEO_FRAME_I : xop::VIDEO_FRAME_P; // 建议确定帧类型。I帧(xop::VIDEO_FRAME_I) P帧(xop::VIDEO_FRAME_P)
		videoFrame.size = nalu.len;  // 视频帧大小 
		videoFrame.timestamp = xop::H265Source::GetTimestamp(); // 时间戳, 建议使用编码器提供的时间戳
		videoFrame.buffer.reset(new uint8_t[videoFrame.size]);                    
		memcpy(videoFrame.buffer.get(), nalu.buf, videoFrame.size);					
		if (!s_rtspThreadQuit && rtsp_server)
		{
			rtsp_server->PushFrame(session_id_main, xop::channel_0, videoFrame); //送到服务器进行转发, 接口线程安全
		}
	}

	return 0;
}

int PD_RtspPutFrame_Sub(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
{
	//获取一帧 H265, 打包 去掉startcode 00 00 00 01
	PD_NALU_H265_t nalu;
    int32_t pos = 0, len = 0;
    
    while (pos<iDataLen) 
	{
        len = ReadOneNaluH265FromBuf(pData, iDataLen, pos, &nalu);
		//printf("len(%d)\n", len);
		if (0==len)
		{
			break;
		}
		pos += len;
		//printf("pos(%d)\n", pos);
		//解析出nalu
		//printf("## nalu type(%d)\n", nalu.nal_unit_type);
        //printf("NALU size:%8d\n",nalu.len);

		xop::AVFrame videoFrame = {0};
		videoFrame.type = (1 == iFrmType) ? xop::VIDEO_FRAME_I : xop::VIDEO_FRAME_P; // 建议确定帧类型。I帧(xop::VIDEO_FRAME_I) P帧(xop::VIDEO_FRAME_P)
		videoFrame.size = nalu.len;  // 视频帧大小 
		videoFrame.timestamp = xop::H265Source::GetTimestamp(); // 时间戳, 建议使用编码器提供的时间戳
		videoFrame.buffer.reset(new uint8_t[videoFrame.size]);                    
		memcpy(videoFrame.buffer.get(), nalu.buf, videoFrame.size);					
		if (!s_rtspThreadQuit && rtsp_server)
		{
			rtsp_server->PushFrame(session_id_sub, xop::channel_0, videoFrame); //送到服务器进行转发, 接口线程安全
		}
	}
	
	return 0;
}

int PD_RtspPutFrame_Audio(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
{
#if 0
	//获取一帧 AAC, 打包
	xop::AVFrame audioFrame = {0};
	audioFrame.type = xop::AUDIO_FRAME;
	audioFrame.size = iDataLen;  /* 音频帧大小 */
	audioFrame.timestamp = xop::AACSource::GetTimestamp(8000); // 时间戳
	audioFrame.buffer.reset(new uint8_t[audioFrame.size]);                    
	memcpy(audioFrame.buffer.get(), pData, audioFrame.size);
	if (!s_rtspThreadQuit && rtsp_server)
	{
		rtsp_server->PushFrame(session_id_main, xop::channel_1, audioFrame); // 送到服务器进行转发, 接口线程安全
	}
#endif

	int size = iDataLen/2;
	unsigned char buff[size];
	
	int j = 0;
	int i = 0;
	for (i = 0; i < iDataLen; )
	{
		short *p = (short *)(pData+i);
		if (!p)
		{
			AppErr("the pointer is null\n");
			return -1;
		}
		short val = *p;
		buff[j] = linear2alaw(val);
		j++;
		i+=2;
	}
	//获取一帧 G711A, 打包
	xop::AVFrame audioFrame = {0};
	audioFrame.type = xop::AUDIO_FRAME;
	audioFrame.size = size;  /* 音频帧大小 */
	audioFrame.timestamp = xop::G711ASource::GetTimestamp(); // 时间戳
	audioFrame.buffer.reset(new uint8_t[audioFrame.size]);                    
	memcpy(audioFrame.buffer.get(), buff, audioFrame.size);
	if (!s_rtspThreadQuit && rtsp_server)
	{
		rtsp_server->PushFrame(session_id_main, xop::channel_1, audioFrame); // 送到服务器进行转发, 接口线程安全
	}

	return 0;
}

int PD_RtspPutFrame_Audio1(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
{
#if 0
	//获取一帧 AAC, 打包
	xop::AVFrame audioFrame = {0};
	audioFrame.type = xop::AUDIO_FRAME;
	audioFrame.size = iDataLen;  /* 音频帧大小 */
	audioFrame.timestamp = xop::AACSource::GetTimestamp(8000); // 时间戳
	audioFrame.buffer.reset(new uint8_t[audioFrame.size]);                    
	memcpy(audioFrame.buffer.get(), pData, audioFrame.size);
	if (!s_rtspThreadQuit && rtsp_server)
	{
		rtsp_server->PushFrame(session_id_sub, xop::channel_1, audioFrame); // 送到服务器进行转发, 接口线程安全
	}
#endif

	int size = iDataLen/2;
	unsigned char buff[size];
	
	int j = 0;
	int i = 0;
	for (i = 0; i < iDataLen; )
	{
		short *p = (short *)(pData+i);
		if (!p)
		{
			AppErr("the pointer is null\n");
			return -1;
		}
		short val = *p;
		buff[j] = linear2alaw(val);
		j++;
		i+=2;
	}
	//获取一帧 G711A, 打包
	xop::AVFrame audioFrame = {0};
	audioFrame.type = xop::AUDIO_FRAME;
	audioFrame.size = size;  /* 音频帧大小 */
	audioFrame.timestamp = xop::G711ASource::GetTimestamp(); // 时间戳
	audioFrame.buffer.reset(new uint8_t[audioFrame.size]);                    
	memcpy(audioFrame.buffer.get(), buff, audioFrame.size);
	if (!s_rtspThreadQuit && rtsp_server)
	{
		rtsp_server->PushFrame(session_id_sub, xop::channel_1, audioFrame); // 送到服务器进行转发, 接口线程安全
	}

	return 0;
}

static int gProcessWifiRun = 1;
static int gProcessWifiExit = 0;

#define MAX_SEARCH_WIFI (50)
static CNetWifi::WIFI_LIST s_tpWifiList[MAX_SEARCH_WIFI];
static int wifi_num = 0;
static void *thread_search_wifi(void *arg)
{
	int num;
	CNetWifi::WIFI_LIST stWifi;
	CNetWifi::WIFI_LIST wifiList[MAX_SEARCH_WIFI];
	memset(s_tpWifiList, 0, sizeof(s_tpWifiList));

	while(gProcessWifiRun)
	{
		memset(wifiList, 0, sizeof(wifiList));
		
		num = g_NetWifi.GetWifiWifiList(wifiList, MAX_SEARCH_WIFI);
		printf("wifi num = %d\n", num);
		if( num > 0 )
		{
			//s_wifiListCount = num;
			//按信号强度降序排序
			for (int i = 0; i < num-1; i++)
			{
				for (int j = 0; j < num-i-1; j++)
				{
					if (wifiList[j].quality < wifiList[j+1].quality)
					{
						memcpy(&stWifi, &wifiList[j], sizeof(CNetWifi::WIFI_LIST));
						memcpy(&wifiList[j], &wifiList[j+1], sizeof(CNetWifi::WIFI_LIST));
						memcpy(&wifiList[j+1], &stWifi, sizeof(CNetWifi::WIFI_LIST));
					}
				}
			}
			wifi_num = num;
			memcpy(s_tpWifiList, wifiList, num*sizeof(CNetWifi::WIFI_LIST));
		}

		sleep(5);
	}
	gProcessWifiExit = 1;
	return NULL;
}

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}
static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    const std::string base64_chars = 
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while(i++ < 3)
            ret += '=';
    }

    return ret;

}

#if 01
static void base64_decode(std::string const& encoded_string, unsigned char output[256]) {
    const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
	int output_i = 0;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
				if (output_i < 256)
	                output[output_i++] = char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)	
			if (output_i < 256)
				output[output_i++] = char_array_3[j];
    }
}
#else
static std::string base64_decode(std::string const& encoded_string) {
    const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}
#endif
static std::string PackOsdString(int code,std::string msg, std::string osdKey,std::string osdData)
{
	Json::Value table;
	Json::Value jsonItem; //定义一个子对象
	
	table["code"] = code;
	table["msg"] = msg;
	jsonItem["osdKey"] = osdKey;
	jsonItem["osdData"] = osdData;
	table["data"] = jsonItem;
	
	std::string str = table.toStyledString();
	printf("resp=%s\n",str.c_str());
	return str;
}
static std::string PackOsdInt(int code,std::string msg, std::string osdKey,int osdData)
{
	Json::Value table;
	Json::Value jsonItem; //定义一个子对象
	
	table["code"] = code;
	table["msg"] = msg;
	jsonItem["osdKey"] = osdKey;
	jsonItem["osdData"] = osdData;
	table["data"] = jsonItem;
	
	std::string str = table.toStyledString();
	printf("resp=%s\n",str.c_str());
	return str;
}
static std::string PackCmdString(int status,std::string msg)
{
	Json::Value resptable;
	resptable["msg"] = msg;//Json::StaticString((const char*)mfg.c_str());
	resptable["status"] = status;//0-失败 1-成功
	std::string respstr = resptable.toStyledString();
	printf("resp=%s\n",respstr.c_str());
	return respstr;
}
static std::string PackCmdWithData(int status,std::string msg,int data)
{
	Json::Value resptable;
	Json::Value jsonItem; //定义一个子对象
	resptable["msg"] = msg;//Json::StaticString((const char*)mfg.c_str());
	resptable["status"] = status;//0-失败 1-成功
	jsonItem["value"] = data;
	resptable["data"] = jsonItem;
	std::string respstr = resptable.toStyledString();
	printf("resp=%s\n",respstr.c_str());
	return respstr;
}

static std::string pull(std::string req)
{
	printf("\npull file:%s\n",req.c_str());

	Json::Reader reader;  
	Json::Value root;
	std::string poststring;

	// {
	// 	code:0,
	// 	msg:"获取成功",
	// }
	Json::Value table;
	int len = 0;
	int len_bk = 0;

	table["code"] = "1";
	table["msg"] = "operation failed";
	goto end;

	#if 1
	if (reader.parse((const char*)req.c_str(), root))	// reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		poststring = root["data"].asString();
		if ((0==poststring.compare("")))
		{
			table["code"] = "1";
			table["msg"] = "Content is empty";
			goto end;
		}
	}
	else
	{
		table["code"] = "1";
        table["msg"] = "operation failed";
		goto end;
	}
	#else
	poststring = req;
	#endif


	//todo 写文件
	delete_file(DEV_CONFIG_FILE);
	delete_file(DEV_CONFIG_FILE_BK);

	len = dg_save_file(DEV_CONFIG_FILE, (unsigned char*)poststring.c_str(), poststring.length());
	len_bk = dg_save_file(DEV_CONFIG_FILE_BK, (unsigned char*)poststring.c_str(), poststring.length());
	if ( (len < 0) || (len_bk <0) )
	{
		table["code"] = "1";
		table["msg"] = "operation failed";
	}
	else
	{
		table["code"] = "0";
		table["msg"] = "success";
	}
end:
	std::string str = table.toStyledString();
	printf("resp=%s\n",str.c_str());
	return str;
}

static std::string burnLicense(std::string req)
{
	printf("\nburnLicense:%s\n",req.c_str());
    
	Json::Reader reader;  
	Json::Value root;
	std::string poststring;
	std::string decoded;
	Json::Value table;
	int rev = 0;


	if (reader.parse((const char*)req.c_str(), root))	// reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		poststring = root["IUDID"].asString();
	}
	else
	{
		table["code"] = 400;
        table["msg"] = "operation failed";
		goto blexit;
	}

	#if 1
	SMsgAVIoctrlBurnLicenseReq BurnLicenseReq;
	BurnLicenseReq.force = 0;
	base64_decode(poststring, BurnLicenseReq.license);
	rev = g_EncryptionHandle.BurnLicenseReqProc(&BurnLicenseReq);
	#else
	decoded = base64_decode(poststring);

	SMsgAVIoctrlBurnLicenseReq BurnLicenseReq;
	BurnLicenseReq.force = 0;
	memcpy(BurnLicenseReq.license,decoded.c_str(),sizeof(BurnLicenseReq.license));
	rev = g_EncryptionHandle.BurnLicenseReqProc(&BurnLicenseReq);
	#endif

	if ( 0 == rev ) //烧录成功
	{
		table["code"] = 0;
		table["msg"] = "success";
	}
	else if (10 == rev) //烧录出错
	{
		table["code"] = 1;
		table["msg"] = "operation failed";
	}
	else if (100 == rev) //设备已经有号
	{
		table["code"] = 0;//2;
		table["msg"] = "success";//"operation failed";
	}
	else if (110 == rev) //license与设备版本不匹配
	{
		table["code"] = 3;
		table["msg"] = "operation failed";
	}
	else
	{
		table["code"] = 400;
		table["msg"] = "operation failed";
	}
blexit:
	std::string str = table.toStyledString();
	printf("burnLicense=%s\n",str.c_str());
	return str;
}
static std::string getWifiListInfo(std::string req)
{
	printf("\n======================get wifi info====================\n");
	

	/*{
		"code":0,
		"msg":"success",
		"data":[{"ssid":"xxxx","signal":80},{"ssid":"xxxx","signal":80}]
	}*/


	Json::Value table;
	std::string resq;
	if (wifi_num > 0 )
	{
		table["code"] = 0;
		table["msg"] = "success";
		
		char str[10] = {0};
		for (int i = 0; i < wifi_num; i++)
		{
			Json::Value jsonItem; //定义一个子对象
			jsonItem["ssid"] = s_tpWifiList[i].essid;
			jsonItem["signal"] = s_tpWifiList[i].quality;
			table["data"].append(jsonItem);
		}
	}
	
	resq = table.toStyledString();

	return resq;
}
static std::string info(std::string req)
{
	printf("\n==========================info:%s====================\n",req.c_str());
	

	// {
	// "osdKey": "tfcardUse"
	// }

	// {
	// "code":0,
	// "msg":"success",
	// "data": {
	// 	"osdKey": "tfcardUse",
	// 	"osdData": 16,
	// }
	// }

	Json::Reader reader;  
	Json::Value root;
	std::string osdKey;
	std::string resq;

	if (!reader.parse((const char*)req.c_str(), root))	// reader将Json字符串解析到root，root将包含Json里所有子元素  
		resq = PackOsdString(1,"operation failed","","");
	else
		osdKey = root["osdKey"].asString();

	printf("osdKey=%s\n",osdKey.c_str());

	if ((0==osdKey.compare("ProductType")))
	{
#ifdef RC0240
		resq = PackOsdString(0,"success", osdKey,"RC0240");
#endif
#ifdef RC0240V20
		resq = PackOsdString(0,"success", osdKey,"RC0240V20");
#endif
#ifdef RC0240V30
		resq = PackOsdString(0,"success", osdKey,"RC0240V30");
#endif
#ifdef RC0240V40
		resq = PackOsdString(0,"success", osdKey,"RC0240V40");
#endif
#ifdef RC0240_LGV10
		resq = PackOsdString(0,"success", osdKey,"RC0240_LGV10");
#endif
#ifdef RC0330_V20
		resq = PackOsdString(0,"success", osdKey,"RC0330");
#endif
	}
	else if ((0==osdKey.compare("CloudPlatform")))
	{
		resq = PackOsdString(0,"success", osdKey,"TUYA");
	}
	else if ((0==osdKey.compare("Version")))
	{
		resq = PackOsdString(0,"success", osdKey,IPC_APP_VERSION);
	}
	else if ((0==osdKey.compare("MAC")))
	{
		char mac[6] = {0};
		char smac[30] = {0};
		
		NetGetMac_2("wlan0", (unsigned char*)mac);
		snprintf(smac, sizeof(smac), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		std::string strmac((char*)smac);
		resq = PackOsdString(0,"success", osdKey,strmac);
	}
	else if ((0==osdKey.compare("PID")))
	{
		if(g_EncryptionHandle.GetLicenseSavaType() != 0)
		{
			std::string pid((char*)DevInfoFromEEPROM_g.TUYA_PID);
			resq = PackOsdString(0,"success", osdKey,pid);
		}
		else
		{
			std::string pid((char*)DevInfoFromEEPROM_g.TUYA_PID);
			resq = PackOsdString(1,"operation failed", osdKey,pid);
		}
		
	}
	else if ((0==osdKey.compare("UID")))
	{
		if(g_EncryptionHandle.GetLicenseSavaType() != 0)
		{
			std::string uid((char*)DevInfoFromEEPROM_g.TUYA_UID);
			resq = PackOsdString(0,"success", osdKey,uid);
		}
		else
		{
			std::string uid((char*)DevInfoFromEEPROM_g.TUYA_UID);
			resq = PackOsdString(1,"operation failed", osdKey,uid);
		}
		
	}
	else if (	0==osdKey.compare("tfcardTotal") ||
				0==osdKey.compare("tfcardUse")   ||
				0==osdKey.compare("tfcardFree")	  )
	{
		int iDiskStatus = DISK_STATUS_UNKNOWN;
		unsigned int total = 0;
		unsigned int free = 0;
		iDiskStatus = g_StorageManager->GetDiskState();
		
		//未检测到SD 卡
		if ( (DISK_STATUS_UNKNOWN == iDiskStatus) || (DISK_STATUS_NO_DISK == iDiskStatus) )
		{
			total = 0;
			free = 0;
		}
		//有卡
		else
		{
			unsigned long long ullDiskSize = 0;
			unsigned long long ullDiskFreeSize = 0;
			g_StorageManager->GetDiskcapacity(&ullDiskSize, NULL, &ullDiskFreeSize);
			ullDiskSize /= 1024*1024;
			ullDiskFreeSize /= 1024*1024;
			
			AppInfo("ullDiskTotalSize=%llu\n", ullDiskSize);
			AppInfo("ullDiskFreeSize=%llu\n", ullDiskFreeSize);

			total = ullDiskSize;
			free = ullDiskFreeSize;
		}
		if (0==osdKey.compare("tfcardTotal"))
		{
			resq = PackOsdInt(0,"success", osdKey,total);
		}
		else if (0==osdKey.compare("tfcardUse"))
		{
			resq = PackOsdInt(0,"success", osdKey,total-free);
		}
		else if (0==osdKey.compare("tfcardFree"))
		{
			resq = PackOsdInt(0,"success", osdKey,free);
		}
	}
	else if (0==osdKey.compare("Wifi"))
	{
		router_signal_s stWifiSignal = {0};
		int ret = WifiSignal(&stWifiSignal);
		AppInfo("ret = %d\n", ret);
		AppWarning("signal = %d\n", stWifiSignal.uchSignal);
		int iTmp = stWifiSignal.uchSignal;
		iTmp = iTmp * 100 / 100;
		resq = PackOsdInt(0,"success", osdKey,iTmp);
	}
	else if (0==osdKey.compare("Eth0"))
	{
		int status = g_NetWifi.GetEthStatus();
		resq = PackOsdInt(0,"success", osdKey,status);
	}
	else
	{
		resq = PackOsdString(1,"operation failed", osdKey,"");
	}

	return resq;
}
static std::string check(std::string req)
{
	printf("\n==========================check====================\n");
	
	// {
	// "code":0,
	// "msg":"success",
	// }

	Json::Value table;
	
	table["code"] = 0;
	table["msg"] = "success";

	std::string str = table.toStyledString();
	printf("resp=%s\n",str.c_str());
	return str;
}
static std::string getlicense(std::string req)
{
	printf("\n==========================getlicense====================\n");

	Json::Value table;
	Json::Value jsonItem; //定义一个子对象
	std::string strid;
	if(g_EncryptionHandle.GetLicenseSavaType() != 0)
	{
		table["code"] = 0;
		table["msg"] = "success";
		table["platform"] = "tuya";
		strid = ((char*)DevInfoFromEEPROM_g.TUYA_PID);
		jsonItem["pid"] = strid;
		strid = ((char*)DevInfoFromEEPROM_g.TUYA_UID);
		jsonItem["uuid"] = strid;
		strid = ((char*)DevInfoFromEEPROM_g.TUYA_AUTHKEY);
		jsonItem["key"] = strid;
		
		table["data"] = jsonItem;
	}
	else
	{
		table["code"] = 1;
		table["msg"] = "success";
		table["platform"] = "tuya";
		jsonItem["pid"] = "";
		jsonItem["uuid"] = "";
		jsonItem["key"] = "";
		table["data"] = jsonItem;
	}
	
	std::string str = table.toStyledString();
	printf("resp=%s\n",str.c_str());
	return str;
}
static std::string getmethod(std::string req)
{
	std::string resq = "ok";

	if (req.find("ispmode=day") != -1)
	{
		printf("\n==========================ispmode day====================\n");
		g_Camera.setMode(CAMERA_MODE_DAY);
	}
	else if (req.find("ispmode=night") != -1)
	{
		printf("\n==========================ispmode night==================\n");
		g_Camera.setMode(CAMERA_MODE_NIGHT);
	}
#if 0
	if (req.find("cal=0") != -1)
	{
		printf("\n==========================cal=0====================\n");
		system("touch " CAL_TEST);

		system("rm -rf " AWB_TEST);
		system("rm -rf " PRO_TEST);
		#if 1
		system("reboot");
		#else
		system("killall sample_demo_vi_avs_venc");
		system("killall rkaiq_tool_server");

		char cmd[256] = {"\0"};
		snprintf(cmd,256,"sample_demo_vi_avs_venc --vi_size 1920x1080 --avs_chn0_size 3840x1080 --avs_chn1_size 1920x544 -a /oem/usr/share/iqfiles/ -e h264cbr -b 4096 -n 2 --stitch_distance %f -F 15 --avs_mode_blend 2 --set_ldch -1 &",ProductCof_g.stitch_distance);
		printf("cal test=%s\n",cmd);
		system("ifconfig lo up");
		system(cmd);
		system("rkaiq_tool_server &");
		#endif
	}
	else if (req.find("cal=1") != -1)
	{
		printf("\n==========================cal=1==================\n");
		system("touch " AWB_TEST);

		system("rm -rf " CAL_TEST);
		system("rm -rf " PRO_TEST);
		#if 1
		system("reboot");
		#else
		system("killall sample_demo_vi_avs_venc");
		system("killall rkaiq_tool_server");

		system("ifconfig lo up");
		system("rkaiq_tool_server &");
		#endif
	}
	else if (req.find("cal=2") != -1)
	{
		printf("\n==========================cal=2==================\n");
		system("touch " PRO_TEST);

		system("rm -rf " CAL_TEST);
		system("rm -rf " AWB_TEST);
		system("reboot");
	}
#endif
	return resq;
}

static std::string cmd(std::string req)
{
	printf("\n==========================cmd:%s====================\n",req.c_str());
	
	Json::Reader reader;
	Json::Value root;
	Json::Value root_sub; 
	Json::Value root_sub1; 
	std::string resq;
	int type = 0;

	if (!reader.parse((const char*)req.c_str(), root))	// reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		resq = PackCmdString(0,"operation failed");
	}
	else
	{
		// int seqNo = root["seqNo"].asInt();
		// printf("seqNo=%d\n",seqNo);
		root_sub = root["params"];
		type = root_sub["type"].asInt();
		printf("type=%d\n",type);
	}

	switch(type)
	{
		case CProduce::CMD_SPEAKER: //调解喇叭音量 数字：0-100
		{
			std::string svalue =root_sub["value"].asString();
			int value = atoi(svalue.c_str());
			printf("value=%d\n",value);

			CConfigTable table;
			SirenConfig SirenCfg;
			
			g_configManager.getConfig(getConfigName(CFG_SIREN), table);
			TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
			printf("get sirenvolume:%d \r\n", SirenCfg.sirenvolume);
			SirenCfg.sirenvolume = value;
			TExchangeAL<SirenConfig>::setConfig(SirenCfg, table);
			g_configManager.setConfig(getConfigName(CFG_SIREN), table, 0, IConfigManager::applyOK);
			AudioSetVolume(0,0,value);

			//语音提示
			CAudioPrompt::AudioFileParm audioFile;
			audioFile.strFileName = AUDIO_FILE_DINGDINGDING;
			audioFile.type = 0;
			g_AudioPrompt.aoPlay(audioFile);

			resq = PackCmdString(0,"success");
		}
		break;
		case CProduce::CMD_MIC: 
		{
			resq = PackCmdString(0,"success");
		}
		break;
		case CProduce::CMD_DAY_NIGHT: 
		{
			resq = PackCmdString(0,"success");
		}
		break;

		case CProduce::CMD_RESET_IPC:
		{
			printf("\033[1;36m Produce test require reboot ---> reset ipc success.  \033[0m\n");
			printf("Produce test require reboot ---> reset ipc success.\n");
			
			SystemReset();
		}
		break;

		case CProduce::CMD_INDICATOR_LAMP://指示灯 数字： 0/1  0：开启?0?2 1： 关闭
		{
			std::string svalue =root_sub["value"].asString();
			int value = atoi(svalue.c_str());
			printf("value=%d\n",value);
			if (0==value)
			{

				g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_OFF);
				g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_OFF);
			}
			else
			{

				g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_POWER_INDICATOR_LIGHT_ALWAYS_ON);
				g_IndicatorLight.setLightStatus(CIndicatorLight::ENUM_LINK_INDICATOR_LIGHT_ALWAYS_ON);
			}
			resq = PackCmdString(0,"success");
		}
		break;
		case CProduce::CMD_RESET_BUTTON:
		{
			int val = -1;
			val = SystemRest();
			if(val == 0) //key down
			{
				resq = PackCmdWithData(0,"success",1);
			}
			else
			{
				resq = PackCmdWithData(0,"success",0);
			}
		}
		break;
#ifdef RC0240_LGV10
		case CProduce::CMD_PIR1:
		{
			int value = -1;
			value = SystemPirDet();
			// AppErr("SystemPirDet=%d\n",value);
			if (value == -1)
			{
				resq = PackCmdWithData(0,"success",0);
			}
			else if (value & 0x01)//pir 1
			{
				printf("---------------pir1---------------\n");
				resq = PackCmdWithData(0,"success",1);
			}
			else 
			{
				resq = PackCmdWithData(0,"success",0);
			}
		}
		break;
		case CProduce::CMD_PIR2:
		{
			int value = -1;
			value = SystemPirDet();
			// AppErr("SystemPirDet=%d\n",value);
			if (value == -1)
			{
				resq = PackCmdWithData(0,"success",0);
			}
			else if (value & 0x02)//pir 2
			{
				printf("---------------pir2---------------\n");
				resq = PackCmdWithData(0,"success",1);
			}
			else 
			{
				resq = PackCmdWithData(0,"success",0);
			}
		}
		break;
		case CProduce::CMD_PIR3:
		{
			int value = -1;
			value = SystemPirDet();
			// AppErr("SystemPirDet=%d\n",value);
			if (value == -1)
			{
				resq = PackCmdWithData(0,"success",0);
			}
			else if (value & 0x04)//pir 3
			{
				printf("---------------pir3---------------\n");
				resq = PackCmdWithData(0,"success",1);
			}
			else 
			{
				resq = PackCmdWithData(0,"success",0);
			}
		}
		break;
#endif
		case CProduce::CMD_PTZ_UP:
		{
			std::string svalue =root_sub["value"].asString();
			int value = atoi(svalue.c_str());
			printf("value=%d\n",value);
			resq = PackCmdString(0,"success");

			// PTZDirection_e dir = PTZ_UP;
			// g_PtzHandle.DoMotorStop();		
			// g_PtzHandle.DoMotorMoveTest(dir,value);
		}
		break;
		case CProduce::CMD_PTZ_DOWN:
		{
			std::string svalue =root_sub["value"].asString();
			int value = atoi(svalue.c_str());
			printf("value=%d\n",value);
			resq = PackCmdString(0,"success");

			// PTZDirection_e dir = PTZ_DOWN;
			// g_PtzHandle.DoMotorStop();		
			// g_PtzHandle.DoMotorMoveTest(dir,value);
		}
		break;
		case CProduce::CMD_PTZ_LEFT:
		{
			std::string svalue =root_sub["value"].asString();
			int value = atoi(svalue.c_str());
			printf("value=%d\n",value);
			resq = PackCmdString(0,"success");

			// PTZDirection_e dir = PTZ_LEFT;
			// g_PtzHandle.DoMotorStop();		
			// g_PtzHandle.DoMotorMoveTest(dir,value);
		}
		break;
		case CProduce::CMD_PTZ_RIGHT:
		{
			std::string svalue =root_sub["value"].asString();
			int value = atoi(svalue.c_str());
			printf("value=%d\n",value);
			resq = PackCmdString(0,"success");

			// PTZDirection_e dir = PTZ_RIGHT;
			// g_PtzHandle.DoMotorStop();		
			// g_PtzHandle.DoMotorMoveTest(dir,value);
		}
		break;
		case CProduce::HAND_CMD_PTZ:
		{
			root_sub1 = root_sub["value"];
			std::string direction =root_sub1["direction"].asString();
			std::string status =root_sub1["status"].asString();
			printf("direction=%s\n",direction.c_str());
			printf("status=%s\n",status.c_str());
			//direction Up/Down/Left/Right/Stop/Auto
			//status start/stop
			if (!direction.compare("up"))
			{
				if (!status.compare("start"))
				{
					// PTZDirection_e dir = PTZ_UP;
					// g_PtzHandle.DoMotorStop();		
					// g_PtzHandle.DoMotorMove(dir);
				}
				else
				{
					// g_PtzHandle.DoMotorStop();
				}
			}
			else if (!direction.compare("down"))
			{
				if (!status.compare("start"))
				{
					// PTZDirection_e dir = PTZ_DOWN;
					// g_PtzHandle.DoMotorStop();		
					// g_PtzHandle.DoMotorMove(dir);
				}
				else
				{
					// g_PtzHandle.DoMotorStop();
				}
			}
			else if (!direction.compare("left"))
			{
				if (!status.compare("start"))
				{
					// PTZDirection_e dir = PTZ_LEFT;
					// g_PtzHandle.DoMotorStop();		
					// g_PtzHandle.DoMotorMove(dir);
				}
				else
				{
					// g_PtzHandle.DoMotorStop();
				}
			}
			else if (!direction.compare("right"))
			{
				if (!status.compare("start"))
				{
					// PTZDirection_e dir = PTZ_RIGHT;
					// g_PtzHandle.DoMotorStop();		
					// g_PtzHandle.DoMotorMove(dir);
				}
				else
				{
					// g_PtzHandle.DoMotorStop();
				}
			}
			else if (!direction.compare("auto"))
			{
				if (!status.compare("start"))
				{
					// g_PtzHandle.DoMotorStop();
					// g_PtzHandle.DoAutoTest();
				}
			}
			
			resq = PackCmdString(0,"success");

		}
		break;
		case CProduce::CMD_WHITE_LAMP://白光灯 数字： 0/1  0：开启?0?2 1： 关闭
		{
			std::string svalue =root_sub["value"].asString();
			int value = atoi(svalue.c_str());
			printf("value=%d\n",value);
			if (0==value)
			{
				//关白关灯
				g_Camera.SetWhiteLed(false);
			}
			else
			{
				//开白光灯
				g_Camera.SetWhiteLed(true);
			}
			resq = PackCmdString(0,"success");
		}
		break;
		default:
		{
			resq = PackCmdString(1,"operation failed");
		}
		break;
	}

	return resq;
}
static int onCapture_fn(int media_chn,
        int media_type,
        int media_subtype,
        unsigned long long frame_pts,
        unsigned char *frame_data,
        int frame_len,
        int frame_end_flag)
{
	int ret = 0;
	unsigned char *pFrmBuff = frame_data;
	unsigned int uiFrmSize = (unsigned int)frame_len;
	unsigned long long ullTimeStamp = frame_pts;

	if (DMC_MEDIA_VIDEO_MAIN_STREAM == media_chn)
	{
		{
			PD_RtspPutFrame_Main(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			// printf("----------------------------->onCapture_fn, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
		}
	}
	else if  (DMC_MEDIA_VIDEO_SUB_STREAM == media_chn)
	{
		{
			PD_RtspPutFrame_Sub(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			// printf("----------------------------->onCapture_fn, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
		}
	}
	else if (DMC_MEDIA_AUDIO_FRIST_STREAM == media_chn)
	{
		{
			PD_RtspPutFrame_Audio(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			PD_RtspPutFrame_Audio1(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			// printf("----------------------------->onCapture_fn, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
		}
	}

	return 0;
}

#if 0
//音频采集回调
static void onAudioCapture(int iChannel, uint dwStreamType, CPacket *packet)
{
	PKT_HEAD_INFO *pkthead = (PKT_HEAD_INFO *)(packet->GetHeader()); //注意由于下面内存已对齐
	CAP_AV_HEADER *ph = (CAP_AV_HEADER *)(packet->GetBuffer());
	
    unsigned char *pFrmBuff = packet->GetBuffer()+sizeof(CAP_AV_HEADER);
	unsigned int uiFrmSize = packet->GetLength()-sizeof(CAP_AV_HEADER);

	if( 1 == ph->uiType) 		//g711u
	{

	}
	if ( 0 == ph->uiType)      //AAC
	{
		PD_RtspPutFrame_Audio(pkthead->FrameInfo[0].FrameType, &ph->ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
		PD_RtspPutFrame_Audio1(pkthead->FrameInfo[0].FrameType, &ph->ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
	}
}

static void onCapture(int iChannel, uint dwStreamType, CPacket *packet)
{
	PKT_HEAD_INFO *pkthead = (PKT_HEAD_INFO *)(packet->GetHeader()); //注意由于下面内存已对齐

	if( pkthead->FrameInfo[0].FrameType !=3 ) 		//视频数据
	{
		CAP_AV_HEADER *ph = (CAP_AV_HEADER *)(packet->GetBuffer());
		unsigned char *pFrmBuff = packet->GetBuffer()+sizeof(CAP_AV_HEADER);
		unsigned int uiFrmSize = packet->GetLength()-sizeof(CAP_AV_HEADER);

		PD_RtspPutFrame_Main(pkthead->FrameInfo[0].FrameType, &ph->ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
		//printf("----------------------------->onCapture, size = %d, ret = %d\n", packet->GetLength()-sizeof(CAP_AV_HEADER), ret);	
	}
}

static void onCapture_2(int iChannel, uint dwStreamType, CPacket *packet)
{
	PKT_HEAD_INFO *pkthead = (PKT_HEAD_INFO *)(packet->GetHeader()); //注意由于下面内存已对齐

	if( pkthead->FrameInfo[0].FrameType !=3 ) 		//视频数据
	{
		
		CAP_AV_HEADER *ph = (CAP_AV_HEADER *)(packet->GetBuffer());
		unsigned char *pFrmBuff = packet->GetBuffer()+sizeof(CAP_AV_HEADER);
		unsigned int uiFrmSize = packet->GetLength()-sizeof(CAP_AV_HEADER);

		PD_RtspPutFrame_Sub(pkthead->FrameInfo[0].FrameType, &ph->ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
		//printf("----------------------------->onCapture_2,pts=%lld size = %d, ret = %d\n",ph->ullTimeStamp, packet->GetLength()-sizeof(CAP_AV_HEADER), ret);
	}
}
#endif

PATTERN_SINGLETON_IMPLEMENT(CProduce);

CProduce::CProduce() : CThread("Produce", 50)
{
	IEventManager::instance()->attach(IEventManager::Proc(&CProduce::onAppEvent, this));
}

CProduce::~CProduce()
{
}

void CProduce::ThreadProc()
{
#ifdef RC0240_LGV10
	int bPirInited = 0;
#endif

	while(m_bLoop)
	{
#ifdef RC0240_LGV10
		if (!bPirInited)
		{
			//pir
			int ret = -1;
			//AS006A PIR 上电后OUT先输出高电平，热机15秒后，输出低电平，进入检测状态
			ret = SystemPirInit();
			if (ret != -1)
			{
				bPirInited = 1;
				SystemSetPirSen(2,0);
			}
		}
#endif

		sleep(1);
	}
}

int CProduce::init()
{
	// printf("set ulimit -n 4096 \n");
	// TUYA_START_PROCESS("sh", "sh", "-c", "ulimit -n 8192", NULL);
	// TUYA_START_PROCESS("sh", "sh", "-c", "echo  6553560 > /proc/sys/fs/file-max", NULL);
	return 0;
}
///开启标定
bool CProduce::startcal()
{
	g_HttpServer.Init();

	opt.port = 80;
	opt.iocp = 1;
	opt.verbose = 0;
	opt.unlink = 1;

	strcpy(opt.get_req_path,"/get");
	opt.get_req = getmethod;

	opt.cmd = NULL;
	opt.info = NULL;
	opt.check = NULL;
	opt.getlicense = NULL;
	opt.wifi_info = NULL;
	opt.push_file = NULL;
	opt.pull_file = NULL;
	opt.burn_license = NULL;
	opt.unixsock = NULL;

	g_HttpServer.Start(&opt);

	return true;
}
bool CProduce::stopcal()
{
	g_HttpServer.Stop();
	return true;
}
bool CProduce::start()
{
	//启动音视频
	g_AVManager.RealTimeStreamStart(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO, onCapture_fn);
	//摄像头参数控制模块
	g_Camera.start();
	
	CreateDetachedThread((char*)"thread_search_wifi",thread_search_wifi, (void *)NULL, true);

	PD_StartRtspPthread();
	g_HttpServer.Init();

	opt.port = 80;
	opt.iocp = 1;
	opt.verbose = 0;
	opt.unlink = 1;

	strcpy(opt.cmd_req_path,"/cmd");
	strcpy(opt.info_req_path,"/osd");
	strcpy(opt.check_req_path,"/check");
	strcpy(opt.getlicense_req_path,"/getLicense");
	strcpy(opt.wifi_info_req_path,"/getWifiListInfo");
	strcpy(opt.push_req_path,"/downLoadProductFile");
	strcpy(opt.pull_req_path,"/pushConfig");
	strcpy(opt.burn_license_req_path,"/burnLicense");
	strcpy(opt.push_file_name,DEV_CONFIG_FILE);

	strcpy(opt.get_req_path,"/get");
	opt.get_req = getmethod;

	opt.cmd = cmd;
	opt.info = info;
	opt.check = check;
	opt.getlicense = getlicense;
	opt.wifi_info = getWifiListInfo;
	opt.push_file = NULL;
	opt.pull_file = pull;
	opt.burn_license = burnLicense;
	opt.unixsock = NULL;

	g_HttpServer.Start(&opt);

	return CreateThread();
}

bool CProduce::stop()
{
	gProcessWifiRun = 0;
	while(gProcessWifiExit!=1)
	{
		usleep(500);
	}

	PD_StopRtspPthread();

	g_HttpServer.Stop();

	return DestroyThread();
}

void CProduce::onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value *data)
{

}

