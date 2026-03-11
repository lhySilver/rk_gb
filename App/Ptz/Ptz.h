#ifndef __PTZ__
#define __PTZ__


#include "Infra/Thread.h"
#include "ExchangeAL/CommExchange.h"
#include "ExchangeAL/Exchange.h"
#include "Manager/ConfigManager.h"
#include "ExchangeAL/ExchangeKind.h"
#include "Infra/Timer.h"


/** PTZ转动方向 */
typedef enum {
    PTZ_LEFT = 1,
    PTZ_RIGHT,
    PTZ_UP,
    PTZ_DOWN,
} PTZDirection_e;

/** PTZ转动位置坐标信息 */
typedef struct {
    int x;        /**< PTZ的水平方向坐标，[-1.000000~1.000000]，左为负，右为正 */
    int y;       /**< PTZ的垂直方向坐标，[-1.000000~1.000000]，下为负，上为正 */
} PtzPosition_s;

class CPtz : public CThread
{
public:
	PATTERN_SINGLETON_DECLARE(CPtz);

	static int motion_track_ptz_op(int dx,int dy);
	
	/// 线程执行体
	void ThreadProc();

	/// 开启
	bool start();

	/// 停止
	bool stop();
	
	void DoMotorMove(PTZDirection_e dir, bool bStep = false);
	void DoMotorMove_private(PTZDirection_e dir);
	void DoMotorStop();
	int GetPosition(PtzPosition_s *pstPtzPosition);
	int GotoPosition(int x,int y);
	int Calibration();
	int GotoHome();
	bool GetPtzstatus();
	void DoAutoTest();
	void DoPtzTest(bool enable);
	void OnCheckMotorAuto(Param wParam);

private:
	PtzPosition m_stPtzPosition;
	bool m_bDoAutoTest;
	bool m_bDoPtzTest; 	//是否开启云台测试, 在老化测试和PCBA测试时使用
	bool m_Ptzstatus;  //移动追踪是否触发
	CTimer m_timerMotorauto;
};

#define g_PtzHandle (*CPtz::instance())



#endif// __PTZ__

