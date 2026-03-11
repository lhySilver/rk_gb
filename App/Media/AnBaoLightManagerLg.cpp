#include <errno.h>
#include <unistd.h>

#include "AnBaoLightManager.h"

CAnBaoLightManager* CAnBaoLightManager::m_instance = NULL;

CAnBaoLightManager::CAnBaoLightManager() : CThread("AnBaoLightManager", TP_PLAY)
{
	m_iMD_status = 0;
	m_bMD_Enable = false;
	m_eOperation = AnBaoLightManualOperation_AUTO;
	m_eAnBaoLightStatus = AnBaoLightStatus_CLOSE;

	if (g_Camera.GetWhiteLedManualEnable())
	{
		m_eOperation = AnBaoLightManualOperation_OPEN;
		m_eAnBaoLightStatus = AnBaoLightStatus_CLOSE;
	}
	else
	{
		m_eOperation = AnBaoLightManualOperation_CLOSE;
		m_eAnBaoLightStatus = AnBaoLightStatus_OPEN;
	}
	
}

CAnBaoLightManager::~CAnBaoLightManager()
{

}

CAnBaoLightManager* CAnBaoLightManager::instance()
{
	if(NULL == CAnBaoLightManager::m_instance)
	{
		CAnBaoLightManager::m_instance = new CAnBaoLightManager();
	}
	return CAnBaoLightManager::m_instance;
}

bool CAnBaoLightManager::Start()
{
	CreateThread();
	return true;
}

bool CAnBaoLightManager::Stop()
{
	DestroyThread();
	return true;
}

void CAnBaoLightManager::ThreadProc()
{
	int ret;
	
	while (m_bLoop) 
	{
		if( AnBaoLightManualOperation_AUTO == m_eOperation )
		{
			// if( (g_Alarm.GetAlarmLightSwitch()) && m_bMD_Enable )
			if( m_bMD_Enable )
			{
				unsigned int uiMotionDetectClearTime = 0;
				unsigned int uiMotionDetectStatus = 0;		//0 - 清除		1 - 触发
				unsigned int uiLastMotionDetectStatus = 0;	//上次的状态

				m_iMD_status = 0;

				// while( AnBaoLightManualOperation_AUTO == m_eOperation && (g_Alarm.GetAlarmLightSwitch()) && m_bMD_Enable )
				while( AnBaoLightManualOperation_AUTO == m_eOperation )
				{
					if( 1 == m_iMD_status )
					{
						if( 0 == uiMotionDetectStatus )
						{
							//侦测到移动状态开始
							uiMotionDetectStatus = 1;
						}
						uiMotionDetectClearTime = time(NULL) + 10;	//连续没有侦测到移动，才停止
					}
					else
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
							int iNightMode = g_Camera.GetDayNightMode();
							int iDayNightStatus = g_Camera.GetDayNightStatus();
							//if( ((CAMERA_MODE_AUTO == iNightMode) && (CAMERA_STATUS_NIGHT == iDayNightStatus)) || (CAMERA_MODE_OPEN == iNightMode) )
							if( (CAMERA_STATUS_NIGHT == iDayNightStatus) || (CAMERA_MODE_OPEN == iNightMode) )
							{
								uiLastMotionDetectStatus = uiMotionDetectStatus;

								g_Alarm.MotionDetectLightNotify(1);
								m_eAnBaoLightStatus = AnBaoLightStatus_OPEN;
								g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
								g_Camera.DoWhiteLedCtrl(true);
								g_Camera.DoDayNightMode(0,1);//强制白天
							}
						}
						else 							//移动结束
						{
							uiLastMotionDetectStatus = uiMotionDetectStatus;
							
							if( AnBaoLightStatus_OPEN == m_eAnBaoLightStatus )
							{
								//延时关灯
								int time_finish;
								time_finish = time(0) + g_Alarm.GetAlarmLightOnTime();
								// while( AnBaoLightManualOperation_AUTO == m_eOperation && (g_Alarm.GetAlarmLightSwitch()) && m_bMD_Enable )
								while( AnBaoLightManualOperation_AUTO == m_eOperation )
								{
									if( time(0) > time_finish )
										break;
									
									static int s_time_diff = 0;
									int time_diff = time_finish - time(0);
									if (time_diff != s_time_diff)
									{
										s_time_diff = time_diff;
										printf("auto op light countdown %d .....\n",time_diff);
									}

									usleep(30000);	//30ms
								}

								if( AnBaoLightManualOperation_OPEN != m_eOperation ) 		//如果手动开灯, 则不会关灯
								{
									int time_finish;
									//通知联动开灯结束
									g_Alarm.MotionDetectLightNotify(0);
									m_eAnBaoLightStatus = AnBaoLightStatus_CLOSE;
									g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
									g_Camera.DoWhiteLedCtrl(false);
									g_Camera.DoDayNightMode(0,0);//保持原来状态
									//退出循环, 从头开始执行
									break;
								}
							}
						}
						
					}
					
					usleep(30000);	//30ms
				}

				//只要不是手动开灯引起的退出, 都需要关灯
				if( (AnBaoLightManualOperation_OPEN != m_eOperation) && (AnBaoLightStatus_CLOSE != m_eAnBaoLightStatus) )
				{
					printf("auot op light countdown .....\n");
					//通知联动开灯结束
					g_Alarm.MotionDetectLightNotify(0);
					m_eAnBaoLightStatus = AnBaoLightStatus_CLOSE;
					g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
					g_Camera.DoWhiteLedCtrl(false);
					g_Camera.DoDayNightMode(0,0);//保持原来状态				
				}
				
			}
		}
		else if( AnBaoLightManualOperation_OPEN == m_eOperation )
		{
			if(AnBaoLightStatus_OPEN == m_eAnBaoLightStatus)
			{
				usleep(30000); 	//30ms
				continue;
			}

			int duration = g_Camera.GetWhiteLedDuration();
			if (duration > 0)
			{
				//延时亮灯时间到后亮灯
				int time_finish;
				time_finish = time(0) + duration;
				while( AnBaoLightManualOperation_OPEN == m_eOperation )
				{
					if( time(0) > time_finish )
						break;
					
					static int s_time_diff = 0;
					int time_diff = time_finish - time(0);
					if (time_diff != s_time_diff)
					{
						s_time_diff = time_diff;
						printf("manual op light counton %d .....\n",time_diff);
					}

					usleep(30000);	//30ms
				}
				if(AnBaoLightStatus_OPEN != m_eAnBaoLightStatus)
				{
					m_eAnBaoLightStatus = AnBaoLightStatus_OPEN;
					g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
					g_Camera.DoWhiteLedCtrl(true);
					g_Camera.DoDayNightMode(0,1);//强制切换白天
					g_Alarm.MotionDetectLightNotify(1);
				}
			}
			else
			{
				//延时亮灯时间小于等于0马上亮灯
				if(AnBaoLightStatus_OPEN != m_eAnBaoLightStatus)
				{
					m_eAnBaoLightStatus = AnBaoLightStatus_OPEN;
					g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
					g_Camera.DoWhiteLedCtrl(true);
					g_Camera.DoDayNightMode(0,1);//强制切换白天
					g_Alarm.MotionDetectLightNotify(1);
				}
			}
		}
		else //if( AnBaoLightManualOperation_CLOSE == m_eOperation )
		{
			//手动关灯才会进来的
			printf("manual op light countdown .....\n");
			if(AnBaoLightStatus_CLOSE != m_eAnBaoLightStatus)
			{
				//关灯
				g_Alarm.MotionDetectLightNotify(0);
				m_eAnBaoLightStatus = AnBaoLightStatus_CLOSE;
				g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
				g_Camera.DoWhiteLedCtrl(false);
				g_Camera.DoDayNightMode(0,0);//保持原来状态
			}
			
			//手动关灯，重新进入自动模式
			if( AnBaoLightManualOperation_CLOSE == m_eOperation )
			{
				m_eOperation = AnBaoLightManualOperation_AUTO;
			}
		}
		
		usleep(30000); 	//30ms
	}
}

void CAnBaoLightManager::SetAnBaoLightStatus(bool enable)
{
	CGuard guard(m_Mutex);

	int count = 40;
	AnBaoLightManualOperation_E eOperation;

	eOperation = enable ? AnBaoLightManualOperation_OPEN : AnBaoLightManualOperation_CLOSE;

	m_eOperation = eOperation;
	
	// if( AnBaoLightManualOperation_OPEN == eOperation )
	// {
	// 	while( (AnBaoLightStatus_OPEN != m_eAnBaoLightStatus) && (count > 0) )
	// 	{
	// 		count--;
	// 		usleep(50000);	//50ms
	// 	}
	// }
	// else// if( AnBaoLightManualOperation_CLOSE == eOperation )
	// {
	// 	while( (AnBaoLightStatus_CLOSE != m_eAnBaoLightStatus) && (count > 0) )
	// 	{
	// 		count--;
	// 		usleep(50000);	//50ms
	// 	}
	// }
}

bool CAnBaoLightManager::GetAnBaoLightStatus()
{
	return ((m_eAnBaoLightStatus == AnBaoLightStatus_OPEN) ? true : false);
}

void CAnBaoLightManager::SetAnBaoLightlightness()
{
	g_Alarm.SetAllowMotionDetTime(time(NULL)+10);
	g_Camera.DoWhiteLedCtrl(true);
}

void CAnBaoLightManager::SetMDStatus(int iMDStatus)
{
	m_iMD_status = iMDStatus;
}

void CAnBaoLightManager::SetMDEnable(bool enable)
{
	m_bMD_Enable = enable;
}
