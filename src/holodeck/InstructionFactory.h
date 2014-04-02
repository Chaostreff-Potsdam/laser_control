#pragma once

#include "../Object.h"

namespace laser { namespace holodeck {

namespace InstructionFactory {
	ObjectPtr wall(Point p1, Point p2);
	ObjectPtr door(Point p1, Point p2);
	ObjectPtr table(Point p1, Point p2, Point p3, Point p4);
	ObjectPtr player(Point p);
	ObjectPtr button(Point p);
	ObjectPtr beam(Point p1, Point p2);
	ObjectPtr portal(Point p1, Point p2, bool active);
	ObjectPtr zipline(Point p1, Point p2);
	ObjectPtr stool(Point p1, Point p2);
	ObjectPtr corpse(Point head, Point hip, Point leftHand, Point rightHand);
	ObjectPtr water(Point p);
	ObjectPtr poke(Point p1, Point p2);
	ObjectPtr stomper(Point p1, Point p2);

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
	void calculateRectangleCharacteristics(Point p1, Point p2, float &angle, float &length, Point &start, Point &mid, Point &end);
}

}} // namespace laser::holodeck
