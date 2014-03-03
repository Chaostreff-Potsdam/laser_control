#pragma once

#include "../Object.h"

namespace laser { namespace holodeck {

namespace InstructionFactory {
	ObjectPtr wall(Point p1, Point p2);
	ObjectPtr door(Point p1, Point p2);
	ObjectPtr table(Point p1, Point p2, Point p3, Point p4);
	ObjectPtr player(Point p);
	ObjectPtr button(Point p);

}

}} // namespace laser::holodeck
