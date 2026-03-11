KERN_DIR=/home/ale/workspace/RV1106/RV1106_RV1103_LINUX_IPC_SDK/1.5.0/RV1106_IPC_SDK/sysdrv/source/kernel

all:
	make -C $(KERN_DIR) M=$(shell pwd) modules ARCH=arm CROSS_COMPILE=/opt/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-
	
clean:
	rm -rf *.o *~ .depend .*.cmd  *.mod.c .tmp_versions *.ko *.symvers modules.order *.mod
	
obj-m += motor_gpio.o