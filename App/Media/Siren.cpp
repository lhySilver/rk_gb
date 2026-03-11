#include <errno.h>
#include <unistd.h>

#include "Siren.h"

CSiren* CSiren::m_instance = NULL;

CSiren::CSiren() : CThread("Siren", TP_PLAY)
{
	m_iMD_status = 0;
	m_bMD_Enable = false;
}

CSiren::~CSiren()
{

}

CSiren* CSiren::instance()
{
	if(NULL == CSiren::m_instance)
	{
		CSiren::m_instance = new CSiren();
	}
	return CSiren::m_instance;
}

bool CSiren::Start()
{
	CConfigTable tableSiren;
	g_configManager.getConfig(getConfigName(CFG_SIREN), tableSiren);

	TExchangeAL<SirenConfig>::getConfig(tableSiren,m_CCfgSiren);
	g_configManager.attach(getConfigName(CFG_SIREN), IConfigManager::Proc(&CSiren::onConfigSiren, this));

	m_eOperation = SirenOperation_AUTO;
	m_eSirentStatus = SirenStatus_CLOSE;
	
	CreateThread();
	return true;
}

bool CSiren::Stop()
{
	g_configManager.detach(getConfigName(CFG_SIREN), IConfigManager::Proc(&CSiren::onConfigSiren, this));
	DestroyThread();
	return true;
}

void CSiren::onConfigSiren(const CConfigTable &table, int &ret)
{
	SirenConfig Cfgsiren;
	TExchangeAL<SirenConfig>::getConfig(table, Cfgsiren);
	if ( (m_CCfgSiren.sirenvolume != Cfgsiren.sirenvolume) || (m_CCfgSiren.sirenlist != Cfgsiren.sirenlist) )
	{
		if (!GetSirenStatus()) //如果按下按钮或者联动报警，不播放
		{
			g_AudioPrompt.stopPlay();
			CAudioPrompt::AudioFileParm audioFile;
			audioFile.strFileName = g_AudioPrompt.get_play_siren_list(Cfgsiren.sirenlist);
			audioFile.type = 0;
			g_AudioPrompt.aoPlay(audioFile);
		}
	}
	m_CCfgSiren = Cfgsiren;
}

void CSiren::ThreadProc()
{
	int ret;
	
	while (m_bLoop) 
	{
		if( SirenOperation_AUTO == m_eOperation )//自动 add on 2025.01.06 注释
		{
			if( m_CCfgSiren.autosiren && m_bMD_Enable )//警笛联动开了 && 联动触发
			{
				unsigned int uiMotionDetectClearTime = 0;	//移动侦测清除时间
				unsigned int uiMotionDetectStatus = 0;		//0 - 清除		1 - 触发
				unsigned int uiLastMotionDetectStatus = 0;	//上次的状态

				m_iMD_status = 0;

				//满足联动警笛的基础条件（相应的开关开启）add on 2025.01.06 注释
				while( SirenOperation_AUTO == m_eOperation && m_CCfgSiren.autosiren && m_bMD_Enable )
				{
					if( 1 == m_iMD_status )	// 联动触发 add on 2025.01.06 注释
					{
						if( 0 == uiMotionDetectStatus )
						{
							//侦测到移动状态开始
							uiMotionDetectStatus = 1;
						}
						uiMotionDetectClearTime = time(NULL) + 10;	//连续没有侦测到移动，才停止
					}						
					else				 	//联动触发结束，则会进入等待蜂鸣时长的流程 add on 2025.01.06 注释
					{
						if( time(NULL) > uiMotionDetectClearTime )
						{
							if( 1 == uiMotionDetectStatus )
							{
								//侦测到移动状态清除
								uiMotionDetectStatus = 0;
							}
						}
					}

					if( uiLastMotionDetectStatus != uiMotionDetectStatus )		//状态改变, 调用处理函数
					{
						if( uiMotionDetectStatus ) 		//移动开始
						{
							uiLastMotionDetectStatus = uiMotionDetectStatus;

							printf("\033[1;32m""----------> SirenStatus_OPEN 1\n""\033[0m");
							m_eSirentStatus = SirenStatus_OPEN;
							g_TuyaHandle.ReportSirenSwitch(1);
							g_AudioPrompt.playSiren();
							
						}
						else 							//移动结束
						{
							uiLastMotionDetectStatus = uiMotionDetectStatus;
							
							#if 0
							break;
							#else
							if( SirenStatus_OPEN == m_eSirentStatus )//如果蜂鸣器处于开启的状态 add on 2025.01.06 注释
							{
								//延时关闭
								int time_finish;
								time_finish = time(0) + m_CCfgSiren.sirenduration;//如果哪天客户投诉联动蜂鸣器的时间不太对，就在此处把我们大工等待移动侦测结束的10s减去即可 add on 2025.01.07 注释
								while( SirenOperation_AUTO == m_eOperation && m_CCfgSiren.autosiren && m_bMD_Enable )
								{
									if( time(0) > time_finish )
										break;
									
									//打印关蜂鸣器倒计时 add on 2025.01.06 注释
									static int s_time_diff = 0;
									int time_diff = time_finish - time(0);
									if (time_diff != s_time_diff)
									{
										s_time_diff = time_diff;
										printf("auto op siren countdown %d .....\n",time_diff);
									}
									//打印关蜂鸣器倒计时 add on 2025.01.06 注释
									usleep(30000);	//30ms
								}

								if( SirenOperation_OPEN != m_eOperation ) 		//如果手动开, 则不会关
								{
									int time_finish;
									//通知联动开警笛结束
									m_eSirentStatus = SirenStatus_CLOSE;
									g_TuyaHandle.ReportSirenSwitch(0);
									g_AudioPrompt.stopPlay();
									//退出循环, 从头开始执行
									break;
								}
							}
							#endif
						}
						
					}
					
					usleep(30000);	//30ms
				}

				//只要不是手动开警笛引起的退出, 都需要关警笛
				if( (SirenOperation_OPEN != m_eOperation) && (SirenStatus_CLOSE != m_eSirentStatus) )
				{
					printf("auot op siren countdown .....\n");
					//通知联动开警笛结束
					m_eSirentStatus = SirenStatus_CLOSE;
					g_TuyaHandle.ReportSirenSwitch(0);
					g_AudioPrompt.stopPlay();	
				}
				
			}
		}
		else if( SirenOperation_OPEN == m_eOperation )//手动打开蜂鸣器 add on 2025.01.07 注释
		{
			if(SirenStatus_OPEN != m_eSirentStatus)//蜂鸣器状态不为开启时，打开蜂鸣器，并将状态标志位切换至开启 add on 2025.01.07 注释
			{
				printf("\033[1;32m""----------> SirenStatus_OPEN 2\n""\033[0m");
				m_eSirentStatus = SirenStatus_OPEN;
				g_TuyaHandle.ReportSirenSwitch(1);
				g_AudioPrompt.playSiren();
			}

			//延时开警笛时间到后关
			int time_finish;
			time_finish = time(0) + m_CCfgSiren.sirenduration;
			while( SirenOperation_OPEN == m_eOperation )//等待sirenduration
			{
				if( time(0) > time_finish )
					break;
				
				static int s_time_diff = 0;
				int time_diff = time_finish - time(0);
				if (time_diff != s_time_diff)
				{
					s_time_diff = time_diff;
					printf("manual op siren countdown %d .....\n",time_diff);
				}

				usleep(30000);	//30ms
			}
			
			//关警笛
			if( SirenOperation_OPEN == m_eOperation )//时间到，上报关闭状态
			{
				//超时关警笛需要通知APP
				g_TuyaHandle.ReportSirenSwitch(0);
			}
			m_eSirentStatus = SirenStatus_CLOSE;
			g_AudioPrompt.stopPlay();

			m_eOperation = SirenOperation_CLOSE;	//联动超时，退出联动状态
			
			//如果超时退出, 重新进入自动模式
			if( SirenOperation_CLOSE == m_eOperation )
			{
				m_eOperation = SirenOperation_AUTO;
			}
		}
		else //if( SirenOperation_CLOSE == m_eOperation )//手动关蜂鸣器 add on 2025.01.07 注释
		{
			//这里是联动触发开警笛, 然后手动关才会进来的
			printf("manual op siren countdown .....\n");
			if(SirenStatus_CLOSE != m_eSirentStatus)
			{
				//关警笛
				m_eSirentStatus = SirenStatus_CLOSE;
				g_TuyaHandle.ReportSirenSwitch(0);
				g_AudioPrompt.stopPlay();
			}

			//如果超时退出, 重新进入自动模式
			if( SirenOperation_CLOSE == m_eOperation )
			{
				m_eOperation = SirenOperation_AUTO;
			}
		}
		
		usleep(30000); 	//30ms
	}
}

void CSiren::SetSirenStatus(bool enable)
{
	CGuard guard(m_Mutex);

	int count = 40;
	SirenManualOperation_E eOperation;

	eOperation = enable ? SirenOperation_OPEN : SirenOperation_CLOSE;

	m_eOperation = eOperation;
	
	if( SirenOperation_OPEN == eOperation )
	{
		while( (SirenStatus_OPEN != m_eSirentStatus) && (count > 0) )
		{
			count--;
			usleep(50000);	//50ms
		}
	}
	else// if( SirenOperation_CLOSE == eOperation )
	{
		while( (SirenStatus_CLOSE != m_eSirentStatus) && (count > 0) )
		{
			count--;
			usleep(50000);	//50ms
		}
	}
}

bool CSiren::GetSirenStatus()
{
	printf("\033[1;32m""----------> SirenStatus_OPEN %s\n""\033[0m", (m_eSirentStatus == SirenStatus_OPEN) ? "true" : "false");
	return ((m_eSirentStatus == SirenStatus_OPEN) ? true : false);
}

/**
 * @brief 设置警笛联动标志位
 *
 * 设置警笛的联动标志位。
 *
 * @param iMDStatus 警笛联动标志位，用于控制警笛的开关状态
 *
 * @details
 * 该函数用于设置警笛的联动标志位，将传入的参数赋值给成员变量 m_iMD_status。
 * 此函数在 2025 年 1 月 6 日添加注释。
 */
void CSiren::SetMDStatus(int iMDStatus)
{
	m_iMD_status = iMDStatus;
}

/**
 * @brief 设置警笛联动开关
 *
 * 设置警笛联动的基础条件，移动侦测开关决定。
 *
 * @param enable 是否启用警笛联动
 *     - true: 启用警笛联动
 *     - false: 禁用警笛联动
 *
 * @note 此函数于2025年1月6日添加注释
 */
void CSiren::SetMDEnable(bool enable)//设置警笛联动的基础条件 PIR开启 或者 移动侦测打开 add on 2025.01.06 添加注释
{
	m_bMD_Enable = enable;
}
