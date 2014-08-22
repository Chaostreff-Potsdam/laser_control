#include "InstructionFactory.h"
#include "../animations/Animation.h"

#include "../objects/Line.h"
#include "../objects/Circle.h"
#include "../objects/Rectangle.h"
#include "../objects/Spiral.h"
#include "../objects/CompositeObject.h"

#include <chrono>
#include <cmath>
#include <cassert>

#include <boost/date_time/time.hpp>

#include <json/value.h>

namespace laser { namespace holodeck {

const Color MetaDataColor(Color::LIGHTBLUE);

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
	{Point(1255, 394), Point(1492, 460), Point(1639, 657), Point(1682, 902),
	Point(1704, 1151), Point(1716, 1401), Point(1690, 1649), Point(1612, 1885),
	Point(1434, 2049), Point(1193, 2103), Point(970, 2000), Point(846, 1786),
	Point(806, 1540), Point(783, 1291), Point(792, 1041), Point(832, 795),
	Point(927, 565), Point(1127, 431), Point(1255, 394)},
	/* ##################### #1 ########################### */
	{Point(975, 581), Point(1213, 505), Point(1451, 429), Point(1511, 411),
	Point(1523, 407), Point(1525, 416), Point(1525, 578), Point(1524, 828),
	Point(1524, 1078), Point(1523, 1328), Point(1523, 1578), Point(1522, 1828),
	Point(1522, 2078), Point(1522, 2094)},
	/* ##################### #2 ########################### */
	{Point(1765, 2095), Point(1515, 2094), Point(1265, 2094), Point(1015, 2093),
	Point(765, 2093), Point(752, 2093), Point(740, 2093), Point(741, 2087),
	Point(876, 1923), Point(1031, 1727), Point(1186, 1531), Point(1341, 1335),
	Point(1496, 1138), Point(1582, 908), Point(1563, 662), Point(1412, 464),
	Point(1172, 406), Point(936, 476), Point(788, 567)},
	/* ##################### #3 ########################### */
	{Point(891, 491), Point(1128, 413), Point(1376, 413), Point(1568, 560),
	Point(1572, 803), Point(1411, 988), Point(1176, 1073), Point(1118, 1095),
	Point(1106, 1099), Point(1107, 1100), Point(1269, 1094), Point(1502, 1177),
	Point(1656, 1370), Point(1678, 1615), Point(1568, 1837), Point(1371, 1988),
	Point(1131, 2050), Point(884, 2090), Point(814, 2101)},
	/* ##################### #4 ########################### */
	{Point(1792, 1555), Point(1542, 1555), Point(1292, 1556), Point(1042, 1556),
	Point(792, 1557), Point(729, 1557), Point(717, 1557), Point(711, 1553),
	Point(799, 1417), Point(940, 1211), Point(1081, 1004), Point(1223, 798),
	Point(1364, 592), Point(1491, 406), Point(1498, 396), Point(1501, 399),
	Point(1498, 649), Point(1495, 899), Point(1491, 1149), Point(1488, 1399),
	Point(1485, 1649), Point(1482, 1899), Point(1479, 2109)},
	/* ##################### #5 ########################### */
	{Point(1713, 408), Point(1463, 415), Point(1213, 421), Point(1013, 427),
	Point(1001, 427), Point(997, 436), Point(996, 461), Point(987, 711),
	Point(978, 961), Point(971, 1173), Point(971, 1185), Point(978, 1185),
	Point(988, 1179), Point(1221, 1092), Point(1465, 1127), Point(1620, 1314),
	Point(1679, 1551), Point(1614, 1790), Point(1448, 1975), Point(1220, 2073),
	Point(971, 2068), Point(787, 2059)},
	/* ##################### #6 ########################### */
	{Point(1598, 397), Point(1351, 432), Point(1135, 554), Point(974, 744),
	Point(870, 971), Point(826, 1217), Point(790, 1463), Point(806, 1713),
	Point(892, 1945), Point(1103, 2075), Point(1347, 2094), Point(1565, 1981),
	Point(1679, 1764), Point(1701, 1515), Point(1624, 1283), Point(1418, 1148),
	Point(1171, 1145), Point(953, 1261), Point(836, 1480), Point(798, 1564)},
	/* ##################### #7 ########################### */
	{Point(802, 395), Point(1051, 410), Point(1301, 408), Point(1551, 407),
	Point(1688, 406), Point(1697, 407), Point(1692, 418), Point(1652, 496),
	Point(1537, 718), Point(1428, 943), Point(1330, 1173), Point(1231, 1403),
	Point(1133, 1633), Point(1035, 1863), Point(936, 2092), Point(931, 2105)},
	/* ##################### #8 ########################### */
	{Point(1248, 1191), Point(1479, 1104), Point(1608, 901), Point(1628, 656),
	Point(1474, 465), Point(1234, 410), Point(1014, 513), Point(867, 704),
	Point(885, 949), Point(1053, 1120), Point(1290, 1194), Point(1527, 1268),
	Point(1669, 1465), Point(1699, 1707), Point(1603, 1935), Point(1391, 2052),
	Point(1146, 2081), Point(923, 1975), Point(828, 1751), Point(809, 1506),
	Point(942, 1300), Point(1174, 1213), Point(1248, 1191)},
	/* ##################### #9 ########################### */
	{Point(891, 2102), Point(1137, 2068), Point(1356, 1949), Point(1525, 1767),
	Point(1635, 1543), Point(1673, 1297), Point(1697, 1048), Point(1700, 799),
	Point(1622, 564), Point(1417, 423), Point(1170, 400), Point(948, 506),
	Point(817, 716), Point(790, 963), Point(870, 1197), Point(1076, 1336),
	Point(1320, 1355), Point(1542, 1247), Point(1670, 1037), Point(1708, 893)}
};

static ObjectPtr getDigit(const Json::Value &root, unsigned int i, Point p = Point(0, 0))
{
	unsigned int id = root.get("turkers",
								Json::Value())
						  .get(i,
								Json::Value())
						  .asUInt();
	assert(id < number_points.size());
	ObjectPtr polygon = std::make_shared<Polygon>(number_points[id], false, false, false);
	polygon->setColor(MetaDataColor);
	polygon->move(p);
	return polygon;
}

static ObjectPtr MovingIndicator(const Point & p1, double angle)
{
	const double width = 700, height = 750;

	const double right = p1.x() + width * 0.5;
	const double left = p1.x() - width * 0.5;
	const double top = p1.y() - height * 0.5;
	const double bottom = p1.y() + height * 0.5;

	CompositeObjectPtr group = CompositeObject::construct();
	Line *lA0 = new Line(Point(left, bottom), p1),
		 *lA1 = new Line(p1, Point(right, bottom)),
		 *lB0 = new Line(right, p1.y(), p1.x(), top),
		 *lB1 = new Line(p1.x(), top, left, p1.y());

	group->add(lA0);
	group->add(lA1);
	group->add(lB0);
	group->add(lB1);

	group->setColor(MetaDataColor);
	group->rotate(angle, p1);
	group->addAnimation([=](Object *){
		// What about static's semantic?
		static int step = 3;
		lA0->setVisible((bool)(step & 1) != (bool)(step & 2));
		lA1->setVisible((bool)(step & 1) != (bool)(step & 2));

		lB0->setVisible(step & 2);
		lB1->setVisible(step & 2);
		step = (step + 1) & 3;
	}, Animation::msecs(200));
	return group;
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
	ObjectPtr turkerId = getDigit(root, 0);
	turkerId->rotate(alpha);
	turkerId->move(midPoint - p1p2 / 2);
	turkerId->move(Point(-p1p2.y(), p1p2.x()).norm() * 100);

	group->add(turkerId);

	return group;
}

ObjectPtr InstructionFactory::Door(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr door = CompositeObject::construct();

	door->add(new Line(p1, p2));
	door->add(new Circle(p2, 750));
	return door;
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

ObjectPtr InstructionFactory::Switch(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();

	static const double handleSize = 1500.0;
	group->add(new Circle(p2, handleSize));

	Point stickDirection(p2 - p1);
	Point attachmentPoint = stickDirection.norm() * handleSize;
	group->add(new Line(p1, attachmentPoint));

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
	bigRect->rotate(alpha-M_PI_2, midPoint);
	group->add(bigRect);

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
	static const double innerCircle = 500.0;
	static const double outerCircle = 1000.0;

	group->add(new Circle(p1, outerCircle));
	group->add(new Circle(p1, innerCircle));

	group->add(new Line(p1, p2));

	group->add(new Circle(p2, innerCircle));
	group->add(new Circle(p2, outerCircle));

	return group;
}

ObjectPtr InstructionFactory::Stool(const Json::Value &root, Point p1, Point p2, Point p3, Point p4)
{
	return std::make_shared<Rectangle>(p1, p2, p3, p4, false);
}

ObjectPtr InstructionFactory::Corpse(const Json::Value &root, Point head, Point chest)
{
	CompositeObjectPtr corpse = CompositeObject::construct();

	const double spineRot = atan2(head.y() - chest.y(), head.x() - chest.x());
	const double bodyAspectRatio = 0.702;
	const double spine = (head - chest).abs(); // bodyHeight
	const double bodyWidth = bodyAspectRatio * spine * 2;

	ObjectPtr body(new Circle(chest, spine, M_PI));
	body->scale(bodyAspectRatio, 1.0);

	corpse->add(body);
	corpse->add(new Circle(chest + Point(0, -spine), 0.4 * spine, radians(158), radians(382)));

	Point armStart = chest + Point(bodyWidth * (3.0 / 8), -spine * 1.0/3);
	Point ellbow(chest.x() + bodyWidth * 0.5, head.y());
	Point upperArm(ellbow - armStart);

	corpse->add(new Line(armStart, ellbow));
	corpse->add(new Line(ellbow, ellbow + upperArm.scaled(-1, 1)));

	std::cerr << M_PI_2 + spineRot << std::endl;
	corpse->rotate(M_PI_2 + spineRot, chest);

	CompositeObjectPtr group = CompositeObject::construct(); // Safely store transform on corpse
	group->add(corpse);
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

ObjectPtr InstructionFactory::Guardrail(const Json::Value &root, Point p1, Point p2, Point p3, Point p4)
{
	CompositeObjectPtr group = CompositeObject::construct();

	float alpha;
	float length;
	Point midPoint;
	Point start;
	Point end;

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, midPoint, end);

	Point p1p2 = p2 - p1;
	ObjectPtr turkerId = getDigit(root, 0);

	turkerId->rotate(alpha);
	turkerId->move(midPoint - p1p2 / 2);

	turkerId->move(Point(-p1p2.y(), p1p2.x()).norm() * 100);
	group->add(turkerId);
	group->add(new Rectangle(p1, p2, p3, p4, false));
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
	Point arrowTop = (p1 + p2) / 2 + Point(normalX, normalY).norm() * 1000;
	Point arrowEnd = arrowTop
					 - Point(normalX, normalY).norm() * 500
					 + (p2 - p1).norm() * 250;

	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Line(p1, p2));

	std::vector<Point> arrowPoints {(p1 + p2) / 2, arrowTop, arrowEnd};

	ObjectPtr arrow = std::make_shared<Polygon>(arrowPoints, false, false, false);
	arrow->addAnimation([] (Object *me) {
										static bool visible = true;
										visible = !visible;
										me->setVisible(visible);
									},
						std::chrono::milliseconds(500)
	);
	group->add(arrow);


	return group;
}

}} // namespace laser::holodeck
