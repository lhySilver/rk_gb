#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/delay.h>
#include <linux/printk.h>

#include <asm/uaccess.h>
#include <asm/io.h>


#include <linux/hrtimer.h>
#include "motor_gpio.h"

struct motor_chip *motor_dev;

static int step_compare_countmax0 = 0;
static int step_compare_countmax1 = 0;
static unsigned int check_speed = 1300000;

static int step_compare_countcur0 = 0;
static int step_compare_countcur1 = 0;

static int normal_stop = 0;
static int normal_stop0 = 1;
static int normal_stop1 = 1;

static int motor_set_gpio(int motor, struct motor_gpio *drvio)
{
    int ret = -1;

    motor_dev->mtpara[motor].drvio = *drvio;

    if ( !motor_dev->mtpara[motor].gpio_flag ) {
        if(gpio_request(drvio->gpio_a, "MOTOR_GPIO")){
            pr_err("error MOTOR_GPIO");
            return ret;
        }
        if(gpio_request(drvio->gpio_b, "MOTOR_GPIO")){
            pr_err("error MOTOR_GPIO");
            return ret;
        }
        if(gpio_request(drvio->gpio_c, "MOTOR_GPIO")){
            pr_err("error MOTOR_GPIO");
            return ret;
        }
        if(gpio_request(drvio->gpio_d, "MOTOR_GPIO")){
            pr_err("error MOTOR_GPIO");
            return ret;
        }

        gpio_direction_output(drvio->gpio_a, 0);
        gpio_direction_output(drvio->gpio_b, 0);
        gpio_direction_output(drvio->gpio_c, 0);
        gpio_direction_output(drvio->gpio_d, 0);
    } else {
        pr_err("free gpio firstly");
        return ret;
    }

    motor_dev->mtpara[motor].gpio_flag = 1;

    return 0;

}

static int motor_clear_gpio(int motor)
{
    struct motor_gpio *drvio;

    if (motor >= MOTOR_DEVICE_NUM) {
        return -EINVAL;
    }

    drvio = &motor_dev->mtpara[motor].drvio;

    if (motor_dev->mtpara[motor].gpio_flag) {
        gpio_free(drvio->gpio_a);
        gpio_free(drvio->gpio_b);
        gpio_free(drvio->gpio_c);
        gpio_free(drvio->gpio_d);
        
        motor_dev->mtpara[motor].gpio_flag = 0;
    }

    return 0;
}

static int step_count_0 = 0;
static char beat_count_0 = 0;
static enum hrtimer_restart motor0_drive_func(struct hrtimer *timer)
{
    step_count_0--;
     if ( motor_dev->mtpara[0].drvconfig.mt_rotat )
	 {
		 step_compare_countcur0++;
		 if(step_compare_countcur0 > step_compare_countmax0)
		 {
			 step_compare_countcur0 = step_compare_countmax0;
			 //step_count_0 = 0;
		 }
	 }
	 else
	 {
		 step_compare_countcur0--;
		 if(step_compare_countcur0 < 0)
		 {
			 step_compare_countcur0 = 0;
			 //step_count_0 = 0;
		 }
	 }
	
    if ( !step_count_0 )
    {
        motor_dev->mtpara[0].drvconfig.en = 0;
        gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 0);
        gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 0);
        gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 0);
        gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 0);
			
		if ( motor_dev->mtpara[0].drvconfig.mt_rotat )
		{
			step_compare_countcur0--;
		}
		else
		{
			step_compare_countcur0++;
		}
		
		normal_stop0 = 1;
        return HRTIMER_NORESTART;
    }
    
	normal_stop0 = 0;
    if ( motor_dev->mtpara[0].drvconfig.mt_rotat )
    {
        beat_count_0 = (beat_count_0 + 1) > 8 ? 1: (beat_count_0 + 1);
    }
    else
    {
        beat_count_0 = (beat_count_0 - 1) < 1 ? 8: (beat_count_0 - 1);
    }
    switch ( beat_count_0 )
    {
        case 1:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 0);
            break;
        case 2:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 0);
            break;
        case 3:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 0);
            break;
        case 4:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 0);
            break;
        case 5:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 0);
            break;
        case 6:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 1);
            break;
        case 7:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 1);
            break;
        case 8:
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_a, 1);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[0].drvio.gpio_d, 1);
            break;
        default:
            pr_err("never happen");
            break;
    }

    hrtimer_forward_now(timer, ktime_set(0, motor_dev->mtpara[0].drvconfig.nsec));
    return HRTIMER_RESTART;
}

static int beat_count_1 = 0;
static int step_count_1 = 0;
static enum hrtimer_restart motor1_drive_func(struct hrtimer *timer)
{
    step_count_1--;
    
	if ( motor_dev->mtpara[1].drvconfig.mt_rotat )
	 {
		 step_compare_countcur1++;
		 if(step_compare_countcur1 > step_compare_countmax1)
		 {
			 step_compare_countcur1 = step_compare_countmax1;
			// step_count_1 = 0;
		 }
	 }
	 else
	 {
		 step_compare_countcur1--;
		 if(step_compare_countcur1 < 0)
		 {
			 step_compare_countcur1 = 0;
			 //step_count_1 = 0;
		 }
	 }
	
    if ( !step_count_1 )
    {
        motor_dev->mtpara[1].drvconfig.en = 0;
        gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 0);
        gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 0);
        gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 0);
        gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 0);
		
		
		if ( motor_dev->mtpara[1].drvconfig.mt_rotat )
		{
			step_compare_countcur1--;
		}
		else
		{
			step_compare_countcur1++;
		}
		
		normal_stop1 = 1;
        return HRTIMER_NORESTART;
    }

    normal_stop1 = 0;
    if ( motor_dev->mtpara[1].drvconfig.mt_rotat )
    {
        beat_count_1 = (beat_count_1 + 1) > 8 ? 1: (beat_count_1 + 1);
    }
    else
    {
        beat_count_1 = (beat_count_1 - 1) < 1 ? 8: (beat_count_1 - 1);
    }
    
    switch ( beat_count_1 )
    {
        case 1:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 0);
            break;
        case 2:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 0);
            break;
        case 3:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 0);
            break;
        case 4:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 0);
            break;
        case 5:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 0);
            break;
        case 6:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 1);
            break;
        case 7:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 1);
            break;
        case 8:
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_a, 1);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_b, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_c, 0);
            gpio_set_value(motor_dev->mtpara[1].drvio.gpio_d, 1);
            break;
        default:
            pr_err("never happen");
            break;
    }

    hrtimer_forward_now(timer, ktime_set(0, motor_dev->mtpara[1].drvconfig.nsec));
    return HRTIMER_RESTART;
}


static void motor_drv_start(int motor)//当定时器未跑完时再设置参数并start定时经测试会重置
{
    prdebug("-----------motor_gpio_start------------");
    
    if ( motor_dev->mtpara[motor].drvconfig.en == MOTOR_DISABLE ) {
        pr_err("please enable motor firstly");
        return;
    }
    
    if (motor == 0)
    {
        step_count_0 = motor_dev->mtpara[0].drvconfig.step_num + 1;//为了电机最后一步能够生效
        motor_dev->mtpara[0].mtimer.function = motor0_drive_func;
    }
    else if (motor == 1)
    {
        step_count_1 = motor_dev->mtpara[1].drvconfig.step_num + 1;//为了电机最后一步能够生效
        motor_dev->mtpara[1].mtimer.function = motor1_drive_func;
    }
    hrtimer_start(&motor_dev->mtpara[motor].mtimer, ktime_set(0, 0), HRTIMER_MODE_REL);
}

int motor_drv_open (struct inode * inode, struct file *filp)
{
    int minor;
    int major;


    prdebug("--------------%s----------------\n",__func__);

    minor = iminor(inode);
    major = imajor(inode);

    filp->private_data = (void *)minor;

    return 0;
}

ssize_t motor_drv_read (struct file *filp, char __user *userbuf, size_t count, loff_t *fpos)
{
    int ret, rlen;
    char kbuf[100];
    struct inode *inode;
    int minor;
    
    prdebug("--------------%s----------------\n", __func__);
    //获取inode对象
    inode = filp->f_path.dentry->d_inode;
 
    //获取次设备号
    minor= MINOR(inode->i_rdev);

    if (*fpos)
       return 0;

    rlen  = snprintf(kbuf, 100, "%d,%d,%d,%d,%d,%d,%d,%d\n", \
                       motor_dev->mtpara[minor].drvio.gpio_a,\
                       motor_dev->mtpara[minor].drvio.gpio_b,\
                       motor_dev->mtpara[minor].drvio.gpio_c,\
                       motor_dev->mtpara[minor].drvio.gpio_d,\
                       motor_dev->mtpara[minor].drvconfig.en, \
                       motor_dev->mtpara[minor].drvconfig.mt_rotat, \
                       motor_dev->mtpara[minor].drvconfig.step_num, \
                       motor_dev->mtpara[minor].drvconfig.nsec);

    if (clear_user(userbuf, count)) {
        pr_err("clear error\n");
        return -EIO;
    }
    
    prdebug("motor-%d:\n"
                "\t\tgpio: %d %d %d %d\n"
                "\t\tenable: %d\n"
                "\t\treverse: %d\n"
                "\t\tnumofstep: %d\n"
                "\t\ttimeofpulse: %d\n",
                minor, motor_dev->mtpara[minor].drvio.gpio_a, \
                       motor_dev->mtpara[minor].drvio.gpio_b, \
                       motor_dev->mtpara[minor].drvio.gpio_c, \
                       motor_dev->mtpara[minor].drvio.gpio_d, \
                       motor_dev->mtpara[minor].drvconfig.en, \
                       motor_dev->mtpara[minor].drvconfig.mt_rotat, \
                       motor_dev->mtpara[minor].drvconfig.step_num, \
                       motor_dev->mtpara[minor].drvconfig.nsec);

    ret = copy_to_user(userbuf, kbuf, rlen);//无意义
    if(ret > 0){
        pr_err("error copy_to_user\n");
        return -EFAULT;
    }
    *fpos += rlen;

    return rlen;
}

static void del_char(char *str, char ch)
{
	char *p = str;
	char *q = str;
	while (*q) {
		if (*q != ch)
			*p++ = *q;
		q++;
	}
	*p = '\0';
}

ssize_t motor_drv_write (struct file *filp, const char __user *userbuf, size_t count, loff_t *fpos)
{
    int ret;
    struct inode *inode;
    int minor;
    char kbuf[64] = {0};
    int i;
	char *cur = kbuf;
    char *const delim = ",";
    char *param_str[5];
    unsigned int param[5];
    struct motor_gpio motor_io;

    prdebug("--------------%s----------------\n", __func__);

    //获取inode对象
    inode = filp->f_path.dentry->d_inode;
 
    //获取次设备号
    minor = MINOR(inode->i_rdev);

    count = (count > 64) ? 64 : count;

    ret = copy_from_user(kbuf, userbuf, count);
    if (ret > 0){
        pr_err("error copy_from_user\n");
        return -EFAULT;
    }

    for (i = 0; i < 5; i++) {
        param_str[i] = strsep(&cur, delim);
        if (!param_str[i]) {
            pr_err("%s: ERROR: parameter[%d] is empty\n",__func__, i);
            pr_err("cmd, enable, reverse, num of step(pulse), time of pulse(ns)\n");
            pr_err("eg. echo '0,1,1,100,1000000' > /dev/motor-%d\n", minor);
            return -EINVAL;
        } else {
            del_char(param_str[i], ' ');
            del_char(param_str[i], '\n');
            param[i] = (unsigned int)simple_strtoul(param_str[i], NULL, 10);
            if (param[i] < 0) {
                pr_err("%s: ERROR: parameter[%d] is incorrect\n", __func__, i);
                pr_err("cmd, enable, reverse, num of step(pulse), time of pulse(ns)\n");
                pr_err("eg. echo '0,1,1,100,1000000' > /dev/motor-%d\n", minor);
                return -EINVAL;
            }
        }
    }

    if (!motor_dev)
    {
        pr_err("motor_dev isn't alloc\n");
        return -EINVAL;
    }

    if (param[0] == 22) {//set gpio
        if(motor_dev->mtpara[minor].gpio_flag == 1){
            if (motor_clear_gpio(minor) < 0) {
                pr_err("set motor gpio fail\n");
                return -EINVAL;
            }
        }
        
        motor_io.gpio_a = param[1];
        motor_io.gpio_b = param[2];
        motor_io.gpio_c = param[3];
        motor_io.gpio_d = param[4];
        
        if (motor_set_gpio(minor, &motor_io) < 0) {
            pr_err("set motor gpio fail\n");
            return -EINVAL;
        }
    } else if (param[0] == 0) {//set motor drive config
        if (motor_dev->mtpara[minor].gpio_flag == 0)
        {
            pr_err("please set motor gpio firstly\n");
            pr_err("eg. echo '22,26,27,28,4' > /dev/motor-%d\n", minor);
            return -EINVAL;
        }

        motor_dev->mtpara[minor].drvconfig.en        = param[1];
        motor_dev->mtpara[minor].drvconfig.mt_rotat  = param[2];
        motor_dev->mtpara[minor].drvconfig.step_num  = param[3];
        motor_dev->mtpara[minor].drvconfig.nsec      = param[4];

        if (param[1]) {
            motor_drv_start(minor);
        }
    } else {
        pr_err("%s: ERROR: parameter[0] is incorrect\n", __func__);
        pr_err("cmd, enable, reverse, num of step(pulse), time of pulse(ns)\n");
        pr_err("eg. echo '0,1,1,100,1000000' > /dev/motor-%d\n", minor);
        return -EINVAL;
    }

    return count;
}

int motor_drv_close (struct inode *inode, struct file *filp)
{

    prdebug("--------------%s----------------\n",__func__);

    return 0;
}

static long _check_motor_config(int motor, struct motor_config * mt_config)
{
    if (motor >= MOTOR_DEVICE_NUM || motor < 0)
    {
        pr_err("the motor is non-existent\n");
        return -1;
    }
    if (mt_config->nsec > MOTOR_DELAY_TIME_MAX || MOTOR_DELAY_TIME_MIN > mt_config->nsec)
    {
        pr_err("set pulse width is out of range\n");
        return -1;
    }
    if (mt_config->mt_rotat < MOTOR_CORATATION || mt_config->mt_rotat > MOTOR_REVERSE)
    {
        pr_err("rotation param is illegal\n");
        return -1;
    }
    if (mt_config->step_num < 0)
    {
        pr_err("step_num shouldn't be less than 0\n");
        return -1;
    }
    return 0;
}

static long motor_drv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    //获取inode对象
    struct inode *inode = filp->f_path.dentry->d_inode;
 
    //获取次设备号
    int minor = MINOR(inode->i_rdev);

    /* 检查设备类型 */
    if (_IOC_TYPE(cmd) != MOTOR_MAGIC) {
        pr_err("[%s] command type [%c] error!\n", __func__, _IOC_TYPE(cmd));
        return -ENOTTY; 
    }

    /* 检查序数 */
    if (_IOC_NR(cmd) > 0xa8 || _IOC_NR(cmd) < 0xa1) { 
        pr_err("[%s] command numer [%d] exceeded!\n", __func__, _IOC_NR(cmd));
        return -ENOTTY;
    }    

    /* 检查访问模式 */
    if (_IOC_DIR(cmd) & _IOC_READ)
        ret= !access_ok((void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        ret= !access_ok((void __user *)arg, _IOC_SIZE(cmd));
    if (ret) {
        pr_err("ioc dir err\n");
        return -EFAULT;
    }

    switch (cmd)
    {
        case MOTOR_SET_GPIO:
        {
            struct motor_gpio *motor_io = (struct motor_gpio *)kmalloc(sizeof(struct motor_gpio), GFP_KERNEL);
            if (copy_from_user(motor_io, (void *)arg, sizeof(struct motor_gpio)))
            {
                pr_err("copy_from_user fail\n");
                kfree(motor_io);
                return -EINVAL;
            }

            if(motor_dev->mtpara[minor].gpio_flag == 1){
                if (motor_clear_gpio(minor) < 0) {
                    pr_err("clear motor gpio fail\n");
                    kfree(motor_io);
                    return -EINVAL;
                }
            }
            if (motor_set_gpio(minor, motor_io) < 0) {
                pr_err("set motor gpio fail\n");
                kfree(motor_io);
                return -EINVAL;
            }
            kfree(motor_io);
            break;
        }
        case MOTOR_GET_STATUS:
        {
			struct fhmotor_status status;
			if((normal_stop0 == 1) && (normal_stop1 == 1))
			{
				normal_stop = 1;
			}
			else
			{
				normal_stop = 0;
			}
			status.stop_enable = normal_stop;
			status.fh_cur_step0 = step_compare_countcur0;
			status.fh_cur_step1 = step_compare_countcur1;
			if (copy_to_user((void *)arg,&status,sizeof(struct fhmotor_status)))
            {
                return -EINVAL;
            }
            break;
        }
        case MOTOR_SET_DRV_CONFIG:
        {
            struct motor_config *motor_drv = (struct motor_config *)kmalloc(sizeof(struct motor_config), GFP_KERNEL);
            if (copy_from_user(motor_drv, (void *)arg, sizeof(struct motor_config)))
            {
                kfree(motor_drv);
                return -EINVAL;
            }

            if (_check_motor_config(minor, motor_drv) < 0)
            {
                kfree(motor_drv);
                return -EINVAL;
            }
            motor_dev->mtpara[minor].drvconfig = *motor_drv;
            if (motor_drv->en && motor_dev->mtpara[minor].gpio_flag)
                motor_drv_start(minor);
            kfree(motor_drv);
            break;
        }
        case MOTOR_GET_DRV_CONFIG:
        {
            break;
        }
		case MOTOR_RESET:
		{
			struct fhmotor_reset_data *motor_data = (struct fhmotor_reset_data *)kmalloc(sizeof(struct fhmotor_reset_data), GFP_KERNEL);
			struct motor_config *motor_drv = (struct motor_config *)kmalloc(sizeof(struct motor_config), GFP_KERNEL);
			
			motor_drv->en = MOTOR_ENABLE;
			motor_drv->mt_rotat = MOTOR_REVERSE;
			motor_drv->nsec = check_speed;
			motor_drv->step_num = step_compare_countmax0;
			motor_dev->mtpara[0].drvconfig = *motor_drv;
			if (motor_drv->en && motor_dev->mtpara[0].gpio_flag && motor_drv->step_num)
			{
				motor_drv_start(0);
			}
			msleep(50);
			while(!normal_stop0)
			{
				msleep(10);
			}
			
			normal_stop0 = 0;
			motor_drv->en = MOTOR_ENABLE;
			motor_drv->mt_rotat = MOTOR_CORATATION;
			motor_drv->nsec = check_speed;
			motor_drv->step_num = step_compare_countmax0/2;
			motor_dev->mtpara[0].drvconfig = *motor_drv;
			if (motor_drv->en && motor_dev->mtpara[0].gpio_flag && motor_drv->step_num)
			{
				motor_drv_start(0);
			}
			msleep(50);
			while(!normal_stop0)
			{
				msleep(10);
			}
			normal_stop1 = 0;
			motor_drv->en = MOTOR_ENABLE;
			motor_drv->mt_rotat = MOTOR_REVERSE;
			motor_drv->nsec = check_speed;
			motor_drv->step_num = step_compare_countmax1;
			motor_dev->mtpara[1].drvconfig = *motor_drv;
			if (motor_drv->en && motor_dev->mtpara[1].gpio_flag && motor_drv->step_num)
			{
				motor_drv_start(1);
			}
			msleep(50);
			while(!normal_stop1)
			{
				msleep(10);
			}
			normal_stop1 = 0;
			motor_drv->en = MOTOR_ENABLE;
			motor_drv->mt_rotat = MOTOR_CORATATION;
			motor_drv->nsec = check_speed;
			motor_drv->step_num = step_compare_countmax1/2;
			motor_dev->mtpara[1].drvconfig = *motor_drv;
			if (motor_drv->en && motor_dev->mtpara[1].gpio_flag && motor_drv->step_num)
			{
				motor_drv_start(1);
			}
			msleep(50);
			while(!normal_stop1)
			{
				msleep(10);
			}
			
			motor_data->fhxmax_steps = step_compare_countmax0;
			motor_data->fhymax_steps = step_compare_countmax1;
			motor_data->fhx_cur_step = step_compare_countmax0/2;
			motor_data->fhy_cur_step = step_compare_countmax1/2;
			step_compare_countcur0 = step_compare_countmax0/2;
			step_compare_countcur1 = step_compare_countmax1/2;
			if (copy_to_user((void *)arg,motor_data,sizeof(struct fhmotor_reset_data)))
            {
                kfree(motor_drv);
				kfree(motor_data);
                return -EINVAL;
            }
			kfree(motor_drv);
			kfree(motor_data);
			break;
		}
		case MOTOR_SET_STEP:
		{
			struct fhmotor_set_step *motor_step= (struct fhmotor_set_step *)kmalloc(sizeof(struct fhmotor_set_step), GFP_KERNEL);
			if (copy_from_user(motor_step, (void *)arg, sizeof(struct fhmotor_set_step)))
            {
                kfree(motor_step);
                return -EINVAL;
            }
			
			step_compare_countmax0 = motor_step->fhxmax_steps;
			step_compare_countmax1 = motor_step->fhymax_steps;
			check_speed = motor_step->check_speed;
			kfree(motor_step);
			break;
		}
		case MOTOR_SET_CUR_STEP:
		{
			struct fhmotor_set_cur_step *motor_step= (struct fhmotor_set_cur_step *)kmalloc(sizeof(struct fhmotor_set_cur_step), GFP_KERNEL);
			if (copy_from_user(motor_step, (void *)arg, sizeof(struct fhmotor_set_cur_step)))
            {
                kfree(motor_step);
                return -EINVAL;
            }
			
			step_compare_countcur0 = motor_step->fhxcur_steps;
			step_compare_countcur1 = motor_step->fhycur_steps;
			kfree(motor_step);
			break;
		}
        default: 
            pr_err("unbeknown cmd\n");
            return -EINVAL;
    }

    return 0;
}

const struct file_operations motor_fops = {
    .open = motor_drv_open,
    .write = motor_drv_write,
    .read = motor_drv_read,
    .release = motor_drv_close,
    .unlocked_ioctl = motor_drv_ioctl,
};

static int __init motor_drv_init(void)
{
    int ret;
    int j;

    prdebug("--------------%s----------------\n",__func__);

    motor_dev = kzalloc(sizeof(struct motor_chip), GFP_KERNEL);
    if(motor_dev == NULL){
        pr_err("kzalloc error\n");
        return -ENOMEM;
    }

    //动态的申请设备号
    ret = alloc_chrdev_region(&motor_dev->devno, 0, 2, MOTOR_DEVICE_NAME);
    if(ret != 0){
        pr_err("error alloc_chrdev_region\n");
        goto err_free;
    }

    //分配cdev对象
    motor_dev->cdev = cdev_alloc();
    cdev_init(motor_dev->cdev, &motor_fops);
    cdev_add(motor_dev->cdev, motor_dev->devno, 2);

    //自动创建设备节点
    motor_dev->cls = class_create(THIS_MODULE,MOTOR_CLASS_NAME);
    if(IS_ERR(motor_dev->cls)){
        pr_err("error class_create\n");
        ret = PTR_ERR(motor_dev->cls);
        goto err_unregister;
    }

    for (j=0;j<MOTOR_DEVICE_NUM;j++) {
        motor_dev->dev[j] = device_create(motor_dev->cls, NULL, MKDEV(MAJOR(motor_dev->devno), j), NULL, "motor-%d", j);
        if(IS_ERR(motor_dev->dev[j])){
            pr_err("error device_create\n");
            ret = PTR_ERR(motor_dev);
            goto err_class_error;
         }
    }

    for (j=0;j<MOTOR_DEVICE_NUM;j++) {
        hrtimer_init(&motor_dev->mtpara[j].mtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        motor_dev->mtpara[j].gpio_flag = 0;
    }

    return 0;

err_class_error:
    class_destroy(motor_dev->cls);

err_unregister:
    cdev_del(motor_dev->cdev);
    unregister_chrdev_region(motor_dev->devno,2);

err_free:
    kfree(motor_dev);
    return ret;


}

static void __exit motor_drv_exit(void)
{
    int j;
    prdebug("--------------%s----------------\n", __func__);
    
    for (j=0;j<MOTOR_DEVICE_NUM;j++) {
        motor_clear_gpio(j);
        hrtimer_cancel(&motor_dev->mtpara[j].mtimer);
        device_destroy(motor_dev->cls, MKDEV(MAJOR(motor_dev->devno), j));
    }
    class_destroy(motor_dev->cls);
    cdev_del(motor_dev->cdev);
    unregister_chrdev_region(motor_dev->devno, 2);
    kfree(motor_dev);

}

module_init(motor_drv_init);
module_exit(motor_drv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangyx@fullhan.com");


