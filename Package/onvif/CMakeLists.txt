add_definitions(-Wno-stringop-overflow)
aux_source_directory(. DIR_LIB_SRCS)
include_directories(. inc)
# 生成链接库
add_library (Onvif ${DIR_LIB_SRCS})
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib/Package)
