#include "Ptz.h"

#include "Log/DebugDef.h"
#include "DebugInfo/debuginfo.h"
#include "../tuya/Tuya.h"
#include "../tuya/Tuya_common.h"
#include "Media/Camera.h"
#include "PAL/Camera.h"
#include "PAL/Motor.h"


//static bool s_bMotorUsed = false;
static struct motors_steps s_stMotorsSteps;
static int s_iMotorMoveSpeed;
static bool s_bFlastStop = false;
enum motor_used s_bMotorUsed;
int motion_track_check = 0;
int motion_track_check_tmp = -1;
int iAllowMotorTrackTime;

extern int DeviceMode_g; 		//0:正常模式， 1:测试模式


extern unsigned char bStartPrivateMode; 		//是否开启隐私模式
extern PtzPosition_s pstPtzPosition_sleep;

PATTERN_SINGLETON_IMPLEMENT(CPtz);

CPtz::CPtz() : CThread("PTZ", 50)
{
	m_bDoAutoTest = false;
	m_bDoPtzTest = false;
	m_Ptzstatus = false;
}

CPtz::~CPtz()
{
}

bool CPtz::start()
{
	CConfigTable table;
	g_configManager.getConfig(getConfigName(CFG_PTZ_RESUME_POS), table);
	TExchangeAL<PtzPosition>::getConfig(table, m_stPtzPosition);
	pstPtzPosition_sleep.x = m_stPtzPosition.privatex;
	pstPtzPosition_sleep.y = m_stPtzPosition.privatey;
	AppInfo("ptz resume pos enable : %s, pos[%d, %d]\n", m_stPtzPosition.Enable ? "true" : "false", m_stPtzPosition.x, m_stPtzPosition.y);

	return CreateThread();
}

bool CPtz::stop()
{
	DestroyThread();
	MotoUinit();
	return true;
}

void CPtz::OnCheckMotorAuto(Param wParam)
{
	int ret = -1;
	struct motor_config ptz_info_auto;
	struct motor_get_status stMotorMessage_auto;
	
	ret = MotorGetStatus(&stMotorMessage_auto);
	if( 0 == ret )
	{
		motion_track_check_tmp = -1;
		motion_track_check = 0;
		iAllowMotorTrackTime = time(NULL) + 10;
		ptz_info_auto.en = MOTOR_ENABLE;
		ptz_info_auto.nsec = ProductCof_g.track_speed;
		if(m_stPtzPosition.x - stMotorMessage_auto.fhx_cur_step<0)
		{
			ptz_info_auto.mt_rotat = MOTOR_CORATATION;
		}
		else
		{
			ptz_info_auto.mt_rotat = MOTOR_REVERSE;
		}
		ptz_info_auto.step_num = abs(m_stPtzPosition.x - stMotorMessage_auto.fhx_cur_step);
		MotorMove(0,&ptz_info_auto);
		if(m_stPtzPosition.y - stMotorMessage_auto.fhy_cur_step<0)
		{
			ptz_info_auto.mt_rotat = MOTOR_CORATATION;
		}
		else
		{
			ptz_info_auto.mt_rotat = MOTOR_REVERSE;
		}
		ptz_info_auto.step_num = abs(m_stPtzPosition.y - stMotorMessage_auto.fhy_cur_step);
		MotorMove(1,&ptz_info_auto);
		s_bMotorUsed = MOTOR_IS_MANUAL;
	}
}

int CPtz::Calibration()
{
	struct motor_set_step step;
	step.xmax_steps = ProductCof_g.lr_len*8;
	step.ymax_steps = ProductCof_g.ud_len*8;
	step.check_speed = ProductCof_g.check_speed;

	iAllowMotorTrackTime = time(NULL) + 20;

	MotorSetStep(step);
	MotoReset();

	return 0;
}

void CPtz::ThreadProc()
{
	int ret;
	int iLastTime = 0;
	struct motor_config ptz_info;
	struct motor_get_status pstMotorMessage;
	ret = MotoInit();
	if( 0 != ret )
	{
		AppErr("Motor init failure\n");
		return ;
	}

	Calibration();//云台自检校准

	if( (-1 == m_stPtzPosition.x) && (-1 == m_stPtzPosition.y) ) 		//初始状态
	{
		ptz_info.en = MOTOR_ENABLE;
		ptz_info.nsec = ProductCof_g.check_speed;
		if(ProductCof_g.up_len < 0)
		{
			ptz_info.mt_rotat = MOTOR_CORATATION;
		}
		else
		{
			ptz_info.mt_rotat = MOTOR_REVERSE;
		}
		ptz_info.step_num = abs(ProductCof_g.up_len*8);
		MotorMove(1,&ptz_info);
	}
	else
	{
		ret = MotorGetStatus(&pstMotorMessage);
		if( 0 == ret )
		{
			ptz_info.en = MOTOR_ENABLE;
			ptz_info.nsec = ProductCof_g.check_speed;
			if(m_stPtzPosition.x - pstMotorMessage.fhx_cur_step<0)
			{
				ptz_info.mt_rotat = MOTOR_CORATATION;
			}
			else
			{
				ptz_info.mt_rotat = MOTOR_REVERSE;
			}
			ptz_info.step_num = abs(m_stPtzPosition.x - pstMotorMessage.fhx_cur_step);
			MotorMove(0,&ptz_info);
			if(m_stPtzPosition.y - pstMotorMessage.fhy_cur_step<0)
			{
				ptz_info.mt_rotat = MOTOR_CORATATION;
			}
			else
			{
				ptz_info.mt_rotat = MOTOR_REVERSE;
			}
			ptz_info.step_num = abs(m_stPtzPosition.y - pstMotorMessage.fhy_cur_step);
			MotorMove(1,&ptz_info);
		}
	}

	s_bMotorUsed = MOTOR_IS_MANUAL;

	while( m_bLoop )
	{
		//在老化测试和PCBA测试时使用
		if( m_bDoPtzTest )
		{
			ptz_info.en = MOTOR_ENABLE;
			ptz_info.nsec = 1400000;
			ptz_info.mt_rotat = MOTOR_CORATATION;
			ptz_info.step_num = ProductCof_g.lr_len * 8;
			MotorMove(0,&ptz_info);
			ptz_info.step_num = ProductCof_g.ud_len * 8;
			MotorMove(1,&ptz_info);
			while(1)
			{
				ret = MotorGetStatus(&pstMotorMessage);
				if( 0 == ret )
				{
					if( MOTOR_IS_STOP == pstMotorMessage.stop_enable )
						break;
				}
				else
				{
					break;
				}
				usleep(500000);
			}
			usleep(2000000);
			ptz_info.en = MOTOR_ENABLE;
			ptz_info.nsec = 1400000;
			ptz_info.mt_rotat = MOTOR_REVERSE;
			ptz_info.step_num = ProductCof_g.lr_len * 8;
			MotorMove(0,&ptz_info);
			ptz_info.step_num = ProductCof_g.ud_len * 8;
			MotorMove(1,&ptz_info);
			while(1)
			{
				ret = MotorGetStatus(&pstMotorMessage);
				if( 0 == ret )
				{
					if( MOTOR_IS_STOP == pstMotorMessage.stop_enable )
						break;
				}
				else
				{
					break;
				}
				usleep(500000);
			}
			usleep(2000000);
			continue;
		}//在老化测试和PCBA测试时使用


		if( (time(0) - iLastTime) > 2)
		{
			iLastTime = time(0);

			if( g_TuyaHandle.IsTuyaSdkStarted() && (s_bMotorUsed == MOTOR_IS_AUTO) )
			{
				ret = MotorGetStatus(&pstMotorMessage);
				if( 0 == ret )
				{
					if(MOTOR_IS_STOP == pstMotorMessage.stop_enable)
					{
						if(m_Ptzstatus == true)
						{
							printf("auto*********allow picture*********\n");
							m_Ptzstatus = false;
						}
					}
				}
			}
			
			if( g_TuyaHandle.IsTuyaSdkStarted() && (s_bMotorUsed == MOTOR_IS_MANUAL) )
			{
				if(motion_track_check != 0)
				{
					printf("start maanual\n");
					motion_track_check = 0;
					motion_track_check_tmp = -1;
					m_timerMotorauto.Stop();
				}
				ret = MotorGetStatus(&pstMotorMessage);
				if( 0 == ret )
				{
					//保留位置状态保存（最后一个位置：手动控制的位置，隐私模式下的位置，收藏点跳转的位置）
					if( (MOTOR_IS_STOP == pstMotorMessage.stop_enable) && 
						((m_stPtzPosition.x != pstMotorMessage.fhx_cur_step) || (m_stPtzPosition.y != pstMotorMessage.fhy_cur_step)) )
					{
						m_stPtzPosition.x = pstMotorMessage.fhx_cur_step;
						m_stPtzPosition.y = pstMotorMessage.fhy_cur_step;
						m_stPtzPosition.privatex = pstPtzPosition_sleep.x;
						m_stPtzPosition.privatey = pstPtzPosition_sleep.y;
						m_Ptzstatus = false;
						printf("m_stPtzPosition.x is %d     m_stPtzPosition.y is %d\n",m_stPtzPosition.x,m_stPtzPosition.y);
						CConfigTable table;
						TExchangeAL<PtzPosition>::setConfig(m_stPtzPosition, table);
						g_configManager.setConfig(getConfigName(CFG_PTZ_RESUME_POS), table, 0, IConfigManager::applyOK);
					}
					if(MOTOR_IS_STOP == pstMotorMessage.stop_enable)
					{
						if(m_Ptzstatus == true)
						{
							printf("manual*********allow picture*********\n");
							m_Ptzstatus = false;
						}
					}
				}
			}	
			if( g_TuyaHandle.IsTuyaSdkStarted() && (s_bMotorUsed == MOTOR_IS_AUTO) )
			{
				if(motion_track_check != motion_track_check_tmp)
				{
					printf("start auto OnCheckMotorAuto\n");
					motion_track_check_tmp = motion_track_check;

					//没有移动追踪后回到保留位置
					m_timerMotorauto.Start(CTimer::Proc(&CPtz::OnCheckMotorAuto, this), ProductCof_g.tracker_stop_overtime*1000, 0);
				}
			}
		}
		
		//产测模式下测试
		if( m_bDoAutoTest && (1 == DeviceMode_g) )
		{
			m_bDoAutoTest = false;
			ptz_info.en = MOTOR_ENABLE;
			ptz_info.nsec = 1000000;
			ptz_info.mt_rotat = MOTOR_CORATATION;
			ptz_info.step_num = ProductCof_g.lr_len * 8;
			MotorMove(0,&ptz_info);
			ptz_info.step_num = ProductCof_g.ud_len * 8;
			MotorMove(1,&ptz_info);
			while(1)
			{
				ret = MotorGetStatus(&pstMotorMessage);
				if( 0 == ret )
				{
					if( MOTOR_IS_STOP == pstMotorMessage.stop_enable )
						break;
				}
				else
				{
					break;
				}
				usleep(100000);
			}
			ptz_info.en = MOTOR_ENABLE;
			ptz_info.nsec = 1000000;
			ptz_info.mt_rotat = MOTOR_REVERSE;
			ptz_info.step_num = ProductCof_g.lr_len * 8;
			MotorMove(0,&ptz_info);
			ptz_info.step_num = ProductCof_g.ud_len * 8;
			MotorMove(1,&ptz_info);
			while(1)
			{
				ret = MotorGetStatus(&pstMotorMessage);
				if( 0 == ret )
				{
					if( MOTOR_IS_STOP == pstMotorMessage.stop_enable )
						break;
				}
				else
				{
					break;
				}
				usleep(100000);
			}
			ptz_info.en = MOTOR_ENABLE;
			ptz_info.nsec = 1000000;
			ptz_info.mt_rotat = MOTOR_CORATATION;
			ptz_info.step_num = ProductCof_g.lr_len * 4;
			MotorMove(0,&ptz_info);
			ptz_info.step_num = ProductCof_g.ud_len * 4;
			MotorMove(1,&ptz_info);
		}//产测模式下测试
		
		usleep(50000);
	}
}

int CPtz::motion_track_ptz_op(int dx,int dy)
{
	int ret;
	struct motor_config ptz_info;
	struct motor_get_status pstMotorMessage;

	if(bStartPrivateMode == 1)
		return 0;

	if( time(NULL) < iAllowMotorTrackTime )
			return 0;

	motion_track_check++;
	s_bMotorUsed = MOTOR_IS_AUTO;

	if ( (dx < 1200) || (dx > 2500) )
	{
		printf("x range is to max\n");
		return 0;
	}
	
	ret = MotorGetStatus(&pstMotorMessage);
	if( 0 == ret )
	{
		ptz_info.en = MOTOR_ENABLE;
		ptz_info.nsec = ProductCof_g.track_speed;
		//
		if(dx - pstMotorMessage.fhx_cur_step < 0)
		{
			if( RA_NONE == g_Camera.getRotateAttr() )
				ptz_info.mt_rotat = MOTOR_CORATATION;
			else
				ptz_info.mt_rotat = MOTOR_REVERSE;
		}
		else
		{
			if( RA_NONE == g_Camera.getRotateAttr() )
				ptz_info.mt_rotat = MOTOR_REVERSE;
			else
				ptz_info.mt_rotat = MOTOR_CORATATION;
		}
		printf("ptz cur x :[%u]\n",pstMotorMessage.fhx_cur_step);
		ptz_info.step_num = abs(dx - pstMotorMessage.fhx_cur_step);
		if (ptz_info.step_num > 50)
		{
			printf("ptz x :[%u] -> [%u]\n",pstMotorMessage.fhx_cur_step,dx);

			s_bMotorUsed = MOTOR_IS_AUTO;
			g_PtzHandle.m_Ptzstatus = true;

			MotorMove(0,&ptz_info);
		}
		//
		if(dy - pstMotorMessage.fhy_cur_step < 0)
		{
			if( RA_NONE == g_Camera.getRotateAttr() )
				ptz_info.mt_rotat = MOTOR_CORATATION;
			else
				ptz_info.mt_rotat = MOTOR_REVERSE;
		}
		else
		{
			if( RA_NONE == g_Camera.getRotateAttr() )
				ptz_info.mt_rotat = MOTOR_REVERSE;
			else
				ptz_info.mt_rotat = MOTOR_CORATATION;
		}
		printf("ptz cur y :[%u]\n",pstMotorMessage.fhy_cur_step);
		ptz_info.step_num = abs(dy - pstMotorMessage.fhy_cur_step);
		if (ptz_info.step_num > 50)
		{
			printf("ptz y :[%u] -> [%u]\n",pstMotorMessage.fhy_cur_step,dy);

			s_bMotorUsed = MOTOR_IS_AUTO;
			g_PtzHandle.m_Ptzstatus = true;

			MotorMove(1,&ptz_info);
		}
	}

	return 0;
}
extern int s_scale;
void CPtz::DoMotorMove(PTZDirection_e dir, bool bStep/*=false*/)
{
	//通过驱动控制云台
	int x_dist, y_dist;
	struct motor_config ptz_info;

	s_bMotorUsed = MOTOR_IS_MANUAL;
	iAllowMotorTrackTime = time(NULL) + 20;
	int MotorLrMoveSpeed = ProductCof_g.run_speed;
	int MotorUdMoveSpeed = ProductCof_g.run_ud_speed;

	printf("s_scale=%d\n",s_scale);
	switch (s_scale)
	{
		case 0:
			MotorLrMoveSpeed = ProductCof_g.run_speed;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed;
		break;
		case 1:
			MotorLrMoveSpeed = ProductCof_g.run_speed;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed;
		break;
		case 2:
			MotorLrMoveSpeed = ProductCof_g.run_speed+1000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+1000000;
		break;
		case 3:
			MotorLrMoveSpeed = ProductCof_g.run_speed+2000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+2000000;
		break;
		case 4:
			MotorLrMoveSpeed = ProductCof_g.run_speed+3000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+3000000;
		break;
		case 5:
			MotorLrMoveSpeed = ProductCof_g.run_speed+4000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+4000000;
		break;
		case 6:
			MotorLrMoveSpeed = ProductCof_g.run_speed+5000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+5000000;
		break;
		case 7:
			MotorLrMoveSpeed = ProductCof_g.run_speed+6000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+6000000;
		break;
		case 8:
			MotorLrMoveSpeed = ProductCof_g.run_speed+6000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+6000000;
		break;
		case 9:
			MotorLrMoveSpeed = ProductCof_g.run_speed+6000000;
			MotorUdMoveSpeed = ProductCof_g.run_ud_speed+6000000;
		break;

		default:
		MotorLrMoveSpeed = ProductCof_g.run_speed;
		MotorUdMoveSpeed = ProductCof_g.run_ud_speed;
		break;
	}
	printf("MotorLrMoveSpeed=%u\n",MotorLrMoveSpeed);
	printf("MotorUdMoveSpeed=%u\n",MotorUdMoveSpeed);
	if( 1 )
	{
		x_dist = ProductCof_g.lr_len * 8;
		y_dist = ProductCof_g.ud_len * 8;
	}
	
	ptz_info.en = MOTOR_ENABLE;
	if((dir == 1) || (dir == 2))		
		ptz_info.nsec = MotorLrMoveSpeed;//ProductCof_g.run_speed;
	else
		ptz_info.nsec = MotorUdMoveSpeed;//ProductCof_g.run_ud_speed;
	switch (dir)
	{
		case PTZ_LEFT:
			ptz_info.mt_rotat = MOTOR_CORATATION;
			ptz_info.step_num = x_dist;
			break;
		case PTZ_RIGHT:
			ptz_info.mt_rotat = MOTOR_REVERSE;
			ptz_info.step_num = x_dist;
			break;
		case PTZ_UP:
			ptz_info.mt_rotat = MOTOR_CORATATION;
			ptz_info.step_num = y_dist;
			break;
		case PTZ_DOWN:
			ptz_info.mt_rotat = MOTOR_REVERSE;
			ptz_info.step_num = y_dist;
			break;
		default :
			return ;
	}

	if((dir == 1) || (dir == 2))
	{
		if( ProductCof_g.lr_dir )
		{
			if(ptz_info.mt_rotat == MOTOR_CORATATION)
			{
				ptz_info.mt_rotat = MOTOR_REVERSE;
			}
			else
			{
				ptz_info.mt_rotat = MOTOR_CORATATION;
			}
		}
	}
	else
	{ 
		if ( ProductCof_g.ud_dir )
		{
			if(ptz_info.mt_rotat == MOTOR_CORATATION)
			{
				ptz_info.mt_rotat = MOTOR_REVERSE;
			}
			else
			{
				ptz_info.mt_rotat = MOTOR_CORATATION;
			}
		}
	}
	
	//画面翻转
	if( RA_NONE != g_Camera.getRotateAttr() )
	{
		if(ptz_info.mt_rotat == MOTOR_CORATATION)
		{
			ptz_info.mt_rotat = MOTOR_REVERSE;
		}
		else
		{
			ptz_info.mt_rotat = MOTOR_CORATATION;
		}
	}
	if((dir == 1) || (dir == 2))
	{
		MotorMove(0,&ptz_info);
	}
	else
	{
		MotorMove(1,&ptz_info);
	}

}

void CPtz::DoMotorMove_private(PTZDirection_e dir)
{
	//通过驱动控制云台
	int x_dist, y_dist;
	struct motor_config ptz_info;

	s_bMotorUsed = MOTOR_IS_MANUAL;

	s_iMotorMoveSpeed = ProductCof_g.run_speed;
	MotorSetSpeed(s_iMotorMoveSpeed); 		//先回复默认速度, 因为移动追踪会改变云台速度

	if( 1 )
	{
		x_dist = ProductCof_g.lr_len * 8;
		y_dist = ProductCof_g.ud_len * 8;
	}
	
	ptz_info.en = MOTOR_ENABLE;
	if((dir == 1) || (dir == 2))		
		ptz_info.nsec = ProductCof_g.run_speed;
	else
		ptz_info.nsec = ProductCof_g.run_ud_speed;
	switch (dir)
	{
		case PTZ_LEFT:
			ptz_info.mt_rotat = MOTOR_CORATATION;
			ptz_info.step_num = x_dist;
			break;
		case PTZ_RIGHT:
			ptz_info.mt_rotat = MOTOR_REVERSE;
			ptz_info.step_num = x_dist;
			break;
		case PTZ_UP:
			ptz_info.mt_rotat = MOTOR_CORATATION;
			ptz_info.step_num = y_dist;
			break;
		case PTZ_DOWN:
			ptz_info.mt_rotat = MOTOR_REVERSE;
			ptz_info.step_num = y_dist;
			break;
		default :
			return ;
	}

	
	if((dir == 1) || (dir == 2))
	{
		if( ProductCof_g.lr_dir )
		{
			if(ptz_info.mt_rotat == MOTOR_CORATATION)
			{
				ptz_info.mt_rotat = MOTOR_REVERSE;
			}
			else
			{
				ptz_info.mt_rotat = MOTOR_CORATATION;
			}
		}
	}
	else
	{ 
		if ( ProductCof_g.ud_dir )
		{
			if(ptz_info.mt_rotat == MOTOR_CORATATION)
			{
				ptz_info.mt_rotat = MOTOR_REVERSE;
			}
			else
			{
				ptz_info.mt_rotat = MOTOR_CORATATION;
			}
		}
	}
	
	if((dir == 1) || (dir == 2))
	{
		MotorMove(0,&ptz_info);
	}
	else
	{
		MotorMove(1,&ptz_info);
	}

}


void CPtz::DoMotorStop()
{
	MotorStop();
}

int CPtz::GetPosition(PtzPosition_s *pstPtzPosition)
{
	int ret = -1;
	struct motor_get_status stMotorMessage_sleep;
	ret = MotorGetStatus(&stMotorMessage_sleep);
	if( 0 == ret )
	{
		pstPtzPosition->x = stMotorMessage_sleep.fhx_cur_step;
		pstPtzPosition->y = stMotorMessage_sleep.fhy_cur_step;
	}
	return 0;
}

int CPtz::GotoPosition(int x,int y)
{
	int ret = -1;
	struct motor_config ptz_info_preset;
	struct motor_get_status stMotorMessage_preset;
	
	ret = MotorGetStatus(&stMotorMessage_preset);
	if( 0 == ret )
	{
		iAllowMotorTrackTime = time(NULL) + 10;
		ptz_info_preset.en = MOTOR_ENABLE;
		ptz_info_preset.nsec = ProductCof_g.run_speed;
		if(x - stMotorMessage_preset.fhx_cur_step<0)
		{
			ptz_info_preset.mt_rotat = MOTOR_CORATATION;
		}
		else
		{
			ptz_info_preset.mt_rotat = MOTOR_REVERSE;
		}
		ptz_info_preset.step_num = abs(x - stMotorMessage_preset.fhx_cur_step);
		MotorMove(0,&ptz_info_preset);
		if(y - stMotorMessage_preset.fhy_cur_step<0)
		{
			ptz_info_preset.mt_rotat = MOTOR_CORATATION;
		}
		else
		{
			ptz_info_preset.mt_rotat = MOTOR_REVERSE;
		}
		ptz_info_preset.step_num = abs(y - stMotorMessage_preset.fhy_cur_step);
		MotorMove(1,&ptz_info_preset);
		s_bMotorUsed = MOTOR_IS_MANUAL;
	}
	return 0;
}

bool CPtz::GetPtzstatus()
{
	return m_Ptzstatus;
}

int CPtz::GotoHome()
{
	return 0;
}

void CPtz::DoAutoTest()
{
	m_bDoAutoTest = true;
}

//在老化测试和PCBA测试时使用
void CPtz::DoPtzTest(bool enable)
{
	m_bDoPtzTest = enable;
}


