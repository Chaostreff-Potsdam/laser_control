#include "Config.h"
#include <cstring>

namespace laser { namespace config {

bool forceRecalibration = false;
bool useVirtualLaser = false;

void readCommandLine(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i) {
		forceRecalibration |= !strncmp("--recalib", argv[i], 9);
		useVirtualLaser    |= !strncmp("--virtual", argv[i], 9);
	}
}


}}
