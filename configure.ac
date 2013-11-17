AC_INIT([laser_control], [1])

################################################################################
#
# Set up the whole building process, check for our code and the boost scripts
#
################################################################################
AC_CONFIG_SRCDIR([src/etherdream/common/protocol.h])
AM_INIT_AUTOMAKE([foreign no-installinfo no-installman subdir-objects])
AC_CONFIG_MACRO_DIR([m4])
################################################################################

# We need a C compiler
AC_PROG_CC

# and a C++ compiler
AC_PROG_CXX

################################################################################
#
# Only link lrt if it isn't already in libc
#
################################################################################
AC_SEARCH_LIBS([clock_gettime], [rt], [librt_LIBS="$LIBS"; LIBS="$LIBS_system"])
AC_SUBST([librt_LIBS])

################################################################################
#
# Check for boost headers and needed libs
#
################################################################################
AC_LANG_PUSH([C++])
AX_BOOST_BASE([1.40],, [AC_MSG_ERROR([This etherdream wrapper needs Boost, but it was not found in your system])])
AX_BOOST_SYSTEM
AX_BOOST_CHRONO
AX_BOOST_DATE_TIME
AX_BOOST_THREAD
AC_LANG_POP([C++])
################################################################################

# Write to "Makefile"
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
