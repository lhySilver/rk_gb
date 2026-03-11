set(DIR_LIB_SRCS Source/ConfigManager.cpp Source/User.cpp Source/UserManager.cpp Source/Event.cpp Source/MagicBox.cpp Source/DefaultConfig.cpp Source/Md5.cpp)
include_directories(${PROJECT_SOURCE_DIR}/Middleware/Include)
include_directories(Source ${PROJECT_SOURCE_DIR}/Include)
# 生成链接库
add_library (Manager ${DIR_LIB_SRCS})
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib/Package)