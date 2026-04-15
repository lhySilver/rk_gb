#!/bin/sh

wpa_supplicant -Dnl80211 -iwlan0 -c /etc/wpa_supplicant.conf & 
sleep 5 
wpa_cli -i wlan0 add_network 
wpa_cli -i wlan0 set_network 1 ssid '"DGIOT"' 
wpa_cli -i wlan0 set_network 1 psk '"dgiot0202"' 
wpa_cli -i wlan0 enable_network 1 
sleep 10 
udhcpc -b -i wlan0 -s /usr/share/udhcpc/default.script
sleep 5

ifconfig lo up
sleep 1
mount --bind /mnt/sdcard/librockit_full.so /usr/lib/librockit_full.so
sleep 5
cp /mnt/sdcard/rkaiq_tool_server /tmp/rkaiq_tool_server
sleep 1
chmod 777 /tmp/rkaiq_tool_server
sleep 1
sample_demo_vi_avs_venc --vi_size 1920x1080 --avs_chn0_size 3840x1080 --avs_chn1_size 1920x544 -a /oem/usr/share/iqfiles/ -e h264cbr -b 4096 -n 2 --stitch_distance 4 -F 15 --avs_mode_blend 2 --set_ldch -1 &
sleep 1
/tmp/rkaiq_tool_server &
