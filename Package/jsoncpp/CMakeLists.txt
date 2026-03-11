aux_source_directory(. DIR_LIB_SRCS)
include_directories(include)
# 生成链接库
add_library (json ${DIR_LIB_SRCS})
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib/Package)
