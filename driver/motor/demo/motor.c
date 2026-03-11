#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "dsp/fh_system_mpi.h"
#include <stdlib.h>
#include <sys/time.h>
#include "motor_gpio.h"
#include <sys/ioctl.h>

void usage(char const *program_name)
{
    printf("\nUsage:  %s <motor>(0/1) <reverse>(0/1) <param>(>=0) <value>(>0)\n\n",
            program_name);
    printf("Example:\n");
    printf("    %s 0 0 100 1000000\n", program_name);
}

int main(int argc, char const *argv[])
{
    int motor;
    int reverse;
    int param;
    int value;
    
    int fd = -1;

    if (argc < 5)
    {
        usage(argv[0]);
        return -1;
    }

    motor = atoi(argv[1]);
    reverse = atoi(argv[2]);
    param = atoi(argv[3]);
    value = atoi(argv[4]);

    if (motor < 0 || motor > 1 || reverse < 0 || reverse > 1 || param < 0 || value <= 0)
    {
        printf("INPUT EEROR!\n");
        return -1;
    }

    struct motor_gpio motor_io;

    if (motor == 0)
    {
        motor_io.gpio_a = 26;
        motor_io.gpio_b = 27;
        motor_io.gpio_c = 28;
        motor_io.gpio_d = 4;
    }
    else if (motor == 1)
    {
        motor_io.gpio_a = 5;
        motor_io.gpio_b = 19;
        motor_io.gpio_c = 20;
        motor_io.gpio_d = 21;
    }

    struct motor_config motor_cfg;

    motor_cfg.en = 1;
    motor_cfg.mt_rotat = reverse;
    motor_cfg.nsec = value;
    motor_cfg.step_num = param;
    
    if (motor == 0)
    {
        motor_cfg.en = 1;
        motor_cfg.mt_rotat = reverse;
        motor_cfg.nsec = value;
        motor_cfg.step_num = param;

        fd = open("/dev/motor-0", O_RDWR);
        if (fd < 0)
        {
            printf("open motor0 fail!\n");
            return 0;
        }
        if (0 > ioctl(fd, MOTOR_SET_GPIO, &motor_io))//上电后只需调用一次，多次调用也可以
            printf("motor-0 MOTOR_SET_GPIO fail!\n");
            
        if (0 > ioctl(fd, MOTOR_SET_DRV_CONFIG, &motor_cfg))
            printf("motor-0 MOTOR_SET_DRV_CONFIG fail!\n");
            
        close(fd);
    }
    else if (motor == 1)
    {
        fd = open("/dev/motor-1", O_RDWR);
        if (fd < 0)
        {
            printf("open motor1 fail!\n");
            return 0;
        }
        if (0 > ioctl(fd, MOTOR_SET_GPIO, &motor_io))//上电后只需调用一次，多次调用也可以
            printf("motor-1 MOTOR_SET_GPIO fail!\n");
            
        if (0 > ioctl(fd, MOTOR_SET_DRV_CONFIG, &motor_cfg))
            printf("motor-1 MOTOR_SET_DRV_CONFIG fail!\n");
            
        close(fd);
    }

    return 0;
}
