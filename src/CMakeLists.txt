include_directories(${LASERWRAPPER_SOURCE_DIR}/src
					${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver
					${LASERWRAPPER_SOURCE_DIR}/src/etherdream/common
					${Boost_INCLUDE_DIRS})
link_directories(${LASERWRAPPER_BINARY_DIR}/src)

set(MYSOURCES
	${LASERWRAPPER_SOURCE_DIR}/src/EtherdreamWrapper.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/EtherdreamWrapper.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserPainter.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserPainter.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserObject.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserObject.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserCircle.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserCircle.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserRectangle.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserRectangle.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserLine.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserLine.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserServer.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserServer.h
	${LASERWRAPPER_SOURCE_DIR}/src/laser_utilities.h
	${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver/etherdream.c
	${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver/etherdream.h
	${LASERWRAPPER_SOURCE_DIR}/src/etherdream/common/protocol.h)

add_library(laserwrapper SHARED
			${MYSOURCES})

add_executable(laser_wrapper_test
			${LASERWRAPPER_SOURCE_DIR}/src/test.cpp)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(THREAD_LIBRARY rt)
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

TARGET_LINK_LIBRARIES(laser_wrapper_test
					laserwrapper
					m
					pthread
					${THREAD_LIBRARY}
					${Boost_LIBRARIES})

TARGET_LINK_LIBRARIES(laserwrapper
					m
					pthread
					${THREAD_LIBRARY}
					${Boost_LIBRARIES})
