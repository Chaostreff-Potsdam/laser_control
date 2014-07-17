#pragma once

#include "../objects/Object.h"
#include "Instruction.h"
#include <json/forwards.h>

namespace laser { namespace holodeck {

namespace InstructionFactory {
	ObjectPtr Wall(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr Door(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr Table(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Player(InstructionPtr instruction, const Json::Value &root, Point p);
	ObjectPtr Button(InstructionPtr instruction, const Json::Value &root, Point p);
	ObjectPtr Beam(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr PortalInactive(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr PortalActive(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr Zipline(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr Stool(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Corpse(InstructionPtr instruction, const Json::Value &root, Point head, Point hip, Point leftHand, Point rightHand);
	ObjectPtr Water(InstructionPtr instruction, const Json::Value &root, Point p);
	ObjectPtr Poke(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr Stomper(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr Footwear(InstructionPtr instruction, const Json::Value &root, Point p);
	ObjectPtr Heat(InstructionPtr instruction, const Json::Value &root, Point p);
	ObjectPtr Elevator(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2, Point p3);
	ObjectPtr Guardrail(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr BlueprintWall(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
	ObjectPtr MovingWallWarning(InstructionPtr instruction, const Json::Value &root, Point p1, Point p2);
}

}} // namespace laser::holodeck
