if(UNIX)
	set(DRIVER_DIR driver-unix)
else(UNIX)
	set(DRIVER_DIR driver-win)
endif(UNIX)

include_directories(${LASERWRAPPER_SOURCE_DIR}/src
					${LASERWRAPPER_SOURCE_DIR}/src/etherdream/common
					${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}
					${Boost_INCLUDE_DIRS})
link_directories(${LASERWRAPPER_BINARY_DIR}/src)

if(UNIX)
	set(DRIVER_FILES
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/common/protocol.h
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}/etherdream.c
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}/etherdream.h)
else(UNIX)
	set(DRIVER_FILES
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/common/protocol.h
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}/comm.cpp
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}/dac.cpp
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}/dac.h 	
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}/j4cDAC.cpp
		${LASERWRAPPER_SOURCE_DIR}/src/etherdream/${DRIVER_DIR}/j4cDAC.h)
endif(UNIX)

macro(findCppFiles folderNames resultList)
	set(${resultList} )
	foreach(FOLDER ${${folderNames}})
		file(GLOB files_cpp "${FOLDER}/*.cpp")
		file(GLOB files_cc "${FOLDER}/*.cc")
		file(GLOB files_hpp "${FOLDER}/*.hpp")
		file(GLOB files_h "${FOLDER}/*.h")

		list(APPEND ${resultList} ${files_cpp} ${files_cc} ${files_hpp} ${files_h})
	endforeach(FOLDER)
endmacro(findCppFiles)

set(LASER_WRAPPER_FOLDERS
		"."
		"laser_calibration"
		"utils"
		"animations"
		"objects")

findCppFiles(LASER_WRAPPER_FOLDERS laser_wrapper_files)

# Making stand-alone C++ Library
add_library(laserwrapper SHARED
	${laser_wrapper_files}
	${DRIVER_FILES})

if(UNIX)
	INCLUDE_DIRECTORIES(${Jsoncpp_INCLUDE_DIR})
	set(HOLODECK_FOLDERS
			"holodeck")

	findCppFiles(HOLODECK_FOLDERS HOLODECK_FILES)
else(UNIX)
	set(HOLODECK_FILES )
endif(UNIX)

# Making test application
add_executable(laser_server
			${HOLODECK_FILES})

option(NO_ETHERDREAM_TRACE FALSE)

if(NO_ETHERDREAM_TRACE)
	add_definitions(-DNO_ETHERDREAM_TRACE=1)
endif(NO_ETHERDREAM_TRACE)

if(UNIX)
	if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		SET(THREAD_LIBRARY rt)
	ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

	set(OS_SPECIFIC_LINK_LIBRARIES 
			m
                        pthread
                        ${Jsoncpp_LIBRARY})
else(UNIX)
	set(OS_SPECIFIC_LINK_LIBRARIES 
			shlwapi
			ws2_32
			winmm)
endif(UNIX)

TARGET_LINK_LIBRARIES(laser_server
					laserwrapper
					${OS_SPECIFIC_LINK_LIBRARIES}
					${THREAD_LIBRARY}
					${Boost_LIBRARIES}
					${OpenCV_LIBS})

TARGET_LINK_LIBRARIES(laserwrapper
					${OS_SPECIFIC_LINK_LIBRARIES}
					${THREAD_LIBRARY}
					${Boost_LIBRARIES}
					${OpenCV_LIBS})

if(WIN32)
	SET_TARGET_PROPERTIES(laserwrapper PROPERTIES DEBUG_POSTFIX "d")
endif(WIN32)
