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
	ObjectPtr PortalInactive(Point p1, Point p2);
	ObjectPtr PortalActive(Point p1, Point p2);
	ObjectPtr Zipline(Point p1, Point p2);
	ObjectPtr Stool(Point p1, Point p2);
	ObjectPtr Corpse(Point head, Point hip, Point leftHand, Point rightHand);
	ObjectPtr Water(Point p);
	ObjectPtr Poke(Point p1, Point p2);
	ObjectPtr Stomper(Point p1, Point p2);
	ObjectPtr Footwear(Point p);
	ObjectPtr Heat(Point p);
	ObjectPtr Elevator(Point p1, Point p2, Point p3);
}

}} // namespace laser::holodeck
