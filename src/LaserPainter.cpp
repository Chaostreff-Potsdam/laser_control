#include "LaserPainter.h"

LaserPainter::LaserPainter()
{
}

void LaserPainter::paintOn(EtherdreamWrapper *e)
{
	m_canvas = e;
}

void LaserPainter::paint(std::vector<LaserObject> &objects)
{
	m_canvas->clear();

	for (auto it = objects.begin(); it < objects.end(); it++)
	{
		m_canvas->addPoints(it->points());
	}
}

void LaserPainter::add(LaserObject &object)
{
	m_canvas->addPoints(object.points());
}
