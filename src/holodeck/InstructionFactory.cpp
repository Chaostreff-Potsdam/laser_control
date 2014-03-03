#include "InstructionFactory.h"

#include "../Line.h"
#include "../Circle.h"
#include "../Rectangle.h"
#include "../CompositeObject.h"

#include <cmath>

namespace laser { namespace holodeck {

ObjectPtr InstructionFactory::wall(Point p1, Point p2)
{
	ObjectPtr w(new Line(p1, p2, true));
	w->setPermanent(true);
	return w;
}

ObjectPtr InstructionFactory::door(Point p1, Point p2)
{
	CompositeObjectPtr circle = CompositeObject::construct();
	int radius = sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
	float rad = atan2(p2.y() - p1.y(), p2.x() - p1.y());
	Point middle = p1;
	circle->add(new Circle(middle, radius, 0, M_PI_4));
	circle->rotate(rad, p1);
	return circle;
}

ObjectPtr InstructionFactory::table(Point p1, Point p2, Point p3, Point p4)
{
	return std::make_shared<Rectangle>(p1, p2, p3, p4, false);
}

ObjectPtr InstructionFactory::player(Point p)
{
	ObjectPtr c(new Circle(p, 1000));
	c->setPermanent(true);
	return c;
}

ObjectPtr InstructionFactory::button(Point p)
{
	CompositeObjectPtr group = CompositeObject::construct();

	std::vector<Point> ps;

	ps.push_back(Point(p.x() - 6000, p.y() - 4000));
	ps.push_back(Point(p.x() + 6000, p.y() - 4000));
	ps.push_back(Point(p.x(), p.y() + 6000));

	group->add(new Polygon(ps, false, true));

	group->add(new Rectangle(p.x() - 1000, p.y() - 1000, 2000, 2000, false));
	group->add(new Circle(p.x(), p.y() + 1000, 500, 0, M_PI));
	return group;
}

}} // namespace laser::holodeck
