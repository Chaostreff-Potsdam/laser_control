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

set(MYSOURCES
	${LASERWRAPPER_SOURCE_DIR}/src/Animation.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/Animation.h
	${LASERWRAPPER_SOURCE_DIR}/src/Canvas.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/Canvas.h
	${LASERWRAPPER_SOURCE_DIR}/src/Color.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/Color.h
	${LASERWRAPPER_SOURCE_DIR}/src/Config.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/Config.h
	${LASERWRAPPER_SOURCE_DIR}/src/DllExport.h
	${LASERWRAPPER_SOURCE_DIR}/src/EtherdreamWrapper.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/EtherdreamWrapper.h
	${LASERWRAPPER_SOURCE_DIR}/src/Painter.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/Painter.h
	${LASERWRAPPER_SOURCE_DIR}/src/Transform.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/Transform.h
	${LASERWRAPPER_SOURCE_DIR}/src/VirtualLaser.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/VirtualLaser.h
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/Calibration.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/Calibration.h
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/CalibrationRectangle.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/CalibrationRectangle.h
	${LASERWRAPPER_SOURCE_DIR}/src/laser_utilities.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/laser_utilities.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Circle.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Circle.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/CompositeObject.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/CompositeObject.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Line.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Line.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Object.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Object.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Polygon.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Polygon.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Rectangle.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Rectangle.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Spiral.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Spiral.h
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Wave.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/objects/Wave.h
    ${DRIVER_FILES})

# Making stand-alone C++ Library
add_library(laserwrapper SHARED
	${MYSOURCES})

if(UNIX)
        INCLUDE_DIRECTORIES(${Jsoncpp_INCLUDE_DIR})
	set(HOLODECK_FILES
		${LASERWRAPPER_SOURCE_DIR}/src/holodeck/Server.cpp
		${LASERWRAPPER_SOURCE_DIR}/src/holodeck/Server.h
		${LASERWRAPPER_SOURCE_DIR}/src/holodeck/Instruction.cpp
		${LASERWRAPPER_SOURCE_DIR}/src/holodeck/Instruction.h
		${LASERWRAPPER_SOURCE_DIR}/src/holodeck/InstructionFactory.cpp
		${LASERWRAPPER_SOURCE_DIR}/src/holodeck/InstructionFactory.h
		${LASERWRAPPER_SOURCE_DIR}/src/holodeck/InstructionCaller.h
	)
else(UNIX)
	set(HOLODECK_FILES )
endif(UNIX)

# Making test application
add_executable(laser_wrapper_test
			${HOLODECK_FILES}
			${LASERWRAPPER_SOURCE_DIR}/src/test.cpp)

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

TARGET_LINK_LIBRARIES(laser_wrapper_test
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
