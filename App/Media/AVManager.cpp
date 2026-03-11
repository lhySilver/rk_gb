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

