

    message("compile for rv1106 dual-ipc")
	option(COMPILE_FOR_EXE "compile for executable" OFF)
	option(COMPILE_FOR_LIB "compile for lib" ON)
	
	if (RC0240) 
		add_definitions(-DRC0240)
	elseif (RC0240_LGV10)
		add_definitions(-DRC0240_LGV10)
	endif()
	
	add_definitions(-DISP_HW_V30 -DRV1106_DUAL_IPC -DRV1106 -DROCKIT_IVS -DROCKCOMBO -DUAPI2 -DAVS_ENABLE -DRKAIQ)

	include_directories(
		${PROJECT_SOURCE_DIR}/Include
		${CMAKE_CURRENT_SOURCE_DIR}/Include
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/uAPI
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/uAPI2
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/algos
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/algos/awb
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/algos/ae
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/common
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/xcore
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/iq_parser
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/iq_parser_v2
		${CMAKE_CURRENT_SOURCE_DIR}/Include/rkaiq/smartIr
		${CMAKE_CURRENT_SOURCE_DIR}/Include/libdrm
		${CMAKE_CURRENT_SOURCE_DIR}/Include/freetype2
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/isp/rv1106
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/socket_server
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/rtsp
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/rtmp
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/param
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/system
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/osd
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/osd/freetype2
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/roi
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/network
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/storage
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/rockiva
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/event
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/region_clip
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/src/rv1106_dual_ipc/audio
		${CMAKE_CURRENT_SOURCE_DIR}/rkipc/src/rv1106_dual_ipc/video

		${CMAKE_CURRENT_SOURCE_DIR}/src
	)

	if(COMPILE_FOR_EXE)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/src/rv1106_dual_ipc SRCS)
	endif()

	if(COMPILE_FOR_LIB)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRCS)
	endif()

	#aux_source_directory(server SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/libdmc SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/src/rv1106_dual_ipc/audio SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/src/rv1106_dual_ipc/video SRCS)

	
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/isp/rv1106 SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/ SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/socket_server SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/param SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/system SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/rtsp SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/rtmp SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/osd SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/roi SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/network SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/storage SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/rockiva SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/event SRCS)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/rkipc/common/region_clip SRCS)


	message("src_list=${SRCS}")

	add_definitions(-DISP_HW_V32)

	if(COMPILE_FOR_EXE)
	add_executable(${PROJECT_NAME} ${SRCS})
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}../../App/Build/Platform/RV/Device/MultitekSDK)
	target_link_libraries(${PROJECT_NAME} pthread z rockit_full rockchip_mpp rkaiq rtsp rkaudio_detect rkaudio rkaudio_common
	rkaudio_vqe wpa_client m rkmuxer freetype rockiva iconv rknnmrt rga stdc++ rksysutils drm)
	endif()

	if(COMPILE_FOR_LIB)
	# #Generate the static library from the library sources
	add_library(mpp STATIC ${SRCS})
	endif()



set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib)





