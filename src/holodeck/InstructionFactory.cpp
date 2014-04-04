#include "InstructionFactory.h"

#include "../Line.h"
#include "../Circle.h"
#include "../Rectangle.h"
#include "../Spiral.h"
#include "../CompositeObject.h"

#include <cmath>

#include <boost/date_time/time.hpp>

namespace laser { namespace holodeck {

ObjectPtr InstructionFactory::Wall(Point p1, Point p2)
{
	ObjectPtr w(new Line(p1, p2, true));
	w->setPermanent(true);
	return w;
}

ObjectPtr InstructionFactory::Door(Point p1, Point p2)
{
	CompositeObjectPtr circle = CompositeObject::construct();
	int radius = sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
	float rad = atan2(p2.y() - p1.y(), p2.x() - p1.y());
	Point middle = p1;
	circle->add(new Circle(middle, radius, 0, M_PI_4));
	circle->rotate(rad, p1);
	return circle;
}

ObjectPtr InstructionFactory::Table(Point p1, Point p2, Point p3, Point p4)
{
	return std::make_shared<Rectangle>(p1, p2, p3, p4, false);
}

ObjectPtr InstructionFactory::Player(Point p)
{
	ObjectPtr c(new Circle(p, 1000));
	c->setPermanent(true);
	return c;
}

ObjectPtr InstructionFactory::Button(Point p)
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

ObjectPtr InstructionFactory::Beam(Point p1, Point p2)
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

static ObjectPtr InstructionFactory::Portal(Point p1, Point p2, bool active)
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

ObjectPtr InstructionFactory::InActivePortal(Point p1, Point p2)
{ return Portal(p1, p2, false); }

ObjectPtr InstructionFactory::ActivePortal(Point p1, Point p2)
{ return Portal(p1, p2, true); }

ObjectPtr InstructionFactory::Zipline(Point p1, Point p2)
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

ObjectPtr InstructionFactory::Stool(Point p1, Point p2)
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

ObjectPtr InstructionFactory::Corpse(Point head, Point hip, Point leftHand, Point rightHand)
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

ObjectPtr InstructionFactory::Water(Point p)
{
	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Circle(p, 2000, 3.8*M_PI_4, 9*M_PI_4));
	group->add(new Line(p.x() - 1750, p.y()+500, p.x(), p.y() + 4000));
	group->add(new Line(p.x() + 1750, p.y()+500, p.x(), p.y() + 4000));

	return group;
}

ObjectPtr InstructionFactory::Poke(Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point mid;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, mid, end);

	group->add(new Rectangle(start.x(), start.y()-500, length, 1000, false));
	group->add(new Rectangle(start.x()+3000, start.y()-3000, length-3000, 2500, false));
	group->add(new Rectangle(mid.x()+2000, start.y(), length/2-2000, 2500, false));
	group->add(new Line(start.x()+3000, start.y() -2000, mid.x()+2000, start.y() -2000));
	group->add(new Line(start.x()+3000, start.y() -1000, mid.x()+2000, start.y() -1000));
	group->rotate(alpha, mid);

	group->setAnimation([group]()
						{
							static int ticks = 0;
							if (ticks > 20) // every second
							{
								group->setVisible(!group->visible());
								ticks = 0;
							}
							ticks++;
						});

	return group;
}

ObjectPtr InstructionFactory::Stomper(Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point mid;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, mid, end);

	group->add(new Line(start.x(), start.y(), end.x()-2000, end.y()));
	group->add(new Line(end.x()-2000, end.y(), end.x()-3000, end.y()+1000));
	group->add(new Line(end.x()-3000, end.y()-1000, end.x()-2000, end.y()));
	group->add(new Line(end.x()-1000, end.y()-2000, end.x()-1000, end.y()+2000));
	group->rotate(alpha, mid);

	return group;
}

ObjectPtr InstructionFactory::Footwear(Point p)
{
	CompositeObjectPtr group = CompositeObject::construct();
	CompositeObjectPtr groupA = CompositeObject::construct();
	CompositeObjectPtr groupB = CompositeObject::construct();

	groupA->add(new Circle(p, 1000));
	groupA->add(new Circle(p.x() + 1000, p.y() + 1000, 300));


	std::vector<Point> points;

	points.push_back(Point(p.x() + 300, p.y() + 700));
	points.push_back(Point(p.x() + 200, p.y() + 1400));
	points.push_back(Point(p.x()      , p.y() + 700));
	points.push_back(Point(p.x() - 200, p.y() + 1400));
	points.push_back(Point(p.x() - 300, p.y() + 700));
	points.push_back(Point(p.x() - 500, p.y() + 1400));
	points.push_back(Point(p.x() - 700, p.y() + 700));
	//points.push_back(Point(p.x() + 500, p.y() + 700));

	groupA->add(new Polygon(points, false));

	groupA->move(-p.x(), -p.y());
	groupA->scale(1, 2);
	groupA->move(p.x(), p.y());

	groupA->move(-1500, 0);


	groupB->add(new Circle(p, 1000));
	groupB->add(new Circle(p.x() + 1000, p.y() + 1000, 300));


	std::vector<Point> pointsB;

	pointsB.push_back(Point(p.x() + 300, p.y() + 700));
	pointsB.push_back(Point(p.x() + 200, p.y() + 1400));
	pointsB.push_back(Point(p.x()      , p.y() + 700));
	pointsB.push_back(Point(p.x() - 200, p.y() + 1400));
	pointsB.push_back(Point(p.x() - 300, p.y() + 700));
	pointsB.push_back(Point(p.x() - 500, p.y() + 1400));
	pointsB.push_back(Point(p.x() - 700, p.y() + 700));
	//points.push_back(Point(p.x() + 500, p.y() + 700));

	groupB->add(new Polygon(pointsB, false));

	groupB->move(-p.x(), -p.y());
	groupB->scale(-1, 2);
	groupB->move(p.x(), p.y());

	groupB->move(1500, 0);

	group->add(groupA);
	group->add(groupB);

	return group;
}

ObjectPtr InstructionFactory::Heat(Point p)
{
	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Circle(p, 2000, 3.8*M_PI_4, 9*M_PI_4));


	std::vector<Point> points;

	points.push_back(Point(p.x() - 1750, p.y() + 500));
	points.push_back(Point(p.x() - 1500, p.y() + 1000));
	points.push_back(Point(p.x() - 1000, p.y() + 750));
	points.push_back(Point(p.x()       , p.y() + 2000));
	points.push_back(Point(p.x() + 500, p.y() + 1000));
	points.push_back(Point(p.x() + 1250, p.y() + 4000));
	points.push_back(Point(p.x() + 1750, p.y() + 500));

	group->add(new Polygon(points, false, false, false));

    return group;
}

ObjectPtr InstructionFactory::Elevator(Point p1, Point p2, Point p3)
{
    CompositeObjectPtr group = CompositeObject::construct();

    Point arrowOneBottom((p1.x() - p2.x())/5 + (p2.x() - p3.x())/5, (p1.y() - p2.y())/5 + (p2.y() - p3.y())/5);
    Point arrowOneTop((p1.x() - p2.x())/5 + (p2.x() - p3.x())*4/5, (p1.y() - p2.y())/5 + (p2.y() - p3.y())*4/5);
    Point arrowTwoBottom((p1.x() - p2.x())*4/5 + (p2.x() - p3.x())/5, (p1.y() - p2.y())*4/5 + (p2.y() - p3.y())/5);
    Point arrowTwoTop((p1.x() - p2.x())*4/5 + (p2.x() - p3.x())*4/5, (p1.y() - p2.y())*4/5 + (p2.y() - p3.y())*4/5);
    Point arrowOneTopTipRight(arrowOneTop.x() - 1.0/10 * (p1.x() - p3.x()), arrowOneTop.y() - 1.0/10 * (p1.y() - p3.y()));
    Point arrowOneTopTipLeft(arrowOneTop.x() + 1.0/10 * (p1.x() - p3.x()), arrowOneTop.y() - 1.0/10 * (p1.y() - p3.y()));
    Point arrowTwoBottomTipRight(arrowTwoBottom.x() + 1.0/10 * (p1.x() - p3.x()), arrowTwoBottom.y() + 1.0/10 * (p1.y() - p3.y()));
    Point arrowTwoBottomTipLeft(arrowTwoBottom.x() - 1.0/10 * (p1.x() - p3.x()), arrowTwoBottom.y() + 1.0/10 * (p1.y() - p3.y()));
    //Point arrowOneTopTipLeft();

    group->add(new Rectangle(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y(), p3.x()+(p1.x() - p2.x()), p3.y()+(p1.y() - p2.y()), false));
    group->add(new Line(arrowOneBottom, arrowOneTop));
    group->add(new Line(arrowOneTop, arrowOneTopTipRight));
    group->add(new Line(arrowOneTop, arrowOneTopTipLeft));
    group->add(new Line(arrowTwoBottom, arrowTwoTop));
    group->add(new Line(arrowTwoBottom, arrowTwoBottomTipRight));
    group->add(new Line(arrowTwoBottom, arrowTwoBottomTipLeft));


    std::cout << arrowOneTopTipRight.x() << " " << arrowOneTopTipRight.y() << std::endl;


    return group;
}

}} // namespace laser::holodeck
