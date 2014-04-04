#pragma once

#include "../Object.h"

namespace laser { namespace holodeck {

namespace InstructionFactory {
	ObjectPtr Wall(Point p1, Point p2);
	ObjectPtr Door(Point p1, Point p2);
	ObjectPtr Table(Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Player(Point p);
	ObjectPtr Button(Point p);
	ObjectPtr Beam(Point p1, Point p2);
	ObjectPtr InActivePortal(Point p1, Point p2);
	ObjectPtr ActivePortal(Point p1, Point p2);
	ObjectPtr Zipline(Point p1, Point p2);
	ObjectPtr Stool(Point p1, Point p2);
	ObjectPtr Corpse(Point head, Point hip, Point leftHand, Point rightHand);
	ObjectPtr Water(Point p);
	ObjectPtr Poke(Point p1, Point p2);
	ObjectPtr Stomper(Point p1, Point p2);
	ObjectPtr Footwear(Point p);
	ObjectPtr Heat(Point p);
	ObjectPtr Elevator(Point p1, Point p2, Point p3);

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
