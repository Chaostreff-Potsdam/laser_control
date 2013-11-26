include_directories(${LASERWRAPPER_SOURCE_DIR}/src
					${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver
					${LASERWRAPPER_SOURCE_DIR}/src/etherdream/common)
link_directories(${LASERWRAPPER_BINARY_DIR}/src)

add_executable(laser_wrapper
			${LASERWRAPPER_SOURCE_DIR}/src/test.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/EtherdreamWrapper.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserPainter.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserObject.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserCircle.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserRectangle.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserLine.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver/etherdream.c)

IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(THREAD_LIBRARY rt)
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

TARGET_LINK_LIBRARIES(laser_wrapper
					m
					pthread
					${THREAD_LIBRARY})
