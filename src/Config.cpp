#include "Config.h"
#include <cstring>

namespace laser { namespace config {

void readCommandLine(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i) {
		forceRecalibration |= !strncmp("--recalib", argv[i], 9);
	}
}

bool forceRecalibration = false;

}}
