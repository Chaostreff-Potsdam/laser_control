#pragma once

#include "DllExport.h"

namespace laser {
	class EXPORT_LASER_CONTROL Color
	{
	public:
		enum ColorName
		{
			GREEN,
			DARKGREEN,
			LIGHTBLUE,
			BLUE,
			RED,
			PURPLE,
			WHITE,
			BLACK
		};

		Color(ColorName colorName = ColorName::GREEN);
		Color(int screenRed, int screenGreen, int screenBlue);
	
		int red() const;
		int green() const;
		int blue() const;
	private:
		int m_red;
		int m_green;
		int m_blue;
	};
}

