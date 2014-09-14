#include "InstructionFactory.h"
#include "InstructionOptions.h"
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

static ObjectPtr getDigit(const Json::Value &root, unsigned int i, Point p = Point(0, 0), double rotation = 0.0)
{
	unsigned int id = root.get("turkers",
								Json::Value())
						  .get(i,
								Json::Value())
						  .asUInt();
	CompositeObjectPtr digit = opts::getDigit(id);
	digit->rotate(rotation, Point(250, 500));
	digit->scale(-1, 1);
	digit->move(1000, 0);
	digit->move(p);
	digit->setColor(MetaDataColor);
	return digit;
}

static ObjectPtr MovingIndicator(const Point & p1, double angle)
{
	const double right = p1.x() + opts::IndicatorWidth * 0.5;
	const double left = p1.x() - opts::IndicatorWidth * 0.5;
	const double top = p1.y() - opts::IndicatorHeight * 0.5;
	const double bottom = p1.y() + opts::IndicatorHeight * 0.5;

	CompositeObjectPtr group = CompositeObject::construct();
	Line *lA0 = new Line(Point(left, top), p1),
		 *lA1 = new Line(p1, Point(right, top)),
		 *lB0 = new Line(right, p1.y(), p1.x(), bottom),
		 *lB1 = new Line(p1.x(), bottom, left, p1.y());

	group->add(lA0);
	group->add(lA1);
	group->add(lB0);
	group->add(lB1);

	group->setColor(MetaDataColor);
	group->rotate(angle, p1);

	std::shared_ptr<int> storedStep(new int);
	*storedStep = 3;
	group->addAnimation([=](Object *){
		// What about static's semantic?
		int step = *storedStep;
		lA0->setVisible((bool)(step & 1) != (bool)(step & 2));
		lA1->setVisible((bool)(step & 1) != (bool)(step & 2));

		lB0->setVisible(step & 2);
		lB1->setVisible(step & 2);
		*storedStep = (step + 1) & 3;
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
	if(turkerId){
		turkerId->rotate(alpha);
		turkerId->move(midPoint - p1p2 / 2);
		turkerId->move(Point(-p1p2.y(), p1p2.x()).norm() * 100);

		group->add(turkerId);
	}

	return group;
}

ObjectPtr InstructionFactory::MovingWall(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr group = std::dynamic_pointer_cast<CompositeObject>(Wall(root, p1, p2));

	Point direction = p2 - p1;
	Point mid = p1 + direction * 0.5;
	Point spacer = direction.perpendicular().norm() * opts::IndicatorDistance;

	group->add(MovingIndicator(mid + spacer, direction.angle()));

	return group;
}

ObjectPtr InstructionFactory::Door(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr door = std::dynamic_pointer_cast<CompositeObject>(Wall(root, p1, p2));
	door->add(new Circle(p2, 750));

	return door;
}

ObjectPtr InstructionFactory::Table(const Json::Value &root, Point p1, Point p2, Point p3, Point p4)
{
	CompositeObjectPtr group = CompositeObject::construct();
	const Point center = (p1 + p2 + p3 + p4) * 0.25;
	group->add(new Rectangle(p1, p2, p3, p4, false));

	ObjectPtr turkerId = getDigit(root, 0, center);
	if(turkerId) group->add(turkerId);

	return group;
}

ObjectPtr InstructionFactory::Player(const Json::Value &root, Point p)
{
	// No turkerId needed here
	(void)root;
	ObjectPtr c(new Circle(p, 1000));
	c->setPermanent(true);
	return c;
}

ObjectPtr InstructionFactory::Switch(const Json::Value &root, Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();

	group->add(new Circle(p2, opts::SwitchHandleSize));

	Point stickDirection(p2 - p1);
	Point attachmentPoint = p2 - stickDirection.norm() * opts::SwitchHandleSize;
	group->add(new Line(p1, attachmentPoint));
	ObjectPtr turkerId = getDigit(root, 0, p1 + stickDirection / 2, stickDirection.angle());
	if(turkerId)group->add(turkerId);

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
	Point dir(p2 - p1);

	calculateRectangleCharacteristics(p1, p2, alpha, length, start, midPoint, end);

	Rectangle *bigRect = new Rectangle(midPoint.x() - 1000,
									   midPoint.y() - length/2,
									   2000, // beam is 1000 thick
									   length, // and as long as requested
									   false);
	bigRect->rotate(alpha-M_PI_2, midPoint);
	group->add(bigRect);


	ObjectPtr turkerId = getDigit(root, 0, midPoint + dir.norm().perpendicular() * 1000, dir.angle());
	if(turkerId) group->add(turkerId);

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
	const Point midaxis(p2 - p1);
	const Point direction = midaxis.norm();
	const Point perpendic = direction.perpendicular();

	// Need 5 turkers
	auto addTurkerDigit = [&](const int index, const Point & pos) {
		ObjectPtr turkerId = getDigit(root, index, pos, direction.angle());
		if(turkerId) group->add(turkerId);
	};

	addTurkerDigit(0, p1 - direction.norm() * (opts::ZipLineOuterCircle + opts::Number0Right) - perpendic * opts::Number0Bottom);
	addTurkerDigit(1, p1 - direction.norm() * (opts::ZipLineOuterCircle + opts::Number0Right));

	group->add(new Circle(p1, opts::ZipLineOuterCircle));
	group->add(new Circle(p1, opts::ZipLineInnerCircle));

	group->add(new Line(p1, p2));

	group->add(new Circle(p2, opts::ZipLineInnerCircle));
	group->add(new Circle(p2, opts::ZipLineOuterCircle));

	addTurkerDigit(2, p2 + direction.norm() * opts::ZipLineOuterCircle - perpendic * opts::Number0Bottom);
	addTurkerDigit(3, p2 + direction.norm() * opts::ZipLineOuterCircle);

	addTurkerDigit(4, p1 + midaxis * 0.5);

	return group;
}

ObjectPtr InstructionFactory::ZiplineWithStep(const Json::Value &root, Point p1, Point p2, Point p3, Point p4)
{
	CompositeObjectPtr group = std::dynamic_pointer_cast<CompositeObject>(Zipline(root, p1, p2));

	const Point direction(p2 - p1);

	auto fromMidLineTo = [&](const Point & target){
		// Project p1-target-vector on p1-p2-vector
		Point b_proj = direction * (direction.dot(target - p1) / sqr(direction.abs()));
		// find foot point and measure distance
		return target - (p1 + b_proj);
	};

	const Point toP3 = fromMidLineTo(p3);
	const Point toP4 = fromMidLineTo(p4);
	const Point leftStart = p1 + (direction.norm() * opts::ZipLineOuterCircle);
	const Point rightEnd = p2 - (direction.norm() * opts::ZipLineOuterCircle);
	ObjectPtr step(new Rectangle(leftStart + toP4, rightEnd + toP4, rightEnd + toP3, leftStart + toP3));
	group->add(step);

	const Point indicatorLine = p1 + toP3 + toP3.norm() * opts::IndicatorDistance;
	group->add(MovingIndicator(indicatorLine + direction * 0.25, direction.angle()));
	group->add(MovingIndicator(indicatorLine + direction * 0.75, direction.angle()));

	// No digit here. Let the guy with the horizontal stick think he is in charge

	return group;
}

ObjectPtr InstructionFactory::Stool(const Json::Value &root, Point p1, Point p2, Point p3, Point p4)
{
	//return std::make_shared<Rectangle>(p1, p2, p3, p4, false);

	// They just look the same
	return Table(root, p1, p2, p3, p4);
}

ObjectPtr InstructionFactory::Corpse(const Json::Value &root, Point head, Point chest)
{
	CompositeObjectPtr corpse = CompositeObject::construct();

	const double bodyAspectRatio = 0.702;
	const double spine = (head - chest).abs(); // bodyHeight
	const double bodyWidth = bodyAspectRatio * spine * 2;
	const double armWidth = 0.10 * spine;

	Point armStart(bodyWidth * (3.0 / 8), -spine * 1.0/3);
	Point ellbow(bodyWidth * 0.5, -spine);
	Point upperArm(ellbow - armStart);
	Point armShift(armWidth, 0);


	ObjectPtr turkerId = getDigit(root, 0, Point(0, -spine * 0.3));
	if(turkerId) corpse->add(turkerId);

	ObjectPtr body(new Circle(0, 0, spine, M_PI));
	body->scale(bodyAspectRatio, 1.0);
	corpse->add(body);

	corpse->add(new Circle(Point(0, -spine), 0.4 * spine, radians(158), radians(382)));
	corpse->add(new Line(armStart, ellbow));
	corpse->add(new Line(ellbow, ellbow + upperArm.scaled(-1, 1)));
	corpse->add(new Line(ellbow + upperArm.scaled(-1, 1) + armShift, ellbow + armShift));
	corpse->add(new Line(ellbow + armShift, armStart + armShift));

	corpse->rotate(M_PI_2 + (head - chest).angle());
	corpse->move(chest);

	return CompositeObject::construct(corpse);  // Safely store transform on corpse
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
	Point arrowOneTopTipLeft  = arrowOneTop + s13.scaled(0.1, -0.1);
	Point arrowTwoBottomTipRight = arrowTwoBottom + s13 * 0.1;
	Point arrowTwoBottomTipLeft = arrowTwoBottom + s13.scaled(-0.1, 0.1);

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
	if(turkerId){
		turkerId->rotate(alpha);
		turkerId->move(midPoint - p1p2 / 2);
		turkerId->move(Point(-p1p2.y(), p1p2.x()).norm() * 100);
	}
	group->add(turkerId);
	group->add(new Rectangle(p1, p2, p3, p4, false));
	return group;
}

ObjectPtr InstructionFactory::BlueprintWall(const Json::Value &root, Point p1, Point p2)
{
	// No turker for this one
	(void)root;

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
//	int countdown = root.get("countdown", Json::Value(5000)).asInt();
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

ObjectPtr InstructionFactory::MoveTurker(BlinkFrequency freq, Point p1, Point p2)
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(new /*long*/ Line(p1, p2));
	Point end = p2 + (p1 - p2)/3;
	Line* shortLine = new Line(p2, end);
	shortLine->rotate(30.0f/180.0f*M_PI, p2);
	group->add(shortLine);
	std::chrono::milliseconds blinkfrequency;
	switch (freq) {
	case NO:
		blinkfrequency = std::chrono::hours(24);
		break;
	case LOW:
		blinkfrequency = std::chrono::milliseconds(1000);
		break;
	case MEDIUM:
		blinkfrequency = std::chrono::milliseconds(500);
		break;
	case HIGH:
		blinkfrequency = std::chrono::milliseconds(100);
		break;
	default:
		break;
	}
	group->addAnimation([] (Object* me)
	{
		static bool visible = false;
		visible = !visible;
		me->setVisible(visible);
	}, blinkfrequency);

	return group;
}

ObjectPtr InstructionFactory::MoveTurkerLowFreq(const Json::Value &root, Point p1, Point p2)
{
	return MoveTurker(LOW, p1, p2);
}

ObjectPtr InstructionFactory::MoveTurkerMidFreq(const Json::Value &root, Point p1, Point p2)
{
	return MoveTurker(MEDIUM, p1, p2);
}

ObjectPtr InstructionFactory::MoveTurkerHighFreq(const Json::Value &root, Point p1, Point p2)
{
	return MoveTurker(HIGH, p1, p2);
}

ObjectPtr InstructionFactory::MoveTurkerNoFreq(const Json::Value &root, Point p1, Point p2)
{
	return MoveTurker(NO, p1, p2);
}

}} // namespace laser::holodeck
