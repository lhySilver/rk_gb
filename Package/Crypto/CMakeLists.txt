#cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
#project(DG_crypto VERSION 1.0)
#project(DG_crypto)
#add_definitions(-Wall -o3)
#add_definitions(-w -Wno-psabi)
#add_definitions(-DISP_HW_V32)
#add_definitions(-DISP_HW_V32 -D_GNU_SOURCE)

#系统
#SET(CMAKE_SYSTEM_NAME Linux)

#指定编译器
#c编译
#SET(CMAKE_C_COMPILER "/home/ale/workspace/RV1106/RV1106_IPC_SDK/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc")
#c++编译
#SET(CMAKE_CXX_COMPILER "/home/ale/workspace/RV1106/RV1106_IPC_SDK/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/arm-rockchip830-linux-uclibcgnueabihf-g++")

# 如果想生成静态库，使用下面的语句

#add_library(DG_crypto_static Source/aes.c Source/crypto.c )
#target_include_directories(DG_crypto PUBLIC ${CMAKE_SOURCE_DIR}/Include)
#set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib)

# SET(LIBDG_crypto_SRC Source/aes.c Source/crypto.c)

# ADD_LIBRARY(DG_crypto_static STATIC ${LIBDG_crypto_SRC})

# # 对DG_crypto_static的重名为DG_crypto
# SET_TARGET_PROPERTIES(DG_crypto_static PROPERTIES  OUTPUT_NAME "DG_crypto")
# # cmake 在构建一个新的target 时，会尝试清理掉其他使用这个名字的库，如果没有清理还是会只会构建一个动态库，不会构建出静态库
# SET_TARGET_PROPERTIES(DG_crypto_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# ADD_LIBRARY(DG_crypto SHARED ${LIBDG_crypto_SRC})
    
# # 对DG_crypto_static的重名为DG_crypto
# SET_TARGET_PROPERTIES(DG_crypto PROPERTIES  OUTPUT_NAME "DG_crypto")
# SET_TARGET_PROPERTIES(DG_crypto PROPERTIES CLEAN_DIRECT_OUTPUT 1)


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(Source DIR_LIB_SRCS)
include_directories(Include)
# 生成链接库
add_library (DG_crypto ${DIR_LIB_SRCS})
#set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Lib)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib/Package)
