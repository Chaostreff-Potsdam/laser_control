#pragma once

namespace laser { namespace config {

extern void readCommandLine(int argc, char *argv[]);
extern bool forceRecalibration;
extern bool useVirtualLaser;
extern bool displayTests;

}} // namespace laser::config
