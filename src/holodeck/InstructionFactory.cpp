#include "InstructionFactory.h"

#include "../Line.h"
#include "../Circle.h"
#include "../Rectangle.h"
#include "../Spiral.h"
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

ObjectPtr InstructionFactory::beam(Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point midPoint;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, midPoint, end);

	Rectangle *bigRect = new Rectangle(midPoint.x() - 1000,
									   midPoint.y() - length/2,
									   2000, // beam is 1000 thick
									   length, // and as long as requested
									   false);
	group->add(bigRect);

	Rectangle *bottomLeft = new Rectangle(midPoint.x() - 1200,
										  midPoint.y() - length/2 + 200,
										  200,
										  200,
										  false);
	group->add(bottomLeft);

	Rectangle *bottomRight = new Rectangle(midPoint.x() + 1000,
										  midPoint.y() - length/2 + 200,
										  200,
										  200,
										  false);
	group->add(bottomRight);

	Rectangle *topLeft = new Rectangle(midPoint.x() - 1200,
										  midPoint.y() + length/2 - 400,
										  200,
										  200,
										  false);
	group->add(topLeft);

	Rectangle *topRight = new Rectangle(midPoint.x() + 1000,
										  midPoint.y() + length/2 - 400,
										  200,
										  200,
										  false);
	group->add(topRight);

	group->rotate(alpha, midPoint);

	return group;
}

ObjectPtr InstructionFactory::portal(Point p1, Point p2, bool active)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point mid;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, mid, end);

	if (active)
	{
		group->add(new Line(start.x(),
							start.y(),
							start.x() + length/3,
							start.y()));
		group->add(new Spiral(mid.x(), mid.y() + length/8, 200, length/4, 3, active));
		group->add(new Line(end.x() - length/3,
							end.y(),
							end.x(),
							end.y()));
	}
	else
	{
		group->add(new Line(start.x(),
							start.y(),
							end.x(),
							end.y()));
		group->add(new Spiral(mid.x(), mid.y() + length/8, 200, length/4, 3, active));
	}

	group->rotate(alpha, mid);

	return group;
}

ObjectPtr InstructionFactory::zipline(Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point mid;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, mid, end);

	group->add(new Line(start.x() + length/6, start.y() - 200,
						start.x(),            start.y() - 200));
	group->add(new Line(start.x(),            start.y() - 200,
						start.x(),            start.y() + 200));
	group->add(new Line(start.x(),            start.y() + 200,
						start.x() + length/6, start.y() + 200));

	group->add(new Circle(end, length/3, M_PI-0.1, 5*M_PI_4));
	std::vector<Point> zipIconPoints;
	zipIconPoints.push_back(Point(end.x() - length/3, start.y()));
	zipIconPoints.push_back(Point(mid.x() - length/6, mid.y() - length/6));
	zipIconPoints.push_back(Point(mid.x() - length/6, mid.y() + length/6));
	group->add(new Polygon(zipIconPoints, false));

	group->add(new Line(mid.x() - length/10, mid.y() - length/10,
						mid.x() - length/10, mid.y() + length/10));

	group->add(new Line(end.x() - length/6, end.y() - 200,
						end.x(),            end.y() - 200));
	group->add(new Line(end.x(),            end.y() - 200,
						end.x(),            end.y() + 200));
	group->add(new Line(end.x(),            end.y() + 200,
						end.x() - length/6, end.y() + 200));

	group->rotate(alpha, mid);

	return group;
}

ObjectPtr InstructionFactory::stool(Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point mid;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, mid, end);

	group->add(new Rectangle(start.x(), start.y() - length/2,
							 length, length, false));
	group->add(new Line(start.x() + length/6, start.y() - length/2 + 500,
						start.x() + length/6, start.y() + length/2 + 500));

	group->rotate(alpha);

	return group;
}

ObjectPtr InstructionFactory::corpse(Point head, Point hip, Point leftHand, Point rightHand)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point mid;
	Point start;
	Point end;

	calculateRectangleCharacteristics(head, hip, alpha, length, start, mid, end);

	group->add(new Circle(head, 2000));
	group->add(new Line(head, hip));

	group->add(new Circle(leftHand, 1000));
	group->add(new Line(leftHand, mid));

	group->add(new Circle(rightHand, 1000));
	group->add(new Line(rightHand, mid));

	return group;
}

void InstructionFactory::calculateRectangleCharacteristics(Point p1, Point p2, float &angle, float &length, Point &start, Point &mid, Point &end)
{
	// calculate rotation
	int dx = p2.x() - p1.x();
	int dy = p2.y() - p1.y();
	angle = atan2(dy, dx);

	// length of wall
	length = sqrt(sqr(dx) + sqr(dy));

	start.setX((p1.x() + p2.x()) / 2 - length/2);
	start.setY((p1.y() + p2.y()) / 2);

	end.setX((p1.x() + p2.x()) / 2 + length/2);
	end.setY((p1.y() + p2.y()) / 2);

	mid.setX((p1.x() + p2.x()) / 2);
	mid.setY((p1.y() + p2.y()) / 2);
}

ObjectPtr InstructionFactory::water(Point p)
{
	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Circle(p, 2000, 3.8*M_PI_4, 9*M_PI_4));
	group->add(new Line(p.x() - 1750, p.y()+500, p.x(), p.y() + 4000));
	group->add(new Line(p.x() + 1750, p.y()+500, p.x(), p.y() + 4000));

	//group->rotate(M_PI, p);

	return group;
}

}} // namespace laser::holodeck
