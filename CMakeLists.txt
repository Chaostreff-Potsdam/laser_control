cmake_minimum_required(VERSION 2.6)
project(LASERWRAPPER)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(BOOST_VERSION 1.40)
if(WIN32)
	set(OpenCV_DIR "Z:/lib_msvc11/opencv/2.4.8")
	set(OpenCV_INCLUDE_DIR "Z:/lib_msvc11/opencv/2.4.8/include/opencv")
	set(BOOST_ROOT "Z:/lib_msvc11/boost/1.55.0")
	set(Boost_USE_STATIC_LIBS ON)
	set(BOOST_VERSION 1.55)
endif(WIN32)

find_package(OpenCV REQUIRED)
find_package(Boost ${BOOST_VERSION}
			REQUIRED
                        COMPONENTS system regex date_time)

if(UNIX)
        find_package(Jsoncpp REQUIRED)

        if(Jsoncpp_FOUND)
            message(STATUS Jsoncpp_INCLUDE_DIR = ${Jsoncpp_INCLUDE_DIR})
            message(STATUS Jsoncpp_LIBRARY = ${Jsoncpp_LIBRARY})
        endif(Jsoncpp_FOUND)
endif(UNIX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(UNIX)
	set(UNIX_FLAGS "-Wextra -O2")
else(UNIX)
	set(UNIX_FLAGS "")
endif(UNIX)

if(APPLE)
	set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall ${UNIX_FLAGS}")
	set(CMAKE_C_FLAGS "-std=c99")
else(APPLE)
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall ${UNIX_FLAGS}")
	set(CMAKE_C_FLAGS "-std=c99")
endif(APPLE)
add_definitions(-D_USE_MATH_DEFINES)
add_subdirectory(src)
