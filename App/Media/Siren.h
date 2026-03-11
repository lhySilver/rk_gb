#ifndef __SIREN_MANAGER_H__
#define __SIREN_MANAGER_H__

#include "Common.h"

typedef enum{
	SirenOperation_CLOSE,
	SirenOperation_OPEN,
	SirenOperation_AUTO
} SirenManualOperation_E;

typedef enum{
	SirenStatus_OPEN,
	SirenStatus_CLOSE,
} SirenStatus_E;


class CSiren : public CThread
{
public:
	CSiren();
	~CSiren();

	static CSiren* instance();
	
	bool Start();
	bool Stop();
	
	void ThreadProc();
	void onConfigSiren(const CConfigTable &table, int &ret);
	
	void SetSirenStatus(bool enable);
	bool GetSirenStatus();

	void SetMDStatus(int iMDStatus);
	void SetMDEnable(bool enable);

private:

	CMutex m_Mutex;
	static CSiren *m_instance;

	SirenConfig m_CCfgSiren;
	int m_iMD_status;	//警笛联动标志位 add on 2025.01.06 注释
	SirenManualOperation_E m_eOperation;//警笛的模式 关闭 手动 自动（1. 手动开启，时间到了之后，会被赋值为SirenOperation_CLOSE 2.手动关闭蜂鸣器时，也会设置为SirenOperation_CLOSE）add on 2025.01.06 注释
	SirenStatus_E m_eSirentStatus;	//警笛状态 add on 2025.01.06 注释
	bool m_bMD_Enable;	//警笛联动的基础条件 add on 2025.01.06 添加注释
};

#define g_Siren (*CSiren::instance())

#endif 		//__SIREN_MANAGER_H__
