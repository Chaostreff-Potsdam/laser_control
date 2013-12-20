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
	${LASERWRAPPER_SOURCE_DIR}/src/laser_utilities.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserCompositeObject.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserCompositeObject.h
	${LASERWRAPPER_SOURCE_DIR}/src/LaserPolygon.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/LaserPolygon.h
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/CalibrationRectangle.h
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/CalibrationRectangle.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/Calibration.h
	${LASERWRAPPER_SOURCE_DIR}/src/laser_calibration/Calibration.cpp
	${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver/etherdream.c
	${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver/etherdream.h
	${LASERWRAPPER_SOURCE_DIR}/src/etherdream/common/protocol.h)

# Making stand-alone C++ Library
add_library(laserwrapper SHARED
			${MYSOURCES})

# Making test application
add_executable(laser_wrapper_test
			${LASERWRAPPER_SOURCE_DIR}/src/test.cpp)

if(SWIG_FOUND)
	include(${SWIG_USE_FILE})
	set(CMAKE_SWIG_FLAGS "")
	set_source_files_properties(laserwrapper.i PROPERTIES CPLUSPLUS ON)
	set_property(SOURCE laserwrapper.i PROPERTY SWIG_FLAGS "-namespace" "laser")
	swig_add_module(laserwrappercs csharp laserwrapper.i)
	swig_link_libraries(laserwrappercs
					laserwrapper
					m
					pthread
					${THREAD_LIBRARY}
					${Boost_LIBRARIES})
else(SWIG_FOUND)
	message(WARNING
			"SWIG was not found. You will not be able to compile for C#.")
endif(SWIG_FOUND)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(THREAD_LIBRARY rt)
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

TARGET_LINK_LIBRARIES(laser_wrapper_test
					laserwrapper
					m
					pthread
					${THREAD_LIBRARY}
					${Boost_LIBRARIES}
					${OpenCV_LIBS})

TARGET_LINK_LIBRARIES(laserwrapper
					m
					pthread
					${THREAD_LIBRARY}
					${Boost_LIBRARIES}
					${OpenCV_LIBS})
