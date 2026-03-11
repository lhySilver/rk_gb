KERN_DIR=/home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/objs_kernel

all:
	make -C $(KERN_DIR) M=$(shell pwd) modules ARCH=arm CROSS_COMPILE=arm-rockchip830-linux-uclibcgnueabihf-
	
clean:
	rm -rf *.o *~ .depend .*.cmd  *.mod.c .tmp_versions *.ko *.symvers modules.order *.mod
	
obj-m += pir_gpio.o