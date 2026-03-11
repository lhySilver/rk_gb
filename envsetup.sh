#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
RK_TOOLCHAIN_BIN="${SCRIPT_DIR}/../RK/arm-rockchip830-linux-uclibcgnueabihf/bin"

if [ -d "${RK_TOOLCHAIN_BIN}" ]; then
    export PATH="${RK_TOOLCHAIN_BIN}:${PATH}"
else
    echo "[envsetup] warning: RK toolchain not found at ${RK_TOOLCHAIN_BIN}"
fi

export PLATFORM="RV"
export CROSS=arm-rockchip830-linux-uclibcgnueabihf-

#RV1106_DUAL_IPC
#RV1103_DUAL_IPC
#export BOARD_TYPE=RV1103_DUAL_IPC
