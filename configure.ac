AC_INIT([laser_control], [1])
AC_CONFIG_SRCDIR([src/etherdream/common/protocol.h])
AM_INIT_AUTOMAKE([foreign no-installinfo no-installman subdir-objects])

AC_PROG_CC

AC_PROG_CXX

AC_SEARCH_LIBS([clock_gettime], [rt], [librt_LIBS="$LIBS"; LIBS="$LIBS_system"])
AC_SUBST([librt_LIBS])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
