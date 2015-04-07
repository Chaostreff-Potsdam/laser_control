#pragma once

#include "objects/Object.h"

namespace laser {

	extern std::vector<ObjectPtr> flattenObject(const ObjectPtr & object);
	extern std::vector<ObjectPtr> flattenObjects(const std::vector<ObjectPtr> & objects);

}
