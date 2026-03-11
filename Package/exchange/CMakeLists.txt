aux_source_directory(Source DIR_LIB_SRCS)
include_directories(${PROJECT_SOURCE_DIR}/Middleware/Include)
include_directories(Source ${PROJECT_SOURCE_DIR}/Include)
# 生成链接库
add_library (Exchanger ${DIR_LIB_SRCS})
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib/Package)
