#include "PAL/MW_Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "PAL/Motor.h"


#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>
#include <poll.h>


static int fmoto = -1;
static int fmoto1 = -1;
int MotoInit()
{
	MSG("MOTO test!!!\n");
	int ret;
	int fd;
	struct motor_gpio motor_io;
	motor_io.gpio_a = 51;
    motor_io.gpio_b = 50;
    motor_io.gpio_c = 49;
    motor_io.gpio_d = 48;
	
	fd = open("/dev/motor-0", O_RDWR | O_NONBLOCK);
	if(fd < 0)
	{
		EMSG("open error!\n");
		return -1;
	}
	
	if (0 > ioctl(fd, MOTOR_SET_GPIO, &motor_io))//上电后只需调用一次，多次调用也可以
            EMSG("motor-0 MOTOR_SET_GPIO fail!\n");
		
	fmoto = fd;

	
	motor_io.gpio_a = 55;
	motor_io.gpio_b = 54;
	motor_io.gpio_c = 53;
	motor_io.gpio_d = 52;

	fd = open("/dev/motor-1", O_RDWR | O_NONBLOCK);
	if(fd < 0)
	{
		EMSG("open error!\n");
		return -1;
	}
	
	if (0 > ioctl(fd, MOTOR_SET_GPIO, &motor_io))//上电后只需调用一次，多次调用也可以
            EMSG("motor-1 MOTOR_SET_GPIO fail!\n");

	fmoto1 = fd;
	
	return 0;
}

int MotoUinit()
{
	if (fmoto)
	{
		close(fmoto);
		fmoto = -1;
	}

	if (fmoto1)
	{
		close(fmoto1);
		fmoto1 = -1;
	}
	
	return 0;
}

int MotoReset()
{
	int ret;
	struct motor_reset_data stMotorResetData = {0};

	if( fmoto < 0 )
		return -1;

	ret = ioctl(fmoto, MOTOR_RESET, &stMotorResetData);
	MSG("x_max_steps=%d,y_max_steps=%d,x_cur_step=%d,y_cur_step=%d\n",
			stMotorResetData.xmax_steps,stMotorResetData.ymax_steps,
			stMotorResetData.x_cur_step,stMotorResetData.y_cur_step);

	return ret;
}

int MotorMove(int motor_chan,struct motor_config *pstMotorStep)
{
	if(motor_chan == 0)
	{
		if(fmoto < 0)
			return -1;
		if (0 > ioctl(fmoto, MOTOR_SET_DRV_CONFIG, pstMotorStep))
            EMSG("motor-0 move MOTOR_SET_DRV_CONFIG fail!\n");
	}
	else
	{
		if( fmoto1 < 0 )
			return -1;

		if (0 > ioctl(fmoto1, MOTOR_SET_DRV_CONFIG, pstMotorStep))
            EMSG("motor-1 move MOTOR_SET_DRV_CONFIG fail!\n");
	}
}

void MotorStop()
{
	struct motor_config motor_cfg;
	motor_cfg.en = 1;
	motor_cfg.mt_rotat = 0;
	motor_cfg.nsec = 1000000;
	motor_cfg.step_num = 0;

	if (0 > ioctl(fmoto, MOTOR_SET_DRV_CONFIG, &motor_cfg))
            EMSG("motor-0 stop MOTOR_SET_DRV_CONFIG fail!\n");
	if (0 > ioctl(fmoto1, MOTOR_SET_DRV_CONFIG, &motor_cfg))
            EMSG("motor-1 stop MOTOR_SET_DRV_CONFIG fail!\n");
}


//设置云台速度 	100-900 值越大速度越快
int MotorSetSpeed(int speed)
{
	;
}

int MotorGetStatus(struct motor_get_status *pstMotorMessage) 
{
	if( fmoto < 0 )
		return -1;

	if(ioctl(fmoto, MOTOR_GET_STATUS, pstMotorMessage) < 0)
	{
		EMSG("ioctl MOTOR_GET_STATUS error!\n");
		return -1;
	}
	else
	{
//		printf("ioctl MOTOR_GET_STATUS ok\n");
		return 0;
	}
}

int MotorSetStep(struct motor_set_step step)
{
	if(fmoto < 0)
		return -1;
	if (0 > ioctl(fmoto, MOTOR_SET_STEP, &step))
            EMSG("motor-0 MOTOR_SET_STEP fail!\n");
}

