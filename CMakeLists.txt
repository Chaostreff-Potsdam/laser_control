cmake_minimum_required(VERSION 2.6)
project(LASERWRAPPER)

find_package(SWIG)
find_package(OpenCV REQUIRED)
find_package(Boost 1.40
			REQUIRED
			COMPONENTS system regex date_time)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(APPLE)
	set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall -Wextra")
	set(CMAKE_C_FLAGS "-std=c99")
else(APPLE)
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
	set(CMAKE_C_FLAGS "-std=c99")
endif(APPLE)
add_definitions(-D_USE_MATH_DEFINES)
add_subdirectory(src)
