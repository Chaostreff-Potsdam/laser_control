#pragma once

#include "../objects/Object.h"
#include "Instruction.h"
#include <json/forwards.h>

namespace laser { namespace holodeck {

namespace InstructionFactory {
	ObjectPtr Wall(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Door(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Table(const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Player(const Json::Value &root, Point p);
	ObjectPtr Button(const Json::Value &root, Point p);
	ObjectPtr Beam(const Json::Value &root, Point p1, Point p2);
	ObjectPtr PortalInactive(const Json::Value &root, Point p1, Point p2);
	ObjectPtr PortalActive(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Zipline(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Stool(const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Corpse(const Json::Value &root, Point head, Point hip, Point leftHand, Point rightHand);
	ObjectPtr Water(const Json::Value &root, Point p);
	ObjectPtr Poke(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Stomper(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Footwear(const Json::Value &root, Point p);
	ObjectPtr Heat(const Json::Value &root, Point p);
	ObjectPtr Elevator(const Json::Value &root, Point p1, Point p2, Point p3);
	ObjectPtr Guardrail(const Json::Value &root, Point p1, Point p2);
	ObjectPtr BlueprintWall(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MovingWallWarning(const Json::Value &root, Point p1, Point p2);
}

}} // namespace laser::holodeck
