#!/bin/sh
cmd=`realpath $0`
_DIR=`dirname $cmd`
cd $_DIR

export PATH=$PATH:/oem/usr/ko/

#for fastboot
#insmod_wifi.ko ${RK_ENABLE_WIFI_CHIP} ${RK_ENABLE_FASTBOOT}
if [ "${1}"x = "y"x ];then
	insmod dw_mmc.ko
	insmod dw_mmc-pltfm.ko
	insmod dw_mmc-rockchip.ko
	case "$2" in
		ATBM6441)
			insmod cfg80211.ko
			insmod atbm6041_wifi_sdio.ko
			;;
		HI3861L)
			# cat /sys/bus/sdio/devices/*/uevent | grep "0296:5347"
			insmod /oem/usr/ko/hichannel.ko hi_rk_irq_gpio=40
			;;
		*)
			exit 1
			;;
	esac
	rkwifi_server start &
	exit 0
fi

#AIC8800DW
cat /sys/bus/sdio/devices/*/uevent | grep "8800"
if [ $? -eq 0 ];then
	insmod cfg80211.ko
	insmod aic_load_fw.ko aic_fw_path=/oem/usr/ko/
	insmod aic8800_fdrv.ko
	insmod bcmdhd.ko
fi

#AP6XXX
cat /sys/bus/sdio/devices/*/uevent | grep -i "02d0"
if [ $? -eq 0 ];then
	insmod cfg80211.ko
	insmod bcmdhd.ko
fi

#rtl8723ds
cat /sys/bus/sdio/devices/*/uevent | grep "024C:D723"
if [ $? -eq 0 ];then
	insmod cfg80211.ko
	insmod 8723ds.ko
fi

#rtl8189fs
cat /sys/bus/sdio/devices/*/uevent | grep "024C:F179"
if [ $? -eq 0 ];then
	insmod cfg80211.ko
	insmod 8189fs.ko
fi

#rtl18188fu
cat /sys/bus/usb/devices/*/uevent | grep "bda\/f179"
if [ $? -eq 0 ];then
	#wifi disable power
	echo "wifi disable power"
	echo  4 > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio4/direction
	echo 1 > /sys/class/gpio/gpio4/value
	usleep 100000
	#wifi enable power
	echo "wifi enable power"
	echo 0 > /sys/class/gpio/gpio4/value
	insmod libarc4.ko
	insmod rtl8188ftv/cfg80211.ko
	insmod mac80211.ko
	insmod rtl8188ftv/8188fu.ko
fi

#ssv6x5x
cat /sys/bus/usb/devices/*/uevent | grep "8065\/6000"
if [ $? -eq 0 ];then
	insmod  cfg80211.ko
	insmod  libarc4.ko
	insmod  mac80211.ko
	insmod  ctr.ko
	insmod  ccm.ko
	insmod  libaes.ko
	insmod  aes_generic.ko
	insmod  ssv6x5x.ko
fi

#atbm603x
cat /sys/bus/sdio/devices/*/uevent | grep "007A\:6011"
if [ $? -eq 0 ];then
	insmod cfg80211.ko
	insmod libarc4.ko
	insmod ctr.ko
	insmod ccm.ko
	insmod libaes.ko
	insmod aes_generic.ko
	insmod atbm603x_.ko
fi
#atbm6062 ble wifi
cat /sys/bus/usb/devices/*/uevent | grep "7a\/6052"
if [ $? -eq 0 ];then
	#wifi disable power
	echo "wifi disable power"
	echo  4 > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio4/direction
	echo 1 > /sys/class/gpio/gpio4/value
	usleep 100000
	#wifi enable power
	echo "wifi enable power"
	echo 0 > /sys/class/gpio/gpio4/value
	insmod libarc4.ko
	insmod atbm6062/cfg80211.ko
#	insmod mac80211.ko
#	insmod ccm.ko
#	insmod cmac.ko
	insmod atbm6062/ATBM606x_wifi_usb.ko wifi_bt_comb=1
	ifconfig wlan0 up
	echo "iwpriv wlan0 common set_rate_txpower_mode,6"
	iwpriv wlan0 common set_rate_txpower_mode,6
fi

#atbm6012b ble wifi
cat /sys/bus/usb/devices/*/uevent | grep "7a\/888b"
if [ $? -eq 0 ];then
	#wifi disable power
	echo "wifi disable power"
	echo  4 > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio4/direction
	echo 1 > /sys/class/gpio/gpio4/value
	usleep 100000
	#wifi enable power
	echo "wifi enable power"
	echo 0 > /sys/class/gpio/gpio4/value
	insmod libarc4.ko
	insmod cfg80211.ko
	insmod mac80211.ko
	insmod atbm6012b/atbm603x_comb_wifi_usb.ko wifi_bt_comb=1
fi

#atbm6132 ble wifi
cat /sys/bus/usb/devices/*/uevent | grep "7a\/8890"
if [ $? -eq 0 ];then
	#wifi disable power
	echo "wifi disable power"
	echo  4 > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio4/direction
	echo 1 > /sys/class/gpio/gpio4/value
	usleep 100000
	#wifi enable power
	echo "wifi enable power"
	echo 0 > /sys/class/gpio/gpio4/value
	insmod libarc4.ko
	insmod cfg80211.ko
	insmod mac80211.ko
	insmod atbm6132/atbm6x3x_wifi_usb.ko wifi_bt_comb=1
fi

#aic8800dl ble wifi
cat /sys/bus/usb/devices/*/uevent | grep "a69c\/8d80"
if [ $? -eq 0 ];then
	#wifi disable power
	echo "wifi disable power"
	echo  4 > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio4/direction
	echo 1 > /sys/class/gpio/gpio4/value
	usleep 100000
	#wifi enable power
	echo "wifi enable power"
	echo 0 > /sys/class/gpio/gpio4/value
	insmod libarc4.ko
	insmod cfg80211.ko
	insmod mac80211.ko
	#base
	insmod aic8800dl/aic_load_fw.ko aic_fw_path=/oem/usr/ko/aic8800dl/aic8800D80
	#wifi
	insmod aic8800dl/aic8800_fdrv.ko
	#ble
	insmod aic8800dl/aic_btusb.ko
fi

#start wifi app
#rkwifi_server start &
