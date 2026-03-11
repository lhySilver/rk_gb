####################################
通过GPIO驱动motor：
1、内核驱动：将motor.rar解压后，"motor/驱动"目录下的文件motor_gpio.c motor_gpio.h即为内核驱动，编译到kernel并下载到板子上；
2、demo：将motor.rar解压后，"motor/demo"目录下的文件motor.c和motor_gpio.h为demo代码，通过open ioctl方式驱动motor。

驱动电机方法：
将内核驱动编译并下载到板子后，可通过以下两种方法转动电机：
1、在应用代码中，使用open ioctl close方式驱动电机，详见demo代码；
2、在linux终端，先通过echo ‘22，26，27，28, 4’> /dev/motor-0和echo ‘22，5，19，20, 21’> /dev/motor-1分别设置两电机GPIO口，再通过echo ‘0，0，0，100, 1000000’> /dev/motor-0等命令控制电机转动。

命令格式为'cmd, enable, reverse, num of step(pulse), time of pulse(ns)'：
 cmd： 22：set motor gpio   0：驱动电机
 enable：0：不驱动电机  1：驱动电机转动
 reverse： 0、1：正转或反转
 numofstep：>0：电机走多少步，每步为5.625/64度
 timeofpulse：单位为ns，每步的脉冲持续时间，比如100000ns每步，频率为10KHz，波形频率为10khz/8

注意事项：
1、电机牵入牵出频率分别为500hz和1khz，因此驱动设置了电机可调节转动频率范围为100hz-10khz，在这个区间的更小区间中，电机能够保持转动，并在频率越大时转速越快，但在大于某个阈值时，电机无法转动，会伴随叫声；
2、理论上设置频率越小，电机越能够转动，如果出现频率最小，电机依然无法转动的情况，建议对比多个电机在相同驱动参数下是否有差异，并确认电机负载（有时可能机器连接过紧密，导致摩擦轮过大，电机无法转动）情况。
3、每次电机相线端从低电平变为高电平，在高电平持续时间中会有一些异常波动，包括一个非常明显的电平突变，暂时推测是被同连的电机绕组影响了，不知这种现象是否会影响电机驱动能力。



motor: motor{
		compatible = "motor";
		status = "okay";
		motorA-gpios = <&gpio1 RK_PC3 GPIO_ACTIVE_HIGH>;
		motorB-gpios = <&gpio1 RK_PC2 GPIO_ACTIVE_HIGH>;
		motorC-gpios = <&gpio1 RK_PC1 GPIO_ACTIVE_HIGH>;
		motorD-gpios = <&gpio1 RK_PC0 GPIO_ACTIVE_HIGH>;
		motorE-gpios = <&gpio1 RK_PC7 GPIO_ACTIVE_HIGH>;
		motorF-gpios = <&gpio1 RK_PC6 GPIO_ACTIVE_HIGH>;
		motorG-gpios = <&gpio1 RK_PC5 GPIO_ACTIVE_HIGH>;
		motorH-gpios = <&gpio1 RK_PC4 GPIO_ACTIVE_HIGH>;
	};