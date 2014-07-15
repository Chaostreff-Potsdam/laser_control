#pragma once

#include "../objects/Object.h"
#include "Instruction.h"

namespace laser { namespace holodeck {

namespace InstructionFactory {
	ObjectPtr Wall(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr Door(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr Table(InstructionPtr instruction, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Player(InstructionPtr instruction, Point p);
	ObjectPtr Button(InstructionPtr instruction, Point p);
	ObjectPtr Beam(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr PortalInactive(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr PortalActive(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr Zipline(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr Stool(InstructionPtr instruction, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Corpse(InstructionPtr instruction, Point head, Point hip, Point leftHand, Point rightHand);
	ObjectPtr Water(InstructionPtr instruction, Point p);
	ObjectPtr Poke(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr Stomper(InstructionPtr instruction, Point p1, Point p2);
	ObjectPtr Footwear(InstructionPtr instruction, Point p);
	ObjectPtr Heat(InstructionPtr instruction, Point p);
	ObjectPtr Elevator(InstructionPtr instruction, Point p1, Point p2, Point p3);
	ObjectPtr Guardrail(InstructionPtr instruction, Point p1, Point p2);
}

}} // namespace laser::holodeck
