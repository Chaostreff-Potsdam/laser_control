#include "ObjectSorting.h"

namespace laser {

static double distanceBetween(const etherdream_point & point, const ObjectPtr & target)
{
	if (!target)
		return 0;

	const auto targetPoints = target->pointsToPaint();
	if (targetPoints.empty())
		return 0;

	const auto firstPoint = targetPoints.front();
	return std::sqrt(sqr(point.x - firstPoint.x) + sqr(point.y - firstPoint.y));
}

void sortObjects(std::vector<ObjectPtr> & objects)
{
	if (objects.empty())
		return;

	// First one stays, after that always the object whose startpoints
	// are closests to last objects endpoints
	//
	// Using stupid O(n^2) SelectionSort

	typedef std::vector<ObjectPtr>::iterator ObjIter;

	etherdream_point lastPoint;
	memset(&lastPoint, 0, sizeof(lastPoint));

	auto updateLastPoint = [&](const ObjectPtr & obj) {
		const auto targetPoints = obj->pointsToPaint();
		if (!targetPoints.empty())
			lastPoint = targetPoints.back();
	};

	updateLastPoint(objects.front());

	const int sz = objects.size();
	for (int i = 1; i < sz - 1; ++i) {
		ObjIter targetSlot = objects.begin() + i;

		ObjIter nextObject = std::min(targetSlot, objects.end() - 1,
				 [&](const ObjIter a, const ObjIter b) {
					return distanceBetween(lastPoint, *a) <	distanceBetween(lastPoint, *b);
				});

		std::iter_swap(targetSlot, nextObject);
		updateLastPoint(*targetSlot);
	}
}

}
