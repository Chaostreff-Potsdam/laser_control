# - Try to find Jsoncpp
# Once done, this will define
#
#  Jsoncpp_FOUND - system has Jsoncpp
#  Jsoncpp_INCLUDE_DIRS - the Jsoncpp include directories
#  Jsoncpp_LIBRARIES - link these to use Jsoncpp

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(Jsoncpp_PKGCONF jsoncpp)

IF(APPLE)
# Screw it, pkg config gives the right path, but find_path destroys it.
SET(Jsoncpp_INCLUDE_DIR ${Jsoncpp_PKGCONF_INCLUDE_DIRS})
ELSE(APPLE)
# Include dir
find_path(Jsoncpp_INCLUDE_DIR
  NAMES json/features.h
  PATH_SUFFIXES jsoncpp
  PATHS ${Jsoncpp_PKGCONF_INCLUDE_DIRS} # /usr/include/jsoncpp/json
)
ENDIF(APPLE)

# Finally the library itself
find_library(Jsoncpp_LIBRARY
  NAMES jsoncpp
  PATHS ${Jsoncpp_PKGCONF_LIBRARY_DIRS}
#  PATH ./jsoncpp/
)

set(Jsoncpp_PROCESS_INCLUDES Jsoncpp_INCLUDE_DIR)
set(Jsoncpp_PROCESS_LIBS Jsoncpp_LIBRARY)
libfind_process(Jsoncpp)
