cmake_minimum_required (VERSION 2.6)
project (MarkerTracking)

if (UNIX)
	if (NOT APPLE)
	    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	endif (NOT APPLE)
endif (UNIX)

# Add CMake find modules for special libraries
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

# libraries
SET(Boost_USE_MULTITHREADED   OFF)
SET(Boost_USE_STATIC_LIBS    OFF)
SET(Boost_USE_STATIC_RUNTIME OFF)
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
find_package(Boost REQUIRED system date_time thread timer)
find_package(OpenCV REQUIRED)
find_package(Alvar REQUIRED)
find_package(Bonjour REQUIRED)
find_package(Eigen REQUIRED)
find_package(OpenSceneGraph REQUIRED)

LINK_LIBRARIES(/lib64/libpthread.so.0)

IF(WIN32)
    IF(MSVC)
        ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ENDIF(MSVC)
ENDIF(WIN32)

# Options

# if (WIN32)
	# if (DEFINED OpenCV_INCLUDE_DIR)
		# if (EXISTS "${OpenCV_INCLUDE_DIR}")
			# file(STRINGS ${OpenCV_INCLUDE_DIR}/opencv2/core/version.hpp OpenCV_VERSIONS_TMP REGEX "^#define CV_[A-Z]+_VERSION[ \t]+[0-9]+$")
			# string(REGEX REPLACE ".*#define CV_MAJOR_VERSION[ \t]+([0-9]+).*" "\\1" OpenCV_VERSION_MAJOR ${OpenCV_VERSIONS_TMP})
			# string(REGEX REPLACE ".*#define CV_MINOR_VERSION[ \t]+([0-9]+).*" "\\1" OpenCV_VERSION_MINOR ${OpenCV_VERSIONS_TMP})
			# string(REGEX REPLACE ".*#define CV_SUBMINOR_VERSION[ \t]+([0-9]+).*" "\\1" OpenCV_VERSION_PATCH ${OpenCV_VERSIONS_TMP})
			# set(OpenCV_VERSION ${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH} CACHE STRING "" FORCE)
			# set(CVLIB_SUFFIX "${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
			# message(STATUS "Successfully detected OpenCV ${OpenCV_VERSION}.")
			# include_directories(${OpenCV_INCLUDE_DIR})
            # include_directories(${OpenCV_INCLUDE_DIR}/opencv)
		# else (EXISTS "${OpenCV_INCLUDE_DIR}")
			# message(FATAL_ERROR "${OpenCV_INCLUDE_DIR} does not exist.")
		# endif (EXISTS "${OpenCV_INCLUDE_DIR}")
	# else (DEFINED OpenCV_INCLUDE_DIR)
		# message(FATAL_ERROR "Please set OpenCV_INCLUDE_DIR when compiling under Windows; e.g. -DOpenCV_INCLUDE_DIR=c:\\OpenCV2.3\\build\\include")
	# endif (DEFINED OpenCV_INCLUDE_DIR)

	# if (DEFINED OpenCV_LIB_DIR)
		# if (EXISTS "${OpenCV_LIB_DIR}")
			# set(OPENCV_LIB_COMPONENTS core video highgui imgproc)
			# foreach(__CVLIB ${OPENCV_LIB_COMPONENTS})
				# find_library(OpenCV_${__CVLIB}_LIBRARY NAMES "opencv_${__CVLIB}${CVLIB_SUFFIX}" PATHS "${OpenCV_LIB_DIR}" NO_DEFAULT_PATH)
				# if (OpenCV_${__CVLIB}_LIBRARY)
					# set(LIBS ${LIBS} ${OpenCV_${__CVLIB}_LIBRARY})
				# else (OpenCV_${__CVLIB}_LIBRARY)
					# message(FATAL_ERROR "opencv_${__CVLIB}${CVLIB_SUFFIX} does not exist in ${OpenCV_LIB_DIR}")
				# endif (OpenCV_${__CVLIB}_LIBRARY)
			# endforeach(__CVLIB)
		# else (EXISTS "${OpenCV_LIB_DIR}")
			# message(FATAL_ERROR "${OpenCV_LIB_DIR} does not exist.")
		# endif (EXISTS "${OpenCV_LIB_DIR}")
	# else (DEFINED OpenCV_LIB_DIR)
		# message(FATAL_ERROR "Please set OpenCV_LIB_DIR when compiling under Windows; e.g. -DOpenCV_LIB_DIR=c:\\OpenCV2.3\\build\\x86\\vc10\\lib")
	# endif (DEFINED OpenCV_LIB_DIR)

	# if (EXISTS "$ENV{BONJOUR_SDK_HOME}")
		# include_directories($ENV{BONJOUR_SDK_HOME}/Include)
		# set(LIBS ${LIBS} $ENV{BONJOUR_SDK_HOME}/Lib/Win32/dnssd.lib)
	# else (EXISTS "$ENV{BONJOUR_SDK_HOME}")
		# message(FATAL_ERROR "Environment variable BONJOUR_SDK_HOME was not set correctly. Please ensure that you have the Apple Bonjour SDK for Windows installed and set BONJOUR_SDK_HOME to its location.")
	# endif (EXISTS "$ENV{BONJOUR_SDK_HOME}")

	# if (DEFINED Alvar_INCLUDE_DIR)
		# if (EXISTS "${Alvar_INCLUDE_DIR}")
			# include_directories(${Alvar_INCLUDE_DIR})
		# else (EXISTS "${Alvar_INCLUDE_DIR}")
			# message(FATAL_ERROR "${Alvar_INCLUDE_DIR} does not exist.")
		# endif (EXISTS "${Alvar_INCLUDE_DIR}")
	# else (DEFINED Alvar_INCLUDE_DIR)
		# message(FATAL_ERROR "Please set Alvar_INCLUDE_DIR when compiling under Windows; e.g. -DAlvar_INCLUDE_DIR=c:\\lib")
	# endif (DEFINED Alvar_INCLUDE_DIR)

	# if (DEFINED Alvar_LIB_DIR)
		# if (EXISTS "${Alvar_LIB_DIR}")
			# set(LIBS ${LIBS} ${Alvar_LIB_DIR}/alvar200.lib ${Alvar_LIB_DIR}/alvarplatform200.lib)
		# else (EXISTS "${Alvar_LIB_DIR}")
			# message(FATAL_ERROR "${Alvar_LIB_DIR} does not exist.")
		# endif (EXISTS "${Alvar_LIB_DIR}")
	# else (DEFINED Alvar_LIB_DIR)
		# message(FATAL_ERROR "Please set Alvar_LIB_DIR when compiling under Windows; e.g. -DAlvar_LIB_DIR='C:\\Program Files (x86)\\ALVAR 2.0.0 bin win32 vs2010\\bin'")
	# endif (DEFINED Alvar_LIB_DIR)
# else (WIN32)
	# set(LIBS opencv_core opencv_video opencv_highgui opencv_imgproc alvar200 alvarplatform200)
# endif (WIN32)

SET(TARGET_NAME
	MarkerTracking)

INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${ALVAR_INCLUDE_DIR}
    ${BONJOUR_INCLUDE_DIR}
	${EIGEN_INCLUDE_DIR}
	${OPENSCENEGRAPH_INCLUDE_DIRS}
)

SET(SOURCES
    main.cpp
	src/AlvarTracking.h
	src/AlvarTracking.cpp
	src/Marker.h
	src/Marker.cpp
	src/BonjourRegistration.h
    src/BonjourRegistration.cpp
	src/Common.h
	src/Common.cpp
	src/MarkerTracking.h
	src/MarkerTracking.cpp
	src/MarkerTrackingDetail.h
	src/MarkerTrackingDetail.cpp
	src/NetworkServer.h
	src/NetworkServer.cpp
	src/NetworkServerDetail.h
	src/NetworkServerDetail.cpp
	src/ForwardDeclarations.h
	src/FrameGrabber.h
	src/FrameGrabber.cpp
)

SET(LIBRARIES
    ${Boost_LIBRARIES}
    ${OpenCV_LIBS}
    ${ALVAR_LIBRARIES}
	${BONJOUR_LIBRARIES}
	${OPENSCENEGRAPH_LIBRARIES}
)

# if (UNIX)
	# if (NOT APPLE)
		# set(LIBS ${LIBS} dns_sd)
	# endif (NOT APPLE)
# endif (UNIX)

ADD_EXECUTABLE(${TARGET_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LIBRARIES})

if (WIN32)
    SET_TARGET_PROPERTIES(${TARGET_NAME}
	PROPERTIES
	LINK_FLAGS "/NODEFAULTLIB:libcmt /SUBSYSTEM:CONSOLE"
    )
endif (WIN32)
