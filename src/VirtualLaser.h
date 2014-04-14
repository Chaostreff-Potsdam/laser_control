#pragma once

#include "Canvas.h"
#include <opencv2/core/core.hpp>

namespace laser {

class VirtualLaser: public Canvas
{
public:
	VirtualLaser();
	virtual ~VirtualLaser();

	void writePoints();

private:
	/*!
	 * \brief draws all next points of the same color
	 *
	 * \return the starting index of the next line
	 */
	size_t drawNextPolyline(size_t currentIndex, cv::Mat & canvas);

};

}
