#ifndef __MOTOR_GPIO__
#define __MOTOR_GPIO__

//#define DEBUG
#ifdef DEBUG
#define prdebug(fmt, ...) printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
#else
#define prdebug(fmt, ...)
#endif

#define MOTOR_DEVICE_NAME   "motor"
#define MOTOR_CLASS_NAME    "motor"    //sys/class/motor
#define MOTOR_DEVICE_NUM     2         //设备节点的编号最终生成节点的名字为/dev/motor-1

#define MOTOR_MAGIC         'm'
#define MOTOR_NR_MIN        0xa0
#define MOTOR_NR_MAX        0xb0

#define MOTOR_SET_GPIO          _IOW(MOTOR_MAGIC, 0xa1, struct motor_gpio)
#define MOTOR_GET_STATUS        _IOR(MOTOR_MAGIC, 0xa2, struct fhmotor_status)
#define MOTOR_SET_DRV_CONFIG    _IOW(MOTOR_MAGIC, 0xa3, struct motor_config)
#define MOTOR_GET_DRV_CONFIG    _IOR(MOTOR_MAGIC, 0xa4, struct motor_config)
#define MOTOR_RESET             _IOR(MOTOR_MAGIC, 0xa5, struct fhmotor_reset_data)
#define MOTOR_SET_STEP          _IOW(MOTOR_MAGIC, 0xa6, struct fhmotor_set_step)
#define MOTOR_SET_CUR_STEP      _IOW(MOTOR_MAGIC, 0xa7, struct fhmotor_set_cur_step)

#define MOTOR_DELAY_TIME_MIN (100000) //ns 10KHZ
#define MOTOR_DELAY_TIME_MAX (10000000) //ns 100hz


struct fhmotor_reset_data {
	unsigned int fhxmax_steps;
	unsigned int fhymax_steps;
	unsigned int fhx_cur_step;
	unsigned int fhy_cur_step;
};
struct fhmotor_set_step {
	unsigned int fhxmax_steps;
	unsigned int fhymax_steps;
	unsigned int check_speed;
};

struct fhmotor_set_cur_step {
	unsigned int fhxcur_steps;
	unsigned int fhycur_steps;
};

struct fhmotor_status{
	unsigned int stop_enable;
	int fh_cur_step0;
	int fh_cur_step1;
};

enum motor_enable {
    MOTOR_DISABLE = 0,
    MOTOR_ENABLE
};

enum motor_rotation {
    MOTOR_CORATATION = 0,
    MOTOR_REVERSE    = 1
};

struct motor_gpio {
    int gpio_a;
    int gpio_b;
    int gpio_c;
    int gpio_d;
};

struct motor_config {
    enum motor_enable en;
    enum motor_rotation mt_rotat;
    unsigned int nsec;
    int step_num;
};

struct motor_param {
    char gpio_flag;
    struct motor_gpio drvio;
    struct motor_config drvconfig;
    struct hrtimer mtimer;
};

//MOTOR的设备对象
struct motor_chip {
    dev_t devno;
    struct cdev *cdev;
    struct class *cls;
    struct device *dev[MOTOR_DEVICE_NUM];
    struct motor_param mtpara[MOTOR_DEVICE_NUM];
};

extern void fh_gpio_set(int gpio_id, int value);

#endif

