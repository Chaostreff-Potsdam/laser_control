#pragma once

#include "Canvas.h"
#include <opencv2/core/core.hpp>

namespace laser {

class VirtualLaser: public Canvas
{
public:
	static const char *windowName;
	static const int windowSize;
	static const float scale;

	VirtualLaser();
	virtual ~VirtualLaser();

	void writePoints();
};

}
