#include "InstructionFactory.h"
#include "../Animation.h"

#include "../objects/Line.h"
#include "../objects/Circle.h"
#include "../objects/Rectangle.h"
#include "../objects/Spiral.h"
#include "../objects/CompositeObject.h"

#include <cmath>
#include <cassert>

#include <boost/date_time/time.hpp>

#include <json/value.h>

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
static void calculateRectangleCharacteristics(Point p1, Point p2, float &angle, float &length, Point &start, Point &mid, Point &end)
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

typedef std::vector<std::vector<Point>> PointLUT;

static const PointLUT number_points =
{
	/* ##################### #0 ########################### */
	{Point(5018, 358), Point(6051, 679), Point(6621, 1599),
	Point(6753, 2689), Point(6853, 3784), Point(6831, 4883),
	Point(6608, 5957), Point(6012, 6866), Point(4971, 7204),
	Point(3955, 6849), Point(3384, 5924), Point(3215, 4841),
	Point(3132, 3745), Point(3200, 2648), Point(3444, 1578),
	Point(4041, 669), Point(5018, 358)},
	/* ##################### #1 ########################### */
	{Point(3899, 1121), Point(4947, 788), Point(5996, 456),
	Point(6044, 441), Point(6091, 426), Point(6101, 462),
	Point(6099, 1412), Point(6097, 2512), Point(6094, 3612),
	Point(6092, 4712), Point(6090, 5812), Point(6088, 6912),
	Point(6087, 7174)},
	/* ##################### #2 ########################### */
	{Point(7059, 7117), Point(5959, 7115), Point(4859, 7112),
	Point(3759, 7110), Point(3009, 7108), Point(2959, 7108),
	Point(2964, 7085), Point(3130, 6899), Point(3812, 6036),
	Point(4495, 5173), Point(5178, 4311), Point(5860, 3448),
	Point(6312, 2470), Point(6252, 1386), Point(5559, 544),
	Point(4490, 378), Point(3488, 799), Point(3153, 1004)},
	/* ##################### #3 ########################### */
	{Point(3566, 726), Point(4611, 395), Point(5694, 478),
	Point(6367, 1288), Point(6089, 2320), Point(5176, 2885),
	Point(4471, 3141), Point(4424, 3158), Point(4428, 3162),
	Point(4676, 3135), Point(5748, 3321), Point(6538, 4060),
	Point(6727, 5125), Point(6271, 6109), Point(5393, 6759),
	Point(4325, 6996), Point(3256, 7167)},
	/* ##################### #4 ########################### */
	{Point(7166, 4932), Point(6066, 4934), Point(4966, 4936),
	Point(3866, 4939), Point(2916, 4941), Point(2866, 4941),
	Point(2843, 4926), Point(2870, 4884), Point(3478, 3968),
	Point(4099, 3060), Point(4721, 2152), Point(5342, 1245),
	Point(5964, 337), Point(5964, 337), Point(5992, 296),
	Point(6003, 308), Point(5991, 1308), Point(5977, 2408),
	Point(5963, 3508), Point(5949, 4608), Point(5936, 5708),
	Point(5922, 6808), Point(5918, 7147)},
	/* ##################### #5 ########################### */
	{Point(6852, 383), Point(5752, 412), Point(4652, 441),
	Point(4053, 457), Point(4003, 458), Point(3987, 494),
	Point(3973, 893), Point(3935, 1993), Point(3896, 3092),
	Point(3884, 3442), Point(3882, 3492), Point(3911, 3490),
	Point(4497, 3213), Point(5580, 3153), Point(6388, 3827),
	Point(6718, 4855), Point(6455, 5911), Point(5710, 6705),
	Point(4683, 7062), Point(3585, 7008), Point(3148, 6986)},
	/* ##################### #6 ########################### */
	{Point(6390, 339), Point(5311, 511), Point(4392, 1102),
	Point(3746, 1986), Point(3410, 3025), Point(3213, 4108),
	Point(3175, 5203), Point(3421, 6269), Point(4233, 6955),
	Point(5288, 7140), Point(6261, 6672), Point(6725, 5705),
	Point(6806, 4610), Point(6289, 3665), Point(5280, 3277),
	Point(4218, 3506), Point(3511, 4305), Point(3191, 5006)},
	/* ##################### #7 ########################### */
	{Point(3206, 375), Point(4302, 436), Point(5402, 428),
	Point(6502, 420), Point(6752, 418), Point(6790, 425),
	Point(6767, 470), Point(6422, 1136), Point(5917, 2113),
	Point(5476, 3120), Point(5043, 4132), Point(4610, 5143),
	Point(4178, 6154), Point(3745, 7165), Point(3723, 7216)},
	/* ##################### #8 ########################### */
	{Point(4991, 3539), Point(5995, 3131), Point(6466, 2185),
	Point(6389, 1126), Point(5531, 481), Point(4459, 535),
	Point(3607, 1217), Point(3458, 2285), Point(4027, 3182),
	Point(5060, 3542), Point(6107, 3848), Point(6715, 4726),
	Point(6756, 5801), Point(6204, 6733), Point(5175, 7076),
	Point(4108, 6952), Point(3409, 6167), Point(3187, 5096),
	Point(3621, 4109), Point(4601, 3655), Point(4991, 3539)},
	/* ##################### #9 ########################### */
	{Point(3565, 7175), Point(4642, 7002), Point(5577, 6435),
	Point(6257, 5577), Point(6635, 4551), Point(6741, 3456),
	Point(6836, 2361), Point(6629, 1286), Point(5842, 551),
	Point(4779, 354), Point(3790, 788), Point(3240, 1724),
	Point(3188, 2814), Point(3705, 3743), Point(4684, 4207),
	Point(5755, 4031), Point(6547, 3290), Point(6834, 2338)}
};

static ObjectPtr getDigit(unsigned int i, Point p)
{
	assert(i < number_points.size());
	ObjectPtr polygon = std::make_shared<Polygon>(number_points[i], false, false, false);
	polygon->move(p);
	return polygon;
}

////////////////////////////////////////////////////////////

ObjectPtr InstructionFactory::Wall(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(new Line(p1, p2, true));

	float alpha;
	float length;
	Point midPoint;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, midPoint, end);

	Point p1p2 = p2 - p1;
	ObjectPtr turkerId = getDigit(root.get("turkers",
											Json::Value())
									  .get(0u,		// otherwise ambigious with char*
											Json::Value())
									  .asUInt(),
								  Point(0, 0));
	turkerId->rotate(alpha);
	turkerId->move(midPoint);
	turkerId->move(Point(-p1p2.y(), p1p2.x()) / 10);

	group->add(turkerId);


	return group;
}

ObjectPtr InstructionFactory::Door(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr circle = CompositeObject::construct();
	int radius = sqrt(sqr(p1.x() - p2.x()) + sqr(p1.y() - p2.y()));
	float rad = atan2(p2.y() - p1.y(), p2.x() - p1.y());
	Point middle = p1;
	circle->add(new Circle(middle, radius, 0, M_PI_4));
	circle->rotate(rad, p1);
	return circle;
}

ObjectPtr InstructionFactory::Table(const Json::Value &root, Point p1, Point p2, Point p3, Point p4)
{
	return std::make_shared<Rectangle>(p1, p2, p3, p4, false);
}

ObjectPtr InstructionFactory::Player(const Json::Value &root, Point p)
{
	ObjectPtr c(new Circle(p, 1000));
	c->setPermanent(true);
	return c;
}

ObjectPtr InstructionFactory::Button(const Json::Value &root, Point p)
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

ObjectPtr InstructionFactory::Beam(const Json::Value &root, Point p1, Point p2)
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

	group->rotate(alpha, midPoint);

	return group;
}

static ObjectPtr Portal(const Json::Value &root, Point p1, Point p2, bool active)
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

ObjectPtr InstructionFactory::PortalInactive(const Json::Value &root, Point p1, Point p2)
{ return Portal(root, p1, p2, false); }

ObjectPtr InstructionFactory::PortalActive(const Json::Value &root, Point p1, Point p2)
{ return Portal(root, p1, p2, true); }

ObjectPtr InstructionFactory::Zipline(const Json::Value &root, Point p1, Point p2)
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

ObjectPtr InstructionFactory::Stool(const Json::Value &root, Point p1, Point p2, Point p3, Point p4)
{
	return std::make_shared<Rectangle>(p1, p2, p3, p4, false);
}

ObjectPtr InstructionFactory::Corpse(const Json::Value &root, Point head, Point hip, Point leftHand, Point rightHand)
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

ObjectPtr InstructionFactory::Water(const Json::Value &root, Point p)
{
	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Circle(p, 2000, 3.8*M_PI_4, 9*M_PI_4));
	group->add(new Line(p.x() - 1750, p.y()+500, p.x(), p.y() + 4000));
	group->add(new Line(p.x() + 1750, p.y()+500, p.x(), p.y() + 4000));

	return group;
}

ObjectPtr InstructionFactory::Poke(const Json::Value &root, Point p1, Point p2)
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


	group->addAnimation([](Object *o) {
							o->setVisible(!o->visible());
						}, std::chrono::milliseconds(50));

	return group;
}

ObjectPtr InstructionFactory::Stomper(const Json::Value &root, Point p1, Point p2)
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

ObjectPtr InstructionFactory::Footwear(const Json::Value &root, Point p)
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

ObjectPtr InstructionFactory::Heat(const Json::Value &root, Point p)
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

ObjectPtr InstructionFactory::Elevator(const Json::Value &root, Point p1, Point p2, Point p3)
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

ObjectPtr InstructionFactory::Guardrail(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(new Circle(p1, 325));
	group->add(new Line(p1, p2, true, true));
	group->add(new Circle(p2, 325));
	return group;
}

ObjectPtr InstructionFactory::BlueprintWall(const Json::Value &root, Point p1, Point p2)
{
	Point start = p1 + (p2 - p1) / 10;
	Point end   = p2 + (p1 - p2) / 10;

	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Line(p1, start));
	group->add(new Line(end, p2));

	return group;
}

ObjectPtr InstructionFactory::MovingWallWarning(const Json::Value &root, Point p1, Point p2)
{
	int normalX = root.get("direction", Json::Value()).get("x", Json::Value()).asInt();
	int normalY = root.get("direction", Json::Value()).get("y", Json::Value()).asInt();
	int countdown = root.get("countdown", Json::Value(5000)).asInt();
	Point dir = (p1 + p2) / 2 + Point(normalX, normalY) * 100;
	std::cout << dir << std::endl;

	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Line(p1, p2));
	group->add(new Line(p2, dir));
	group->add(new Line(dir, p1));

	return group;
}

}} // namespace laser::holodeck
