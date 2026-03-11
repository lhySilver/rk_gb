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
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>

#include "pir_gpio.h"

#define DEVICE_NAME  "pir"

static unsigned int ev_press = 0;
static DECLARE_WAIT_QUEUE_HEAD(step_waitq);//注册一个等待队列step_waitq

static int pir_run = 0;
static int pir_sen = 0;
static unsigned int pir_status = 0;
struct PIR_GPIO pir_gpio;
static struct hrtimer pir_mtimer;
static int pir_ready = 0;

static int set_pir_sen(int gpio_num, int sen)//设置pir灵敏度
{
	gpio_set_value(gpio_num, 1);
	msleep(250);//250ms
	
	switch(sen)
	{
		case 2:
		{
			gpio_set_value(gpio_num, 0);
			msleep(90);//90ms
		}
		break;
		case 1:
		{
			gpio_set_value(gpio_num, 0);
			msleep(70);//70ms
		}
		break;
		case 0:
		{
			gpio_set_value(gpio_num, 0);
			msleep(50);//50ms
		}
		break;
		default:
			break;
	}

	gpio_set_value(gpio_num, 1);
	msleep(50);//50ms
	gpio_set_value(gpio_num, 0);
	return 0;
}
static enum hrtimer_restart pir_drive_func(struct hrtimer *timer)
{
    
    //return HRTIMER_NORESTART;

    int value = gpio_get_value(pir_gpio.gpio_out);
    // pr_err("value%d\n",value);
    static int stime = 0;
    static int ptime = 0;
    static int find = 0;
    if ( (1==value) && (0==find) )
    {
        stime += 1;
        if (stime >= 50)
        {
            find = 1;
            pir_status = 0;
            pr_err("find start\n\n");
        }
    }
    else
    {
        stime = 0;
    }

    if (1==find)
    {
        ptime += 1;
        if (1==value)
        {
            if ( (ptime >= 25) && (ptime <= 50) )
            {
                //pr_err("..find pir 1 trager\n");
                pir_status |= 0x01;
            }
            if ( (ptime >= 75) && (ptime <= 100) )
            {
                //pr_err("**find pir 2 trager\n");
                pir_status |= 0x02;
            }
            if ( (ptime >= 125) && (ptime <= 150) )
            {
                //pr_err("xxfind pir 3 trager\n");
                pir_status |= 0x04;
            }
        }

        if (ptime > 150)
        {
            wake_up_interruptible(&step_waitq);   /* 唤醒休眠的进程 */
            ev_press = 1;
            ptime = 0;
            find = 0;
        }
    }

    hrtimer_forward_now(timer, ktime_set(0, 1000000));//1ms
    return HRTIMER_RESTART;
}

static int device_open(struct inode *inode, struct file *filp)
{
    // 设备打开时的操作
    return 0;
}

static int device_release(struct inode *inode, struct file *filp)
{
    // 设备关闭时的操作
    return 0;
}

static ssize_t device_read(struct file *filp, char *user_buf, size_t count, loff_t *f_pos)
{
    int ret;
    ret = copy_to_user(user_buf, &pir_status, sizeof(pir_status));
    ev_press = 0;
    if(ret)
    {
        printk("copy error\n");
        return -1;
    }
    
    return 1;
}

static ssize_t device_write(struct file *filp, const char *user_buf, size_t count, loff_t *f_pos)
{
    return 0;
}

static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
        case PIR_SET_GPIO:
        {
            struct PIR_GPIO parm;
            if (pir_run)
            {
                return -EINVAL;
            }
            if (copy_from_user(&parm, (void *)arg, sizeof(struct PIR_GPIO)))
            {
                return -EINVAL;
            }

            if (pir_gpio.gpio_out != parm.gpio_out)
            {
                gpio_free(pir_gpio.gpio_out);
                pir_gpio.gpio_out = parm.gpio_out;

                if(gpio_request(pir_gpio.gpio_out, "PIR_OUT_GPIO"))
                {
                    pr_err("error request PIR GPIO");
                    return -EINVAL;;
                }

                if (gpio_direction_input(pir_gpio.gpio_out))
                {
                    pr_err("error gpio_direction_input PIR OUT GPIO");
                    return -EINVAL;;
                }
            }

            if (pir_gpio.gpio_sen != parm.gpio_sen)
            {
                gpio_free(pir_gpio.gpio_sen);
                pir_gpio.gpio_sen = parm.gpio_sen;

                if(gpio_request(pir_gpio.gpio_sen, "PIR_SEN_GPIO"))
                {
                    pr_err("error request PIR SEN GPIO");
                    return -EINVAL;
                }

                if (gpio_direction_output(pir_gpio.gpio_sen, 0))
                {
                    pr_err("error gpio_direction_output PIR SEN GPIO");
                    return -EINVAL;
                }
            }
        }
        break;

        case PIR_SET_RUN:
        {
            int parm = 0;
            if (copy_from_user(&parm, (void *)arg, sizeof(int)))
            {
                return -EINVAL;
            }

            if (parm == pir_run)
            {
                return -EINVAL;
            }

            if (parm)
            {
                pir_mtimer.function = pir_drive_func;
                hrtimer_start(&pir_mtimer, ktime_set(0, 0), HRTIMER_MODE_REL);
            }
            else
            {
                hrtimer_cancel(&pir_mtimer);
            }
            pir_run = parm;

        }
        break;

        case PIR_SET_SEN:
        {
            int parm = 0;
            if (copy_from_user(&parm, (void *)arg, sizeof(int)))
            {
                return -EINVAL;
            }

            if (parm == pir_sen)
            {
                return -EINVAL;
            }

            pir_sen = parm;

            set_pir_sen(pir_gpio.gpio_sen, pir_sen);
        }
        break;

        case PIR_CHECK_READY:
        {
            pir_ready = gpio_get_value(pir_gpio.gpio_out);
            if (copy_to_user((void __user *)arg, &pir_ready, sizeof(pir_ready)))
            {
                return -EINVAL;
            }
        }
        break;

        default: 
            pr_err("unbeknown cmd\n");
            return -EINVAL;
    }

    return 0;
}

static unsigned int device_poll(struct file * filp, struct poll_table_struct  * table)
{
    unsigned int mask = 0;
    // 将wq添加table列表当中
    poll_wait(filp, &step_waitq, table);

    if(ev_press)
        mask |=POLLIN;//说明有数据被取到了

    ev_press = 0;
    return mask;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .poll = device_poll,
    .unlocked_ioctl = device_ioctl
};

struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &fops,
};

static int __init chardev_init(void)
{
    int ret = -1;
    pir_gpio.gpio_out = 32+24;
    pir_gpio.gpio_sen = 32+25;
    pir_sen = 0;
    pir_run = 0;

    //设置pir 默认的报警输出io口
    if(gpio_request(pir_gpio.gpio_out, "PIR_OUT_GPIO"))
    {
        pr_err("error request PIR GPIO");
        return 0;
    }

    if (gpio_direction_input(pir_gpio.gpio_out))
    {
        pr_err("error gpio_direction_input PIR OUT GPIO");
        return 0;
    }
    //设置pir 默认的灵敏度io口
    if(gpio_request(pir_gpio.gpio_sen, "PIR_SEN_GPIO"))
    {
        pr_err("error request PIR SEN GPIO");
        return 0;
    }

    if (gpio_direction_output(pir_gpio.gpio_sen, 0))
    {
        pr_err("error pir_gpio.gpio_sen PIR SEN GPIO");
        return 0;
    }


    ret = misc_register(&misc);//在sys/class/misc会自动生成.name命名的文件夹
	if(ret < 0)
    {
		printk("misc device register failed!\r\n");
		return -EFAULT;
	}
    pir_run = 0;
    hrtimer_init(&pir_mtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    // pir_mtimer.function = pir_drive_func;
    // hrtimer_start(&pir_mtimer, ktime_set(0, 0), HRTIMER_MODE_REL);

    printk("misc device register: %s\n", DEVICE_NAME);
    return 0;
}

static void __exit chardev_exit(void)
{
    // 模块退出函数
    printk("misc device exit: %s\n", DEVICE_NAME);
    hrtimer_cancel(&pir_mtimer);
    gpio_free(pir_gpio.gpio_out);
    gpio_free(pir_gpio.gpio_sen);
    misc_deregister(&misc);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GPL");
MODULE_DESCRIPTION("Character Device Driver");
