bin_PROGRAMS = laser_wrapper

laser_wrapper_CFLAGS = -std=c99 -Isrc/etherdream/common
laser_wrapper_CXXFLAGS = -Isrc/etherdream/driver

laser_wrapper_LDADD = -lm -lpthread ${librt_LIBS}

laser_wrapper_SOURCES = src/test.cpp src/etherdream/driver/etherdream.c src/etherdream/driver/etherdream.h src/etherdream/common/protocol.h
