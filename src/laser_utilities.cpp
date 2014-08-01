#include "laser_utilities.h"


std::ostream& operator<< (std::ostream& stream, const laser::Point& point)
{
	stream << "(" << point.x() << "; " << point.y() << ")";
	return stream;
}
