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

add_library(laserwrapper
			${LASERWRAPPER_SOURCE_DIR}/src/EtherdreamWrapper.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserPainter.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserObject.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserCircle.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserRectangle.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserLine.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver/etherdream.c)

if(SWIG_FOUND)
	include(${SWIG_USE_FILE})
	set(CMAKE_SWIG_FLAGS "")
	set_source_files_properties(laserwrapper.i PROPERTIES CPLUSPLUS ON)
	swig_add_module(laserwrappercs csharp laserwrapper.i
			${LASERWRAPPER_SOURCE_DIR}/src/EtherdreamWrapper.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserPainter.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserObject.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserCircle.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserRectangle.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/LaserLine.cpp
			${LASERWRAPPER_SOURCE_DIR}/src/etherdream/driver/etherdream.c)
	swig_link_libraries(laserwrappercs
					m
					pthread
					${THREAD_LIBRARY})
else(SWIG_FOUND)
	message(WARNING
			"SWIG was not found. You will not be able to compile for C#.")
endif(SWIG_FOUND)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(THREAD_LIBRARY rt)
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

TARGET_LINK_LIBRARIES(laser_wrapper
					m
					pthread
					${THREAD_LIBRARY})

TARGET_LINK_LIBRARIES(laserwrapper
					m
					pthread
					${THREAD_LIBRARY})
