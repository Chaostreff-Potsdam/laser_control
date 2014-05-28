#include "InstructionFactory.h"

#include "../objects/Line.h"
#include "../objects/Circle.h"
#include "../objects/Rectangle.h"
#include "../objects/Spiral.h"
#include "../objects/CompositeObject.h"

#include <cmath>

#include <boost/date_time/time.hpp>

namespace laser { namespace holodeck {

static void ShiftPoints(std::vector<Point> & points, Point base)
{
	for (Point & p: points)
		p += base;
}

/*!
 * \brief determine characteristic values for a rectangle
 * \image html calculateRectangleCharacteristics.svg
 * \param[in] p1 midpoint of one short side of the rectangle
 * \param[in] p2 midpoint of the other short side
 * \param[out] angle angle between x-axis and line between \a p1 and \a p2
 * \param[out] length \f$ \mbox{length} = \sqrt{(\mbox{p1}_x - \mbox{p2}_x)^2 + (\mbox{p1}_y - \mbox{p2}_y)^2} \f$
 * \param[out] start start point of axis aligned bounding box
 * \param[out] mid mid point of axis aligned bounding box
 * \param[out] end end point of axis aligned bounding box
 */
void calculateRectangleCharacteristics(Point p1, Point p2, float &angle, float &length, Point &start, Point &mid, Point &end)
{
	// calculate rotation
	int dx = p2.x() - p1.x();
	int dy = p2.y() - p1.y();
	angle = atan2(dy, dx);

	// length of wall
	length = sqrt(sqr(dx) + sqr(dy));

	mid = (p1 + p2) / 2;
	start = mid - Point(0, length / 2);
	end   = mid + Point(0, length / 2);
}

////////////////////////////////////////////////////////////

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
	ps.emplace_back(-6000, -4000);
	ps.emplace_back( 6000, -4000);
	ps.emplace_back(    0,  6000);
	ShiftPoints(ps, p);

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

static ObjectPtr Portal(Point p1, Point p2, bool active)
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
		group->add(new Line(start, start + Point(length / 3, 0)));
		group->add(new Spiral(mid.x(), mid.y() + length/8, 200, length/4, 3, active));
		group->add(new Line(end - Point(length/3, 0), end));
	}
	else
	{
		group->add(new Line(start, end));
		group->add(new Spiral(mid.x(), mid.y() + length/8, 200, length/4, 3, active));
	}

	group->rotate(alpha, mid);

	return group;
}

ObjectPtr InstructionFactory::PortalInactive(Point p1, Point p2)
{ return Portal(p1, p2, false); }

ObjectPtr InstructionFactory::PortalActive(Point p1, Point p2)
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
	groupA->add(new Circle(p + Point(1000, 1000), 300));


	std::vector<Point> points;
	points.emplace_back(+ 300,  700);
	points.emplace_back(+ 200, 1400);
	points.emplace_back(    0,  700);
	points.emplace_back(- 200, 1400);
	points.emplace_back(- 300,  700);
	points.emplace_back(- 500, 1400);
	points.emplace_back(- 700,  700);
	//points.emplace_back(+ 500,  700);
	ShiftPoints(points, p);

	groupA->add(new Polygon(points, false));

	groupA->move(-p);
	groupA->scale(1, 2);
	groupA->move(p);

	groupA->move(-1500, 0);


	groupB->add(new Circle(p, 1000));
	groupB->add(new Circle(p + Point(1000, 1000), 300));


	points.clear();
	points.emplace_back(+ 300,  700);
	points.emplace_back(+ 200, 1400);
	points.emplace_back(    0,  700);
	points.emplace_back(- 200, 1400);
	points.emplace_back(- 300,  700);
	points.emplace_back(- 500, 1400);
	points.emplace_back(- 700,  700);
	//points.emplace_back(+ 500, 700);
	ShiftPoints(points, p);

	groupB->add(new Polygon(points, false));

	groupB->move(-p);
	groupB->scale(-1, 2);
	groupB->move(p);

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

	points.emplace_back(-1750,  500);
	points.emplace_back(-1500, 1000);
	points.emplace_back(-1000,  750);
	points.emplace_back(    0, 2000);
	points.emplace_back(  500, 1000);
	points.emplace_back( 1250, 4000);
	points.emplace_back( 1750,  500);
	ShiftPoints(points, p);

	group->add(new Polygon(points, false, false, false));

    return group;
}

ObjectPtr InstructionFactory::Elevator(Point p1, Point p2, Point p3)
{
	CompositeObjectPtr group = CompositeObject::construct();

	Point s12 = p2 - p1;
	Point s13 = p3 - p1;
	Point s23 = p3 - p2;
	Point arrowOneBottom = p1 + s12 / 5   + s23 / 5;
	Point arrowOneTop    = p1 + s12 / 5   + s23 * 0.8;
	Point arrowTwoBottom = p1 + s12 * 0.8 + s23 / 5;
	Point arrowTwoTop    = p1 + s12 * 0.8 + s23 * 0.8;

	Point arrowOneTopTipRight = arrowOneTop - s13 * 0.1;
	Point arrowOneTopTipLeft  = arrowOneTop + Point(s13.x(), -s13.y()) * 0.1;
	Point arrowTwoBottomTipRight = arrowTwoBottom + s13 * 0.1;
	Point arrowTwoBottomTipLeft = arrowTwoBottom + Point(-s13.x(), s13.y()) * 0.1;

	group->add(new Rectangle(p1, p2, p3, p3 - s12, false));
	group->add(new Line(arrowOneBottom, arrowOneTop));
	group->add(new Line(arrowOneTop, arrowOneTopTipRight));
	group->add(new Line(arrowOneTop, arrowOneTopTipLeft));
	group->add(new Line(arrowTwoBottom, arrowTwoTop));
	group->add(new Line(arrowTwoBottom, arrowTwoBottomTipRight));
	group->add(new Line(arrowTwoBottom, arrowTwoBottomTipLeft));

	return group;
}

ObjectPtr InstructionFactory::Guardrail(Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(new Circle(p1, 325));
	group->add(new Line(p1, p2, true, true));
	group->add(new Circle(p2, 325));
	return group;
}

}} // namespace laser::holodeck
