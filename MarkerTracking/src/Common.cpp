#include "Common.h"

namespace markerTracking
{

namespace definitions
{
    const int captureWidth = 1920;
    const int captureHeight = 1080;

    const int processingWidth = 1920;
    const int processingHeight = 1080;

	// ID of the bottom-left reference marker. The others have to be +1, +2, and +3
	const int referenceMarkerID = 12;

	// Width of the court [m]
	const double courtWidth = 6.0;

	// Height of the court [m]
	const double courtHeight = 4.0;

	// Size of the hat markers [m]
	const double hatMarkerSize = 0.155;

	// Size of the reference markers [m]
	const double referenceMarkerSize = 0.219;
}

}
