#pragma once

#include "DllExport.h"

namespace laser { namespace config {

EXPORT_LASER_CONTROL void readCommandLine(int argc, char *argv[]);
EXPORT_LASER_CONTROL extern bool forceRecalibration;
EXPORT_LASER_CONTROL extern bool useVirtualLaser;
EXPORT_LASER_CONTROL extern bool displayTests;
EXPORT_LASER_CONTROL extern bool testServer;
EXPORT_LASER_CONTROL extern bool oldCalib;
EXPORT_LASER_CONTROL extern bool drawCircs;
EXPORT_LASER_CONTROL extern bool log;

}} // namespace laser::config
