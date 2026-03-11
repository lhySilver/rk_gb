#ifndef __PAL_MOTOR_H__
#define __PAL_MOTOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MOTOR_MAGIC         'm'

/* ioctl cmd */
//#define MOTOR_STOP		0x1
//#define MOTOR_RESET		0x2
//#define MOTOR_MOVE		0x3
//#define MOTOR_GET_STATUS	0x4
//#define MOTOR_SPEED		0x5
//#define MOTOR_GOBACK	0x6
//#define MOTOR_CRUISE	0x7

enum motor_status {
	MOTOR_IS_RUNNING=0,
	MOTOR_IS_STOP=1
};

struct motor_message {
	int x;
	int y;
	enum motor_status status;
	int speed;
};
enum motor_used {
	MOTOR_IS_MANUAL,
	MOTOR_IS_AUTO
};

struct motors_steps{
	int x;
	int y;
};

struct motor_move_st {
	int motor_directional;
	int motor_move_steps;
	int motor_move_speed;
};
struct motor_status_st {
	int directional_attr;
	int total_steps;
	int current_steps;
	int min_speed;
	int cur_speed;
	int max_speed;
	int move_is_min;
	int move_is_max;
};

struct motor_reset_data {
	unsigned int xmax_steps;
	unsigned int ymax_steps;
	unsigned int x_cur_step;
	unsigned int y_cur_step;
};

struct motor_set_step {
	unsigned int xmax_steps;
	unsigned int ymax_steps;
	unsigned int check_speed;
};

struct motor_gpio {
    int gpio_a;
    int gpio_b;
    int gpio_c;
    int gpio_d;
};
enum motor_enable {
    MOTOR_DISABLE = 0,
    MOTOR_ENABLE
};

enum motor_rotation {
    MOTOR_CORATATION = 0,
    MOTOR_REVERSE    = 1
};


struct motor_config {
    enum motor_enable en;
    enum motor_rotation mt_rotat;
    unsigned int nsec;//每步持续时间
    int step_num;
};
struct motor_get_status{
	unsigned int stop_enable;
	int fhx_cur_step;
	int fhy_cur_step;
};

#define MOTOR_SET_GPIO          _IOW(MOTOR_MAGIC, 0xa1, struct motor_gpio)
#define MOTOR_GET_STATUS        _IOR(MOTOR_MAGIC, 0xa2, struct motor_get_status)

#define MOTOR_SET_DRV_CONFIG    _IOW(MOTOR_MAGIC, 0xa3, struct motor_config)
#define MOTOR_RESET             _IOR(MOTOR_MAGIC, 0xa5, struct motor_reset_data)
#define MOTOR_SET_STEP          _IOW(MOTOR_MAGIC, 0xa6, struct motor_set_step)


int MotoInit();

int MotoUinit();

int MotoReset();

int MotorMove(int motor_chan,struct motor_config *pstMotorStep);

void MotorStop();


//设置云台速度 	100-900 值越大速度越快
int MotorSetSpeed(int speed);

int MotorGetStatus(struct motor_get_status *pstMotorMessage);
int MotorSetStep(struct motor_set_step step);


#ifdef __cplusplus
}
#endif

#endif //__PAL_MOTOR_H__

