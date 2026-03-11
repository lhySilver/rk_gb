aux_source_directory(Source DIR_LIB_SRCS)
include_directories(Source)
# 生成链接库
add_library (DebugInfo ${DIR_LIB_SRCS})
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib/Package)
