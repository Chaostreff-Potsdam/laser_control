ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = $(BOOST_CPPFLAGS)
AM_LDFLAGS = $(BOOST_LDFLAGS)

bin_PROGRAMS = laser_wrapper

laser_wrapper_CFLAGS = -std=c99 -Isrc/etherdream/common
laser_wrapper_CXXFLAGS = -std=c++0x -Isrc/etherdream/driver

laser_wrapper_LDADD = \
	$(BOOST_CHRONO_LIB) \
    $(BOOST_SYSTEM_LIB) \
    $(BOOST_THREAD_LIB) \
    $(BOOST_DATE_TIME_LIB) \
     -lm -lpthread ${librt_LIBS}

laser_wrapper_SOURCES = src/test.cpp \
	src/EtherdreamWrapper.cpp \
	src/EtherdreamWrapper.h \
	src/etherdream/driver/etherdream.c \
	src/etherdream/driver/etherdream.h \
	src/etherdream/common/protocol.h
