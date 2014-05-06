#pragma once

#include "../objects/CompositeObject.h"

#include <vector>
#include <memory>

namespace laser {
	namespace holodeck {

		class Instruction;
		typedef std::shared_ptr<Instruction> InstructionPtr;

		class Instruction : public laser::CompositeObject
		{
		public:
			static InstructionPtr construct(const std::vector<ObjectPtr> & objects, int instructionId, const std::vector<int> & turkerIds);

		protected:
			Instruction(int instructionId, const std::vector<int> & turkerIds);

            //CompositeObjectPtr

			int m_instructionId;
			std::vector<int> m_turkerIds;
		};



	}
}
