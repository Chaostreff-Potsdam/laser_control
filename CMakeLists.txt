cmake_minimum_required(VERSION 2.6)
project(LASERWRAPPER)

find_package(SWIG)
find_package(Boost 1.40
			REQUIRED
			COMPONENTS system regex)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
set(CMAKE_C_FLAGS "-std=c99")
add_definitions(-D_USE_MATH_DEFINES)
add_subdirectory(src)
