#ifndef __PAL_MISC_H__
#define __PAL_MISC_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <linux/ioctl.h>

int MiscInit();
int MiscRelease();

/// ϵͳ��ť
int SystemRest();
/// ��ȡadc��������ת���ĵ�ѹֵ ֵԽ�󻷾���Խ��
int SystemReadAdc();
/// ����ledָʾ��
int SystemSetLight_Power(int value);

int SystemSetLight_Link(int value);
int SystemSetLight_Epi(int value);

/// ����speak ����
int SystemAutioCtl(int val);
///���cds���� 0-��ҹ 1-����
int SystemPirDet(void);

//value= 0;���� ircut �ر�
//value = 1;��ҹ ircut ��
//io0 ircut +
//io1 ircut -
//domain 0:DOMAIN 1:mcu
int SystemSetIrcut(int value);

int SystemSetInfraredLamp(int val);//�����

/// ����speak ���� 	1:�� 	0:��
// PA08
int SystemWifiPwrCtl(int val);
int SystemSetPirSen(int val,int option);
int SystemSetColor(int value);
int SystemSetDay(void);
int SystemSetNight(void);
int SystemGetAE();

int SystemPirInit(void);
int SystemPirDeInit(void);


// ���ð׹��
// PB18
typedef unsigned int						__u32;

int SystemSetIncandescentLamp(int value);

// #define PWM_IOCTL_MAGIC				'p'
// #define ENABLE_PWM					_IOWR(PWM_IOCTL_MAGIC, 0, __u32)
// #define DISABLE_PWM					_IOWR(PWM_IOCTL_MAGIC, 1, __u32)

// #define SET_PWM_DUTY_CYCLE			_IOWR(PWM_IOCTL_MAGIC, 2, __u32)
// #define GET_PWM_DUTY_CYCLE			_IOWR(PWM_IOCTL_MAGIC, 3, __u32)
// #define SET_PWM_DUTY_CYCLE_PERCENT	_IOWR(PWM_IOCTL_MAGIC, 4, __u32)
// #define SET_PWM_ENABLE				_IOWR(PWM_IOCTL_MAGIC, 5, __u32)
// #define ENABLE_MUL_PWM				_IOWR(PWM_IOCTL_MAGIC, 6, __u32)
// #define ENABLE_FINSHALL_INTR		_IOWR(PWM_IOCTL_MAGIC, 7, __u32)
// #define ENABLE_FINSHONCE_INTR		_IOWR(PWM_IOCTL_MAGIC, 8, __u32)
// #define DISABLE_FINSHALL_INTR		_IOWR(PWM_IOCTL_MAGIC, 9, __u32)
// #define DISABLE_FINSHONCE_INTR		_IOWR(PWM_IOCTL_MAGIC, 10, __u32)
// #define WAIT_PWM_FINSHALL			_IOWR(PWM_IOCTL_MAGIC, 12, __u32)

// struct fh_pwm_config
// {
// 	unsigned int period_ns;
// 	unsigned int duty_ns;
// 	unsigned int pulses;
// #define FH_PWM_STOPLVL_LOW (0x0)
// #define FH_PWM_STOPLVL_HIGH (0x3)
// #define FH_PWM_STOPLVL_KEEP (0x1)
// #define FH_PWM_STOPCTRL_ATONCE (0x10)
// #define FH_PWM_STOPCTRL_AFTERFINISH (0x00)
// 	unsigned int stop;
// 	unsigned int delay_ns;
// 	unsigned int phase_ns;
// 	unsigned int percent;
// 	unsigned int finish_once;
// 	unsigned int finish_all;
// 	unsigned int shadow_enable;
// };

// struct fh_pwm_status
// {
// 	unsigned int done_cnt;
// 	unsigned int total_cnt;
// 	unsigned int busy;
// 	unsigned int error;
// };

// struct fh_pwm_chip_data
// {
// 	int id;
// 	struct fh_pwm_config config;
// 	struct fh_pwm_status status;
// };


int SystemSetIncandescentLampSwitch(int enable);
int SystemSetIncandescentLampLumi(int duty,int period_white);
int SystemUnInitIncandescentLamp();
int SystemInitIncandescentLamp();

int SystemSetIncandescentLampSwitch_yellow(int enable);
int SystemSetIncandescentLampLumi_yellow(int duty,int period_white);
int SystemUnInitIncandescentLamp_yellow();
int SystemInitIncandescentLamp_yellow();



#ifdef __cplusplus
}
#endif

#endif
