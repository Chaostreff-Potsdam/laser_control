#pragma once

#include "../objects/Object.h"
#include "Instruction.h"
#include <json/forwards.h>

namespace laser { namespace holodeck {

extern const Color MetaDataColor;

namespace InstructionFactory {
	enum BlinkFrequency
	{
		NO,
		LOW,
		MEDIUM,
		HIGH
	};

	ObjectPtr Wall(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Door(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Table(const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Player(const Json::Value &root, Point p);
	ObjectPtr Switch(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Beam(const Json::Value &root, Point p1, Point p2);
	ObjectPtr PortalInactive(const Json::Value &root, Point p1, Point p2);
	ObjectPtr PortalActive(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Zipline(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Stool(const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr Corpse(const Json::Value &root, Point head, Point chest);
	ObjectPtr Water(const Json::Value &root, Point p);
	ObjectPtr Poke(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MovingWall(const Json::Value &root, Point p1, Point p2);
	ObjectPtr Footwear(const Json::Value &root, Point p);
	ObjectPtr Heat(const Json::Value &root, Point p);
	ObjectPtr Elevator(const Json::Value &root, Point p1, Point p2, Point p3);
	ObjectPtr Guardrail(const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr BlueprintWall(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MovingWallWarning(const Json::Value &root, Point p1, Point p2);
	ObjectPtr ZiplineWithStep(const Json::Value &root, Point p1, Point p2, Point p3, Point p4);
	ObjectPtr MoveTurker(BlinkFrequency freq, Point p1, Point p2);
	ObjectPtr MoveTurkerNoFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveTurkerLowFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveTurkerMidFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveTurkerHighFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorClockwiseNoFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorClockwiseLowFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorClockwiseMidFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorClockwiseHighFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorCounterClockwiseNoFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorCounterClockwiseLowFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorCounterClockwiseMidFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorCounterClockwiseHighFreq(const Json::Value &root, Point p1, Point p2);
	ObjectPtr MoveDoorClockwise(BlinkFrequency freq, Point p1, Point p2);
	ObjectPtr MoveDoorCounterClockwise(BlinkFrequency freq, Point p1, Point p2);
	ObjectPtr TurkerLabel(const Json::Value &root, Point p1, Point p2);
}

}} // namespace laser::holodeck
