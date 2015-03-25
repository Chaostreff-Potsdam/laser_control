#include "InstructionOptions.h"
#include "../objects/CompositeObject.h"
#include "../objects/Line.h"
#include "../objects/GenObject.h"
#include "../objects/Circle.h"

namespace laser { namespace holodeck { namespace opts {

#define LASER_HOLODECK_OPTS_FULL (3000.0)
#define LASER_HOLODECK_OPTS_HALF (LASER_HOLODECK_OPTS_FULL / 2)
#define LASER_HOLODECK_OPTS_QURT (LASER_HOLODECK_OPTS_FULL / 4)

#define HANDCRAFTED 1

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
	CompositeObjectPtr group = CompositeObject::construct();
#if HANDCRAFTED
	ObjectPtr circ(new Circle(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF * 0.5));
	circ->scale(1, 2);

	group->add(circ);
#else
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(0, 0)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(0, LASER_HOLODECK_OPTS_FULL)));
#endif
	return group;
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
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	return group;
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
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(0, 0)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	return group;
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
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	return group;
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
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	return group;
}

static CompositeObjectPtr getFive()
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(0, 0)));
	return group;
}

static CompositeObjectPtr getSix()
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, 0)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF), Point(0, LASER_HOLODECK_OPTS_HALF)));
	return group;
}

static CompositeObjectPtr getSeven()
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	return group;
}

static CompositeObjectPtr getEight()
{
	CompositeObjectPtr group = CompositeObject::construct();
#if HANDCRAFTED
	ObjectPtr top(new Circle(0, 0, LASER_HOLODECK_OPTS_QURT * 0.9, -M_PI / 2, M_PI * 1.5));
	ObjectPtr bottom(new Circle(0, 0, LASER_HOLODECK_OPTS_QURT, -M_PI / 2, M_PI * 1.5));

	top->move(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_QURT);
	bottom->flipVertically();
	bottom->move(LASER_HOLODECK_OPTS_HALF, -LASER_HOLODECK_OPTS_QURT);

	group->add(top);
	group->add(bottom);
#else
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(0, 0)));
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
#endif
	return group;
}

static CompositeObjectPtr getNine()
{
	CompositeObjectPtr group = CompositeObject::construct();
	group->add(std::make_shared<Line>(Point(0, 0), Point(LASER_HOLODECK_OPTS_HALF, 0)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, 0), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_FULL)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_FULL), Point(0, LASER_HOLODECK_OPTS_HALF)));
	group->add(std::make_shared<Line>(Point(0, LASER_HOLODECK_OPTS_HALF), Point(LASER_HOLODECK_OPTS_HALF, LASER_HOLODECK_OPTS_HALF)));
	return group;
}

CompositeObjectPtr getDigit(int digit)
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
		return CompositeObject::construct();
		break;
	}
}

}}}
