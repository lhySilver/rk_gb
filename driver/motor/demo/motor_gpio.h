#ifndef __MOTOR_GPIO__
#define __MOTOR_GPIO__

#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include "motor_gpio.h"

#define MOTOR_DEVICE_NAME   "motor"
#define MOTOR_CLASS_NAME    "motor"    //sys/class/motor
#define MOTOR_DEVICE_NUM     2         //设备节点的编号最终生成节点的名字为/dev/motor-1


#define MOTOR_MAGIC         'm'
#define MOTOR_NR_MIN        0xa0
#define MOTOR_NR_MAX        0xb0

#define MOTOR_SET_GPIO          _IOW(MOTOR_MAGIC, 0xa1, struct motor_gpio)
//#define MOTOR_GET_GPIO          _IOR(MOTOR_MAGIC, 0xa2, struct motor_gpio)
#define MOTOR_SET_DRV_CONFIG    _IOW(MOTOR_MAGIC, 0xa3, struct motor_config)
//#define MOTOR_GET_DRV_CONFIG    _IOR(MOTOR_MAGIC, 0xa4, struct motor_config)

#define MOTOR_DELAY_TIME_MIN (100000) //ns 10KHZ
#define MOTOR_DELAY_TIME_MAX (10000000) //ns 100hz

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
    unsigned int nsec;//每步持续时间
    int step_num;
};

#endif

