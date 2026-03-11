#!/bin/bash

ROOT=$(cd $(dirname $0) && pwd )
BUILD_DIR=$ROOT/cmake-build
BUILD_MID_DIR=$ROOT/Middleware/cmake-build
DAEMON_DIR=$ROOT/daemon
BURNTOOL_DIR=$ROOT/burntool

echo $ROOT
echo $DAEMON_DIR
echo $SDUPDATE_DIR

CHIP_TYPE=RV1106_DUAL_IPC
#BOARD_TYPE=RV1103_DUAL_IPC
BOARD_TYPE=RC0240_LGV10
# BLE_TYPE=ATBM6062
# BLE_TYPE=ATBM6012B
# BLE_TYPE=ATBM6132
BLE_TYPE=AIC8800DL
#DMODE=debug
DMODE=release
if [ -z $CROSS ];then
	source $ROOT/envsetup.sh
	echo "CROSS = $CROSS"
else
	echo "CROSS = $CROSS"
fi

function daemon-clean()
{
	make clean -C $DAEMON_DIR;
}

function daemon()
{
	make -C $DAEMON_DIR;
}
#----------------------------
function burntool-clean()
{
	make clean -C $BURNTOOL_DIR;
}
#----------------------------
function burntool()
{
	make -C $BURNTOOL_DIR;
}
#----------------------------
function clean_mid()
{
	rm -rf $BUILD_MID_DIR
	rm -rf $ROOT/Middleware/Lib
}
#----------------------------
function make_mid()
{
	if [ ! -d $BUILD_MID_DIR ]; then
		mkdir $BUILD_MID_DIR
	fi
	cd $BUILD_MID_DIR
	cmake .. -D$CHIP_TYPE=ON -D$BOARD_TYPE=ON -D$DMODE=ON
	make -j4
	cd $ROOT
}
#----------------------------
function clean_app()
{
	rm -rf $BUILD_DIR
	rm -rf $ROOT/Bin
	rm -rf $ROOT/Lib/Package
	daemon-clean
	burntool-clean
}
#----------------------------
function make_app()
{
	if [ ! -d $BUILD_DIR ]; then
		mkdir $BUILD_DIR
	fi
	cd $BUILD_DIR
	cmake .. -D$CHIP_TYPE=ON -D$BOARD_TYPE=ON -D$BLE_TYPE=ON -D$DMODE=ON
	make -j4
	cd $ROOT
	daemon
	burntool
}
#----------------------------
function clean_all()
{
	clean_mid
	clean_app
}
#----------------------------
function make_all()
{
	make_mid
	make_app
}
#----------------------------
function image()
{
	echo "make image ..."
	make -C $PACKAGING;
	echo "make image $PACKAGING $BOARD_TYPE $BLE_TYPE end ..."
}
#----------------------------
function image-clean()
{
	echo "clean image ..."
	make clean -C $PACKAGING;
	echo "clean image $PACKAGING $BOARD_TYPE $BLE_TYPE end ..."
}
#----------------------------
function usage()
{
	echo ""
	echo "Usage: build.sh [Options] [Project]"
	echo ""

	echo "Available Options :"
	echo "all             -build all [app image]"
	echo "clean           -build clean all"
	echo "app             -build app"

	# echo "app-clean     -build clean Middleware"
	# echo "mw            -build Middleware"
	# echo "mw-clean      -build clean Middleware"
	# echo "pkg           -build Package"
	# echo "pkg-clean     -build clean Package"
	# echo "image         -build packaging image"
	# echo "image-clean   -build packaging image clean"

	echo ""
}
#----------------------------
function check()
{
	# CHIP_TYPE=RV1106_DUAL_IPC
	BOARD_TYPE=RC0240_LGV10
	# BLE_TYPE=ATBM6062
	echo "CHIP_TYPE=$CHIP_TYPE"
	echo "BOARD_TYPE=$BOARD_TYPE"
	echo "BLE_TYPE=$BLE_TYPE"
	PACKAGING=$ROOT/packaging-$BOARD_TYPE
	echo "PACKAGING=$PACKAGING"
}

if [ "$1" = "clean" ];then
	clean_all
#	image-clean
elif [ "$1" == "all" ];then
	DMODE=release
	check $2
#	clean_all
	make_all
	echo "BOARD_TYPE=$BOARD_TYPE"
	image
elif [ "$1" == "debug" ];then
	DMODE=debug
	check $2
#	clean_all
	make_all
	echo "BOARD_TYPE=$BOARD_TYPE"
	image
elif [ "$1" == "app" ];then
	check $2
	make_app
elif [ "$1" == "app-clean" ];then
	check $2
	clean_app
elif [ "$1" == "mid" ];then
	check $2
	make_mid
elif [ "$1" == "mid-clean" ];then
	check $2
	clean_mid
elif [ "$1" == "image" ];then
	check $2
	image
elif [ "$1" == "image-clean" ];then
	check $2
	image-clean
else
	usage
fi