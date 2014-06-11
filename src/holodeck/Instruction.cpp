#include "Instruction.h"

#include <vector>

laser::holodeck::InstructionPtr laser::holodeck::Instruction::construct(const std::vector<laser::ObjectPtr> &objects, int instructionId, const std::vector<int> & turkerIds)
{
	InstructionPtr group(new Instruction(instructionId, turkerIds));

	group->self = group;
	group->add(objects);


    return group;
}

laser::holodeck::InstructionPtr laser::holodeck::Instruction::construct(const ObjectPtr object, int instructionId, const std::vector<int> &turkerIds)
{
    InstructionPtr group(new Instruction(instructionId, turkerIds));

    group->self = group;
    group->add(object);


    return group;
}

laser::holodeck::Instruction::Instruction(int instructionId, const std::vector<int> &turkerIds)
	: laser::CompositeObject(),
	  m_instructionId(instructionId),
	  m_turkerIds(turkerIds)
{
	;;
}
