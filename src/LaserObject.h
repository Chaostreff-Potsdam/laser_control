#ifndef LASEROBJECT_H
#define LASEROBJECT_H

#include "etherdream.h"
#include <vector>
#include <memory>


class LaserObject
{
public:
	LaserObject();

	virtual std::vector<etherdream_point> points() const = 0;
	virtual etherdream_point startPoint() const = 0;
	virtual etherdream_point endPoint() const = 0;

};

typedef std::shared_ptr<LaserObject> LaserObjectPtr;

#endif // LASEROBJECT_H
