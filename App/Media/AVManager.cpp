#include "Common.h"

CAVManager* CAVManager::_instance;

// CAVManager::CAVManager() : CThread("AVManager", TP_CAPTURE)
CAVManager::CAVManager()
{
	m_audioInit = false;
	m_videInit = false;
}

CAVManager::~CAVManager()
{
	
}

CAVManager* CAVManager::instance()
{
	if(NULL == CAVManager::_instance)
	{
		CAVManager::_instance = new CAVManager();
	}
	return CAVManager::_instance;
}

//视频
//extern int g_test_enc_type_change; //for debug
//extern int g_test_enc_type[2]; //for debug
void CAVManager::onConfigVideo(const CConfigTable &table, int &ret)
{
	VideoConf_S VideoConfig;
    TExchangeAL<VideoConf_S>::getConfig(table, VideoConfig);

	for (int i = 0; i < VIDEO_CHANNEL_MAX; i++)
	{
		if(memcmp(&m_VideoConfig.chan[i], &VideoConfig.chan[i], sizeof(VideoChannelConf_S)))
		{
			CaptureChangeEncParam(i, VideoConfig.chan[i].enc_type, VideoConfig.chan[i].bit_rate, VideoConfig.chan[i].frmae_rate, VideoConfig.chan[i].gop);
		}
	}
	m_VideoConfig = VideoConfig;
//	g_test_enc_type[0] = m_VideoConfig.chan[0].enc_type;
//	g_test_enc_type[1] = m_VideoConfig.chan[1].enc_type;
//	g_test_enc_type_change = 1;
}
bool CAVManager::VideoParamInit()
{
	CConfigTable table;
	g_configManager.getConfig(getConfigName(CFG_VIDEO), table);
    TExchangeAL<VideoConf_S>::getConfig(table, m_VideoConfig);
	g_configManager.attach(getConfigName(CFG_VIDEO), IConfigManager::Proc(&CAVManager::onConfigVideo, this));

	for (int i = 0; i < VIDEO_CHANNEL_MAX; i++)
	{
//		g_test_enc_type[i] = m_VideoConfig.chan[i].enc_type;
		CaptureInitEncParam(i, m_VideoConfig.chan[i].enc_type, m_VideoConfig.chan[i].bit_rate, m_VideoConfig.chan[i].frmae_rate, m_VideoConfig.chan[i].gop);
	}
	return true;
}

bool CAVManager::VideoInit()
{
	if (!m_videInit)
	{
		m_videInit = true;
		CaptureCreate(0);
		CaptureStart(0,0);
	}

	return true;
}

bool CAVManager::VideoDeInit()
{
	if (m_videInit)
	{
		m_videInit = false;
		CaptureStop(0,0);
		CaptureDestroy(0);
	}
	return true;
}

//音频
bool CAVManager::AudioInit()
{
	if (!m_audioInit)
	{
		m_audioInit = true;
		AudioCreate();
	}

	return true;
}

bool CAVManager::AudioDeInit()
{
	if (m_audioInit)
	{
		m_audioInit = false;
		AudioInDestroy(0);
	}
	
	return true;
}

//AV
bool CAVManager::RealTimeStreamStart(int media_type, dmc_media_input_fn proc)
{
	CaptureSetStreamCallBack((char*)"realtime",media_type,proc);
	return true;
}

bool CAVManager::RealTimeStreamStop(int media_type)
{
	CaptureSetStreamCallBack((char*)"realtime",media_type,NULL);
	return true;
}

bool CAVManager::RecordStreamStart(int media_type, dmc_media_input_fn proc)
{
	CaptureSetStreamCallBack((char*)"record",media_type,proc);
	return true;
}

bool CAVManager::RecordStreamStop(int media_type)
{
	CaptureSetStreamCallBack((char*)"record",media_type,NULL);
	return true;
}

// void CAVManager::ThreadProc()
// {
// 	while (m_bLoop) 
// 	{
// 		sleep(1);
// 	}
// }

bool CAVManager::PlayVoice(uchar *data, int length, int type, int format)
{
	AudioOutPutBuf(data,length);
	return true;
}

