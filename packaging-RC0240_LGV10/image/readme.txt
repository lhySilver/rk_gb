打包工具
用法：
	1.make，编译出 packaging 应用程序；
	2.执行 ./packaging [param 1] [param 2]
		param 1 为分区表配置文件路径；可参考 partition.ini
		param 2 为生成目标文件的路径
		例：/packaging partition.ini Hisilicon_raw.bin

		
注：Bootargs 为 u-boot 环境变量分区文件。
	如需修改环境变量参数，需：
		1.在设备u-boot命令行模式下修改环境变量，然后保存；
		2.在设备应用程序添加相应代码，读出flash中环境变量分区的内容，并写到 注：Bootargs 中。
		
		读取flash分区代码参考：
		
		mount("/dev/mmcblk0p0", "/sd", "vfat", 0, NULL);

		HI_U8 *buffer = (HI_U8 *)malloc(0x00010000);
		if ( !buffer ) {
			MLOGE("malloc failed\n");
			return HI_FAILURE;
		}

		HI_HANDLE flash = HI_FLASH_Open(HI_FLASH_TYPE_SPI_0, 0x00070000, 0x00010000);
		if (flash < 0) {
			MLOGE("flash open failed\n");
			return HI_FAILURE;
		}

		ret = HI_FLASH_Read(flash, 0, (HI_U8 *)buffer, 0x00010000, 0);
		if (ret != 0x00010000) {
			MLOGE("flash read failed\n");
			return HI_FAILURE;
		}

		ret = HI_FLASH_Close(flash);
		if (ret != HI_SUCCESS) {
			MLOGE("flash close failed\n");
			return HI_FAILURE;
		}

		FILE *fp = fopen("/sd/Bootargs", "wb");
		if( !fp )
		{
			MLOGE("open /sd/Bootargs failed\n");
			return HI_FAILURE;
		}

		ret = fwrite(buffer, 0x00010000, 1, fp);
		if (ret != 1)
		{
			MLOGE("write /sd/Bootargs failed\n");
			return HI_FAILURE;
		}

		fflush(fp);
		fclose(fp);

		free(buffer);
