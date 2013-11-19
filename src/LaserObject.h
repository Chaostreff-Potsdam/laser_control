#ifndef LASEROBJECT_H
#define LASEROBJECT_H

#include "etherdream.h"
#include <vector>

class LaserObject
{
public:
	LaserObject();

	virtual std::vector<struct etherdream_point> points() = 0;
};

#endif // LASEROBJECT_H
