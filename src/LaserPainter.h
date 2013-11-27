#ifndef LASERPAINTER_H
#define LASERPAINTER_H

#include "EtherdreamWrapper.h"
#include "LaserObject.h"
#include <utility>
#include <memory>
#include <map>

class LaserPainter
{
	typedef std::pair<int, LaserObjectPtr> LaserObjectPtrPair;
	typedef std::map<int, LaserObjectPtr> LaserObjectPtrMap;

public:
	LaserPainter();

	void aquireEtherdreamWrapper();
	void paintOn(std::shared_ptr<EtherdreamWrapper> e);
	void paint(LaserObjectPtrMap objects);
	void add(LaserObjectPtr object);
	void updatePoints();
	void deleteObject(int id);
	void drawWall(int id, int Ax, int Ay, int Bx, int By, int Px, int Py);

private:
	std::shared_ptr<EtherdreamWrapper> m_canvas;
	LaserObjectPtrMap m_objects;
	int m_smallestFreeId;
};

#endif // LASERPAINTER_H
