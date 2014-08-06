#pragma once

#include "../EtherdreamWrapper.h"
#include "../laser_utilities.h"
#include "../DllExport.h"
#include "../Color.h"
#include "../animations//Animation.h"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <memory>

namespace laser {

	class Object;
	typedef std::shared_ptr<Object> ObjectPtr;

	class CompositeObject;
	typedef std::shared_ptr<CompositeObject> CompositeObjectPtr;

	/*!
	 * \brief an abstract object to be painted
	 */
	class EXPORT_LASER_CONTROL Object
	{
	public:
		typedef CompositeObjectPtr parent_t;

		Object();

		parent_t parent() const;
		void setParent(const parent_t & newParent);

		EtherdreamPoints pointsToPaint();

		/*** Transforms ***/

		cv::Mat transform() const
		{ return m_transform; }

		/*!
		 * \brief rotate this object by \f$\frac{180\cdot rad}{\pi}\f$ degrees
		 */
		void rotate(double rad);
		void rotate(double rad, int centerX, int centerY, double scale = 1);
		void rotate(double rad, const Point & center, double scale = 1)
		{ rotate(rad, center.x(), center.y(), scale); }

		/*!
		 * \brief move this object by x and y
		 */
		void move(int x, int y);
		void move(const Point & p)
		{ move(p.x(), p.y()); }

		void scale(double factorX, double factorY);
		void scale(double factor);

		void resetTransform();

		//! Add an animation, where AnimationFuncT is a class with a operator(Object *)()
		template<typename AnimationFuncT>
		void addAnimation(const std::chrono::milliseconds &period = Animation::defaultPeriod)
		{
			addAnimation(AnimationFuncT(), period);
		}

		void addAnimation(const Animation::Func & func, const std::chrono::milliseconds &period = Animation::defaultPeriod);
		void removeAllAnimations();

		void setVisible(bool visible);
		bool visible();

		void setPixelsPerPoint(int pixelsPerPoint = -1);
		void setMarginPointFraction(int marginPointFraction = -1);

		void setColor(Color color);
		Color color();

		/*** Timeout ***/

		boost::posix_time::ptime started() const;

		void setPermanent(bool permanent);
		bool permanent() const;

		/*!
		 * \brief calculate the points to be drawn with the laser projector
		 */
		virtual EtherdreamPoints points() const = 0;
		/*!
		 * \brief calculate the margin before the actual points to be drawn
		 */
		virtual EtherdreamPoints startPoints() const = 0;
		/*!
		 * \brief just like startPoints() for the end
		 */
		virtual EtherdreamPoints endPoints() const = 0;

		static int s_pixelsPerPointDefault;
		static int s_marginPointFractionDefault;

	protected:
		boost::posix_time::ptime m_started;
		bool m_permanent;
		Color m_color;

		/*!
		 * \brief call this to trigger a repaint
		 */
		void nowDirty()
		{ m_dirty = true; }

		int m_pixelsPerPoint;
		int pixelsPerPoint() const;
		int m_marginPointFraction;
		int marginPointFraction() const;

		const etherdream_point etherdreamPoint(int x, int y, bool visible = true) const;
		const etherdream_point etherdreamPoint(Point p, bool visible = true) const
		{ return etherdreamPoint(p.x(), p.y(), visible); }

	private:
		bool m_dirty, m_isUpdating;
		EtherdreamPoints m_cache;

		cv::Mat m_transform;
		std::weak_ptr<CompositeObject> m_parent;

		std::vector<AnimationPtr> m_animations;
		bool m_isVisible;

		/*!
		 * \brief update the point cache, so you'll not need to acces points() etc.
		 */
		void rebuildCache();
	};

}
