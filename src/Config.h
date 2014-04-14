#pragma once

namespace laser { namespace config {

extern void readCommandLine(int argc, char *argv[]);
extern bool forceRecalibration;
extern bool useVirtualLaser;

}} // namespace laser::config
