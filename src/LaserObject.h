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
	virtual std::vector<etherdream_point> startPoints() const = 0;
	virtual std::vector<etherdream_point> endPoints() const = 0;
	virtual void rotate(double rad) = 0;
	virtual void move(int x, int y) = 0;
};

typedef std::shared_ptr<LaserObject> LaserObjectPtr;

#endif // LASEROBJECT_H
