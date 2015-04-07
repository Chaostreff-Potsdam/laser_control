#include "ObjectFlattening.h"
#include "objects/CompositeObject.h"

namespace laser {

std::vector<ObjectPtr> flattenObject(const ObjectPtr & object)
{
	std::vector<ObjectPtr> flat;
	if (CompositeObjectPtr c = std::static_pointer_cast<CompositeObject>(object)) {
		return flattenObjects(c->children());
	} else {
		flat.push_back(object);
		return flat;
	}
}

std::vector<ObjectPtr> flattenObjects(const std::vector<ObjectPtr> & objects)
{
	std::vector<ObjectPtr> flat;
	flat.reserve(2 * objects.size());

	for (const auto & obj: objects)
		appendToVector(flat, flattenObject(obj));

	return flat;
}

}
