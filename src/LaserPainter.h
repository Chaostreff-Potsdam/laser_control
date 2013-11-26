#ifndef LASERPAINTER_H
#define LASERPAINTER_H

#include "EtherdreamWrapper.h"
#include "LaserObject.h"
#include <vector>
#include <memory>

class LaserPainter
{
public:
	LaserPainter();

	void paintOn(EtherdreamWrapper *e);
	void paint(std::vector<LaserObjectPtr> objects);
	void add(LaserObjectPtr object);

private:
	void updatePoints();
	EtherdreamWrapper *m_canvas;
	std::vector<LaserObjectPtr> m_objects;
};

#endif // LASERPAINTER_H
