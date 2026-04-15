/*
 * Copyright (c) 2023 Fancy Code.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ILog.h"
#include "ITestModuleV2.h"
#include "TestMonitor.h"
#include <thread>

#include "Common.h"
#include "NetWifi.h"
#include "xop/RtspServer.h"
#include "PAL/Audio_coder.h"
#include "../../Middleware/Include/PAL/Misc.h"


//add on 2024.12.28 音频录制
extern void DoProductAudioRecord(const unsigned char *pData, unsigned int size);
//add on 2024.12.28 音频录制

int g_iKeyboardReport = 0; //add on 2025.01.02 按键上报
extern bool g_bDevInfoChanged;//add on 2025.01.02 设备信息同步
extern void GetDevInfo(DeviceInfo *info);//add on 2025.01.02 设备信息同步
char g_rtsp_url[100] = {0};
char g_rtsp_url_sub[100] = {0};

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

//add on 2024.12.30 PIR检测 start
static int bPirInited = 0;
static void *thread_report(void *arg)
{
	int eth0_status_current = 0;	//当前网口连接状态 add on 2025.02.06
	int eth0_status_last = 0;			//网口连接状态 add on 2025.02.06
	int current_time = 0;
	int iPirResultValidTime = 0; 	//时间大于该值，PIR检测结果才有效
	int pir_det_result = 0;		//PIR检测结果
	int iPirTriggerTime = 0; 		//PIR触发时间点，触发后保持1秒触发状态，防止其他线程获取状态时有遗漏
	// int last_pir_det_result = 0;	//上次PIR检测结果
	//此处参考Fuhan代码，具体以实际情况为准
	int PIR1_SWITCH_MASK = 0x2;
	int PIR2_SWITCH_MASK = 0x1;
	int PIR3_SWITCH_MASK = 0x4;
	while(1)
	{
// #ifdef RC0240_LGV10

		eth0_status_current = g_NetWifi.GetEthStatus();//获取网口状态add on 2025.02.06
		
		if(g_bDevInfoChanged || (eth0_status_current != eth0_status_last))//add on 2025.01.02 设备信息同步
		{
			eth0_status_last = eth0_status_current;//同步网口状态 //add on 2025.02.06
			printf("\033[1;36m    eth0_status_last = %d   \033[0m\n",eth0_status_current);
			g_bDevInfoChanged = false;
			DeviceInfo dev_info;
			GetDevInfo(&dev_info);
			TestMonitor::GetInstance()->SendDeviceInfoToPeer(dev_info);
		}

		//按键上报
		while(g_iKeyboardReport > 0)
		{
			TestMonitor::GetInstance()->SendKeyReportEvent("reset_key", KeyTest::CLICK);
			g_iKeyboardReport--;
		}

		current_time = GetSystemUptime_s();
#ifdef RC0240_LGV10
		if (!bPirInited)//pir初始化
		{
			printf("\033[1;36m     SystemPirInit----------->start       \033[0m\n");
			//pir
			int ret = -1;
			//AS006A PIR 上电后OUT先输出高电平，热机15秒后，输出低电平，进入检测状态
			ret = SystemPirInit();
			if (ret != -1)
			{
				bPirInited = 1;
				SystemSetPirSen(2,0);
				iPirResultValidTime = current_time + 5;//延后5秒再检测，因为设置pir灵敏度时，会检测到pir触发
			}
			
			printf("\033[1;36m     SystemPirInit----------->end       \033[0m\n");
		}		
		else
		{
			if(current_time > iPirResultValidTime)//触发后保持一秒有效
			{
				
				pir_det_result = SystemPirDet();//获取PIR结果
				if(pir_det_result == -1)
				{
					pir_det_result = 0;
					// usleep(50000); //50ms延时
					// continue;
				}
				
				iPirResultValidTime = current_time + 2;
				
				char report[10] = {0};
				//解析PIR的触发情况
				sprintf(report, "%d%d%d", 	(PIR1_SWITCH_MASK & pir_det_result) ? 1 : 0, 
											(PIR2_SWITCH_MASK & pir_det_result) ? 1 : 0, 
											(PIR3_SWITCH_MASK & pir_det_result) ? 1 : 0);
				TestMonitor::GetInstance()->SendPirReportEvent(report);//上报给产测工具
				AppErr("report pir alarm, report:%s\n",report);
				
			}
		}
#endif
		usleep(50 *1000); 		//50ms检测一次
	}
}





//add on 2024.12.30 PIR检测 end



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
	// snprintf(url,sizeof(url),"rtsp://%s:8554/stream_main",ip);
	snprintf(url,sizeof(url),"rtsp://%s:8554/main",ip);
	strncpy(g_rtsp_url, url, 99);

	char url_sub[100] = {0};
	// snprintf(url_sub,sizeof(url_sub),"rtsp://%s:8554/stream_sub",ip);
	snprintf(url_sub,sizeof(url_sub),"rtsp://%s:8554/sub",ip);
	strncpy(g_rtsp_url_sub, url_sub, 99);

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
//		session->AddSource(xop::channel_0, xop::H264Source::CreateNew(15)); 
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

static void PD_StopRtspPthread(void)
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

static int PD_StartRtspPthread(void)
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

static int PD_RtspPutFrame_Main(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
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

static int PD_RtspPutFrame_Sub(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
{
	//获取一帧 H265, 打包 去掉startcode 00 00 00 01
	// PD_NALU_H265_t nalu;
	PD_NALU_H264_t nalu;
    int32_t pos = 0, len = 0;
    
    while (pos<iDataLen) 
	{
        // len = ReadOneNaluH265FromBuf(pData, iDataLen, pos, &nalu);
        len = ReadOneNaluFromBuf(pData, iDataLen, pos, &nalu);
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
		// videoFrame.timestamp = xop::H265Source::GetTimestamp(); // 时间戳, 建议使用编码器提供的时间戳
		videoFrame.timestamp = xop::H264Source::GetTimestamp(); // 时间戳, 建议使用编码器提供的时间戳
		videoFrame.buffer.reset(new uint8_t[videoFrame.size]);                    
		memcpy(videoFrame.buffer.get(), nalu.buf, videoFrame.size);					
		if (!s_rtspThreadQuit && rtsp_server)
		{
			rtsp_server->PushFrame(session_id_sub, xop::channel_0, videoFrame); //送到服务器进行转发, 接口线程安全
		}
	}
	
	return 0;
}

static int PD_RtspPutFrame_Audio(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
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

static int PD_RtspPutFrame_Audio1(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen)
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

bool g_mic_is_open = false;
static char g711u_data[320] = {0};
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
	else if (DMC_MEDIA_AUDIO_FRIST_STREAM == media_chn)//pcm
	{
		// if (g_mic_is_open)
		// {
		// 	PD_RtspPutFrame_Audio(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
		// 	PD_RtspPutFrame_Audio1(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
		// 	// printf("----------------------------->onCapture_fn, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
			
		// 	//add on 2024.12.28 
		// 	memset(g711u_data, 0, sizeof(g711u_data));
		// 	int g711u_len = DG_encode_g711u((char *)pFrmBuff, (char *)g711u_data, uiFrmSize);
		// 	if(g711u_len > 0)
		// 		DoProductAudioRecord((const unsigned char*)g711u_data, g711u_len);
		// 	//add on 2024.12.28 end
		// }
	}
	else if(DMC_MEDIA_AUDIO_SECOND_STREAM == media_chn)//g711u
	{
			PD_RtspPutFrame_Audio(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
			PD_RtspPutFrame_Audio1(media_subtype, &ullTimeStamp, (char *)pFrmBuff, uiFrmSize);
		if (g_mic_is_open)
		{
			
			// printf("----------------------------->onCapture_fn, stream %d, size = %d, ret = %d\n", media_chn, frame_len, ret);	
			
			//add on 2024.12.28 
			// memset(g711u_data, 0, sizeof(g711u_data));
			// int g711u_len = DG_encode_g711u((char *)pFrmBuff, (char *)g711u_data, uiFrmSize);
			// if(g711u_len > 0)
				DoProductAudioRecord((const unsigned char*)pFrmBuff, uiFrmSize);
			//add on 2024.12.28 end
		}

	}

	return 0;
}

#if 0
static int gProcessWifiRun = 1;
static int gProcessWifiExit = 0;

#define MAX_SEARCH_WIFI (50)
CNetWifi::WIFI_LIST s_tpWifiList[MAX_SEARCH_WIFI];
int wifi_num = 0;
static int wifi_scan_count = 0;
static void *thread_search_wifi(void *arg)
{
	int num;
	CNetWifi::WIFI_LIST stWifi;
	CNetWifi::WIFI_LIST wifiList[MAX_SEARCH_WIFI];
	memset(s_tpWifiList, 0, sizeof(s_tpWifiList));

	while(gProcessWifiRun)
	{
		if(wifi_scan_count <= 1)
		{
			num = -1;
			memset(wifiList, 0, sizeof(wifiList));
			num = g_NetWifi.GetWifiWifiList(wifiList, MAX_SEARCH_WIFI);
			printf("wifi num = %d\n", num);
			if(num > 0)
			{
				wifi_scan_count++;
			}
		}
	
		
		
		if( num > 0 && wifi_scan_count == 2)
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
			
			 break;//目的是为了搜一次
		}

		sleep(1);
	}
	gProcessWifiExit = 1;
	return NULL;
}
#endif
int TestModuleV2_start()
{
	//PIR模块
	CreateDetachedThread((char*)"thread_report",thread_report, (void *)NULL, true);
	//启动音视频
	g_AVManager.RealTimeStreamStart(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO, onCapture_fn);
	//摄像头参数控制模块
//	g_Camera.start();
	
	// CreateDetachedThread((char*)"thread_search_wifi",thread_search_wifi, (void *)NULL, true);

	PD_StartRtspPthread();

	char ip[20] = {0};
	NetGetLocalIp("wlan0",ip);//产测只用wifi

	/*------------------------------------------------*/
    CreateLogModule();
    ILogInit(LOG_INSTANCE_TYPE_END);

    CreateTestModuleV2Module();
    const char *DEVICE_LOCAL_IP = ip;//"192.168.0.1";
    static std::shared_ptr<VTestMonitor> monitor = std::make_shared<TestMonitor>();

    ITestModuleV2::GetInstance()->SetTestMointor(monitor);
    ITestModuleV2::GetInstance()->Init(DEVICE_LOCAL_IP);
//    ITestModuleV2::GetInstance()->UnInit();
//    DestroyTestModuleV2Module();
//
//    ILogUnInit();
//    DestroyLogModule();
    return 0;
}
