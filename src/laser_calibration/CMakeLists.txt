cmake_minimum_required(VERSION 2.8)

project(LASER_CALIBRATION)

find_package( OpenCV REQUIRED )

include_directories(
        ${LASER_CALIBRATION_SOURCE_DIR}/etherdream/driver
        ${LASER_CALIBRATION_SOURCE_DIR}/etherdream/common
        ${OpenCV_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
set(CMAKE_C_FLAGS "-std=c99")

add_executable( ${PROJECT_NAME}
                ${LASER_CALIBRATION_SOURCE_DIR}/main.cpp
                ${LASER_CALIBRATION_SOURCE_DIR}/EtherdreamWrapper.h
                ${LASER_CALIBRATION_SOURCE_DIR}/EtherdreamWrapper.cpp
                ${LASER_CALIBRATION_SOURCE_DIR}/Calibration.h
                ${LASER_CALIBRATION_SOURCE_DIR}/Calibration.cpp
                ${LASER_CALIBRATION_SOURCE_DIR}/CalibrationRectangle.h
                ${LASER_CALIBRATION_SOURCE_DIR}/CalibrationRectangle.cpp
                ${LASER_CALIBRATION_SOURCE_DIR}/etherdream/driver/etherdream.h
                ${LASER_CALIBRATION_SOURCE_DIR}/etherdream/driver/etherdream.c
                ${LASER_CALIBRATION_SOURCE_DIR}/etherdream/common/protocol.h
)

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

