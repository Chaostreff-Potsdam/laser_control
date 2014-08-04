#include "Instruction.h"

#include "../objects/Polygon.h"

#include <json/value.h>

#include <vector>

laser::holodeck::InstructionPtr laser::holodeck::Instruction::construct(const Json::Value &root)
{
	std::vector<int> turkerIds;
	int id = root.get("instance", Json::Value(0)).asInt();

	Json::Value turkers = root.get("turkers", Json::Value());

	// TODO we should be able to refine this
	for (Json::Value::iterator it = turkers.begin(); it != turkers.end(); ++it)
	{
		turkerIds.push_back((*it).asInt());
	}

	return InstructionPtr(new Instruction(id, turkerIds));
}

laser::holodeck::Instruction::Instruction(int instructionId, const std::vector<int> &turkerIds)
	: laser::CompositeObject(),
	  m_instructionId(instructionId),
	  m_turkerIds(turkerIds)
{
	;;
}
