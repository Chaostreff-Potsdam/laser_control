#ifndef LASERPAINTER_H
#define LASERPAINTER_H

#include "EtherdreamWrapper.h"
#include "LaserObject.h"
#include <vector>

class LaserPainter
{
public:
	LaserPainter();

	void paintOn(EtherdreamWrapper *e);
	void paint(std::vector<LaserObject> &objects);
	void add(LaserObject &object);

private:
	EtherdreamWrapper *m_canvas;
	std::vector<LaserObject> m_objects;
};

#endif // LASERPAINTER_H
