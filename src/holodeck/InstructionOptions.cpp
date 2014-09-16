#include "InstructionOptions.h"
#include "../objects/CompositeObject.h"
#include "../objects/Line.h"
#include "../objects/Polygon.h"

namespace laser { namespace holodeck { namespace opts {

#define LASER_HOLODECK_OPTS_FULL (3000)
#define LASER_HOLODECK_OPTS_HALF (1500)

static CompositeObjectPtr getZero()
{
	/*
	 * r-----q
	 * |=====|
	 * |I   I|
	 * |I   I|
	 * |I   I|
	 * |=====|
	 * L-----J
	 */
	std::vector<Point> points
	{
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, 0),
		Point(0, 0)
	};
	return CompositeObject::construct(std::make_shared<Polygon>(points, false));
}

static CompositeObjectPtr getOne()
{
	/*
	 * r-----q
	 * |    I|
	 * |    I|
	 * |    I|
	 * |    I|
	 * |    I|
	 * L-----J
	 */
	std::vector<Point> points
	{
		Point(LASER_HOLODECK_OPTS_HALF, 0),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
	};
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getTwo()
{
	/*
	 * r-----q
	 * |=====|
	 * |    I|
	 * |=====|
	 * |I    |
	 * |=====|
	 * L-----J
	 */
	std::vector<Point> points
	{
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF),
		Point(0, LASER_HOLODECK_OPTS_HALF),
		Point(0, 0),
		Point(LASER_HOLODECK_OPTS_HALF, 0)
	};
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getThree()
{
	/*
	 * r-----q
	 * |=====|
	 * |    I|
	 * |=====|
	 * |    I|
	 * |=====|
	 * L-----J
	 */
	std::vector<Point> points
	{
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF),
		Point(0, LASER_HOLODECK_OPTS_HALF),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF),
		Point(LASER_HOLODECK_OPTS_HALF, 0),
		Point(0, 0)
	};
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getFour()
{
	/*
	 * r-----q
	 * |I   I|
	 * |I   I|
	 * |=====|
	 * |    I|
	 * |    I|
	 * L-----J
	 */
	std::vector<Point> points
	{
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(0, LASER_HOLODECK_OPTS_HALF),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, 0)
	};
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getFive()
{
	std::vector<Point> points
	{
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(0, LASER_HOLODECK_OPTS_HALF),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF),
		Point(LASER_HOLODECK_OPTS_HALF, 0),
		Point(0, 0)
	};
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(0, 0)));
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getSix()
{
	std::vector<Point> points
	{
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(0, 0),
		Point(LASER_HOLODECK_OPTS_HALF, 0),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF),
		Point(0, LASER_HOLODECK_OPTS_HALF)
	};
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, 0)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF), Point(0, LASER_HOLODECK_OPTS_HALF)));
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getSeven()
{
	std::vector<Point> points
	{
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, 0)
	};
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getEight()
{
	std::vector<Point> points
	{
		Point(0, LASER_HOLODECK_OPTS_HALF),
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF),
		Point(0, LASER_HOLODECK_OPTS_HALF),
		Point(0, 0),
		Point(LASER_HOLODECK_OPTS_HALF, 0),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)
	};
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(0, 0)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

static CompositeObjectPtr getNine()
{
	std::vector<Point> points
	{
		Point(0, 0),
		Point(LASER_HOLODECK_OPTS_HALF, 0),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL),
		Point(0, LASER_HOLODECK_OPTS_FULL),
		Point(0, LASER_HOLODECK_OPTS_HALF),
		Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)
	};
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	return CompositeObject::construct(std::make_shared<Polygon>(points, false, false, false));
}

CompositeObjectPtr getDigit(unsigned int digit)
{
	switch (digit) {
	case 0:
		return getZero();
		break;
	case 1:
		return getOne();
		break;
	case 2:
		return getTwo();
		break;
	case 3:
		return getThree();
		break;
	case 4:
		return getFour();
		break;
	case 5:
		return getFive();
		break;
	case 6:
		return getSix();
		break;
	case 7:
		return getSeven();
		break;
	case 8:
		return getEight();
		break;
	case 9:
		return getNine();
		break;
	default:
		return 0;
		break;
	}
}

}}}
