#include <errno.h>
#include <unistd.h>
#include<sys/stat.h>
#include<math.h>

#include "AudioPrompt.h"
#include "../tuya/Tuya_common.h"
#include "../tuya/Tuya.h"

#define FILE_PLAY_TIME_MS (10*1000)  //限定单个语音播放文件播放时长
#define FILE_PLAY_COUNT   (10)       //限定语音播放文件缓存个数

CAudioPrompt* CAudioPrompt::m_instance = NULL;

CAudioPrompt::CAudioPrompt() : CThread("AudioPrompt", TP_PLAY)
{

	m_bRebootStart = false;
}

CAudioPrompt::~CAudioPrompt()
{

}

CAudioPrompt* CAudioPrompt::instance()
{
	if(NULL == CAudioPrompt::m_instance)
	{
		CAudioPrompt::m_instance = new CAudioPrompt();
	}
	return CAudioPrompt::m_instance;
}

bool CAudioPrompt::start()
{
	if( false == IsThreadOver() ) 	//防止多次调用
		return true;

	CreateThread();
	m_play = 0;
	m_bRebootStart = false;
	return true;
}

bool CAudioPrompt::stop()
{
	if( true == IsThreadOver() ) 	//防止多次调用
		return true;

	DestroyThread();

	m_play = 0;
	m_bRebootStart = false;
	return true;
}

void CAudioPrompt::ThreadProc()
{
	bool bRet;
	FILE *fp = NULL;
	AudioFileParm file;
	char buffer[320];
	char fileName[256];
	while (m_bLoop) 
	{
		if( 0 == getAudioFile(file) )
		{
			AppInfo("aoplay file : %s\n", file.strFileName.c_str());
			strncpy(fileName,file.strFileName.c_str(),sizeof(fileName)-1);
			AppErr("aoplay file : %s\n", fileName);
			bRet = IAudioManager::instance()->StartAudioOut(IAudioManager::AUDIO_TIP_TYPE);
			if( true == bRet )
			{
				struct stat size;
				int filesize = 0;
				unsigned long long play_time_in_ms = 0;
				unsigned long long send_time_in_ms = 0;
				unsigned long long s_timestamp = 0;
				unsigned long long e_timestamp = 0;
				unsigned long long wait_time_in_ms = 0;
				if (stat(fileName, &size) == 0)
				{
					printf("file size = %d\n",size.st_size);
					filesize = size.st_size;
				}
				if (filesize > 0)
				{
					float ms = (float)filesize/1024*8/64*1000;//PCM:8k 16bit  16Byte/1ms 16000 125kbps ;  G711U:64kbps
					printf("ms=%f\n",ms);
					play_time_in_ms = ceil(ms);  
					printf("play_time_in_ms=%llu\n",play_time_in_ms);
				}
				fp = fopen(fileName, "r");
				if( fp )
				{
					m_bStopPlay = false;
					struct timeval tv = {0};
					gettimeofday(&tv, NULL);
					s_timestamp = ((unsigned long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
					printf("s_timestamp=%llu\n",s_timestamp);
					while( !m_bStopPlay )
					{
						int n = fread(buffer, 1, 320, fp);
						if (n != 320)//读音频文件结束
						{
							if(n > 0)
							{
								IAudioManager::instance()->PlayVoice( (uchar *)buffer, n, IAudioManager::AUDIO_TIP_TYPE);
							}
							struct timeval tv = {0};
							gettimeofday(&tv, NULL);
							e_timestamp = ((unsigned long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
							printf("e_timestamp=%llu\n",e_timestamp);
							if (1==file.type) //一直播放文件，由外部停止
							{
								fseek(fp, 0, SEEK_SET );
								continue;
							}
							break;
						}
						IAudioManager::instance()->PlayVoice( (uchar *)buffer, n, IAudioManager::AUDIO_TIP_TYPE);
						usleep(20 * 1000);
					}
					
					if (0==file.type) //计算播放时间，等待播放完成后停止
					{
						send_time_in_ms = e_timestamp - s_timestamp;
						printf("send_time_in_ms=%llu\n",send_time_in_ms);
						if (play_time_in_ms > send_time_in_ms)
						{
							wait_time_in_ms = play_time_in_ms-send_time_in_ms;
							if(wait_time_in_ms > FILE_PLAY_TIME_MS)
							{
								wait_time_in_ms = FILE_PLAY_TIME_MS;
							}
							printf("wait_time_in_ms=%llu\n",wait_time_in_ms);
							usleep(wait_time_in_ms * 1000);
						}
					}
					fclose(fp);
					fp = NULL;
					if (!m_bStopPlay)
					{
						sleep(1);
					}
					AppErr("play end..\n");
					m_bStopPlay = true;
				}
				IAudioManager::instance()->StopAudioOut(IAudioManager::AUDIO_TIP_TYPE, true);			
			}
		}
		else
		{
			m_play = 0;
		}
		usleep(100000); 	//100ms
	}
}

int CAudioPrompt::aoPlay(AudioFileParm file)
{
	CGuard guard(m_Mutex);
	if (m_bRebootStart)
		return -1;

	if( m_audioFileDeque.size() < FILE_PLAY_COUNT )
	{
		m_play = 1;
		m_audioFileDeque.push_front(file);
	}
	return 0;
}
int CAudioPrompt::aoPlayForReboot(AudioFileParm file)
{
	CGuard guard(m_Mutex);
	int timeout = 10;
	m_bRebootStart = true;
	m_bStopPlay = true;
	unsigned long long s_timestamp = 0;
	unsigned long long e_timestamp = 0;
	//清除队列里面的播放文件
	struct timeval tv = {0};
	gettimeofday(&tv, NULL);
	s_timestamp = ((unsigned long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
	while (!m_audioFileDeque.empty())
	{
		m_audioFileDeque.pop_back();
		usleep(100 * 1000);
		
		if (!m_audioFileDeque.empty())
		{
			gettimeofday(&tv, NULL);
			e_timestamp = ((unsigned long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
			if (e_timestamp - s_timestamp > timeout*1000)
				break;
		}
	}
	
	//如果前面有文件播放等待退出或者超时
	while( 1 == m_play)
	{
		timeout --;

		if (timeout < 0 )
			break;
		
		printf("CAudioPrompt::timeout=%d\n",timeout);
		sleep(1);
	}

	m_play = 1;
	m_audioFileDeque.push_front(file);

	return 0;
}

int CAudioPrompt::getAudioFile(AudioFileParm &file)
{
	CGuard guard(m_Mutex);
	int ret = -1;
	if( m_audioFileDeque.size() > 0 )
	{
		file = m_audioFileDeque.back();
		m_audioFileDeque.pop_back();
		ret = 0;
	}
	else
	{
		ret = -1;
	}

	return ret;
}
std::string CAudioPrompt::get_play_siren_list(int list)
{
	std::string strFileName;
	switch(list)
	{
		case 0:
            strFileName = AUDIO_FILE_SIREN1;
		break;
		case 1:
			strFileName = AUDIO_FILE_SIREN2;
		break;
		case 2:
			strFileName = AUDIO_FILE_SIREN3;
		break;
		case 3:
			strFileName = AUDIO_FILE_SIREN4;
		break;
		case 4:
			strFileName = AUDIO_FILE_SIREN5;
		break;
		case 5:
			strFileName = AUDIO_FILE_SIREN6;
		break;
		case 6:
			strFileName = AUDIO_FILE_SIREN6;
		break;
		case 7:
			strFileName = AUDIO_FILE_SIREN6;
		break;
		case 8:
			strFileName = AUDIO_FILE_SIREN6;
		break;
		case 9:
			strFileName = AUDIO_FILE_SIREN6;
		break;
		default:
			break;
	}

	return strFileName;
}

int CAudioPrompt::playSiren()
{

	CAudioPrompt::AudioFileParm audioFile;
	audioFile.strFileName = get_play_siren_list(IPC_APP_get_Siren_Sound());
	audioFile.type = 1;
	g_AudioPrompt.aoPlay(audioFile);
	return 0;
}

void CAudioPrompt::stopPlay()
{
	m_bStopPlay = true;
}
int CAudioPrompt::getPlayStatus()
{
    return m_play;
}
