cmake_minimum_required(VERSION 2.6)
project(LASERWRAPPER)

find_package(SWIG)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(APPLE)
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -m32")
	set(CMAKE_C_FLAGS "-std=c99 -m32")
else(APPLE)
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
	set(CMAKE_C_FLAGS "-std=c99")
endif(APPLE)
add_definitions(-D_USE_MATH_DEFINES)
add_subdirectory(src)
