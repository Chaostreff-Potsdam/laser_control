#include "CompositeObject.h"

namespace laser {

	/*! Only works if points provided by childs are in global painter coordinates
	 *
	 * Thus, no transform must by applied to me, and I may never be in a group
	 */
	class EXPORT_LASER_CONTROL RisenGroup : public CompositeObject
	{
		LASER_OBJECT_GROUP(RisenGroup)

	public:
		double z() const
		{ return m_z; }

		void setZ(const double newZ);

	protected:
		RisenGroup();

		EtherdreamPoints points() const;
		EtherdreamPoints startPoints() const;
		EtherdreamPoints endPoints() const;

	private:
		double m_z;

	};

}
