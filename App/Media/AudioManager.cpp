#include "AudioManager.h"

PATTERN_SINGLETON_IMPLEMENT(CAudioManager);

CAudioManager::CAudioManager() : CThread("AudioManager", TP_PLAY)
{
	m_VoiceType		= IAudioManager::AUDIO_TIP_TYPE; // 初始化为优先级最低
	if(ProductCof_g.speaker_reversed == 0)
		SystemAutioCtl(0);
	else
		SystemAutioCtl(1);
	m_uiTalkFrameCount = 0;
	m_uiAllFrameCount = 0;
	
	CreateThread();
}

CAudioManager::~CAudioManager()
{
	DestroyThread();
}

//各个模块开启语音时调用;
bool CAudioManager::StartAudioOut(IAudioManager::AudioOutType iAudioType)
{
	if( iAudioType > m_VoiceType )
		return false;

	if( iAudioType < m_VoiceType )
	{
		if(ProductCof_g.speaker_reversed == 0)
			SystemAutioCtl(0);
		else
			SystemAutioCtl(1);
	}
	
	m_VoiceType = iAudioType;
	
	if(ProductCof_g.speaker_reversed == 0)
		SystemAutioCtl(1);
	else
		SystemAutioCtl(0);

	return true;
}

//关闭语音功能，同时完成恢复低优先级的语音需求
void CAudioManager::StopAudioOut(IAudioManager::AudioOutType iAudioType, bool bForce/*=false*/)
{
	m_VoiceType = IAudioManager::AUDIO_TIP_TYPE;
	if( bForce )
	{
		if(ProductCof_g.speaker_reversed == 0)
			SystemAutioCtl(0);
		else
			SystemAutioCtl(1);
	}
}

void CAudioManager::ThreadProc()
{
	int iTalkCount = 0;
	unsigned int uiLastTalkFrameCount = 0;
	int iAllCount = 0;
	unsigned int uiLastAllFrameCount = 0;
	
	while (m_bLoop) 
	{
		if( uiLastTalkFrameCount != m_uiTalkFrameCount )
		{
			iTalkCount = 0;
			uiLastTalkFrameCount = m_uiTalkFrameCount;
		}
		else
		{
			iTalkCount++;
		}

		if( 5 == iTalkCount )
		{
			m_VoiceType = IAudioManager::AUDIO_TIP_TYPE;
		}

		
		if( uiLastAllFrameCount != m_uiAllFrameCount )
		{
			iAllCount = 0;
			uiLastAllFrameCount = m_uiAllFrameCount;
		}
		else
		{
			iAllCount++;
		}

		if( 5 == iAllCount )
		{
			if(ProductCof_g.speaker_reversed == 0)
				SystemAutioCtl(0);
			else
				SystemAutioCtl(1);
		}

		sleep(1);
	}
}


bool CAudioManager::PlayVoice(uchar *data, int length, IAudioManager::AudioOutType type, int format)
{
    CGuard	guard(m_AudioMutex);
	if ( IAudioManager::AUDIO_TALK_TYPE == type )
	{
		m_uiTalkFrameCount++;
		m_VoiceType = IAudioManager::AUDIO_TALK_TYPE;
	}
	
	if ( m_VoiceType != type )
	{
		return false;
	}


	m_uiAllFrameCount++;

	if ( IAudioManager::AUDIO_TALK_TYPE == type )
	{
		CConfigTable table;
		IpcVolConfig IpcVolCfg;
		g_configManager.getConfig(getConfigName(CFG_IPC_VOL), table);
		TExchangeAL<IpcVolConfig>::getConfig(table,IpcVolCfg);
		AppWarning("get IpcVolCfg.ipcvolume %d\n",IpcVolCfg.ipcvolume);
		AudioSetVolume(0,0,IpcVolCfg.ipcvolume);
	}
	else
	{
		CConfigTable table;
		SirenConfig SirenCfg;
		g_configManager.getConfig(getConfigName(CFG_SIREN), table);
		TExchangeAL<SirenConfig>::getConfig(table,SirenCfg);
		AppWarning("get sirenvolume %d\n",SirenCfg.sirenvolume);
		AudioSetVolume(0,0,SirenCfg.sirenvolume);
	}

	AudioOutPutBuf(data,length);

	return true;
}

bool CAudioManager::SetAudioOutVolume(int iChannel, unsigned int dwLVolume, unsigned int dwRVolume)
{
	return true;
}

IAudioManager* IAudioManager::instance()
{
    return CAudioManager::instance();
}

