#include "Color.h"

#include <stdint.h>

#include "laser_utilities.h"

laser::Color::Color(laser::Color::ColorName colorName) {
	switch (colorName)
	{
	case laser::Color::GREEN:
		m_red = 0;
		m_green = INT16_MAX;
		m_blue = 0;
		break;
	case laser::Color::DARKGREEN:
		m_red = 0;
		m_green = INT16_MAX/2;
		m_blue = 0;
		break;
	case laser::Color::LIGHTBLUE:
		m_red = 0;
		m_green = INT16_MAX;
		m_blue = INT16_MAX;
		break;
	case laser::Color::BLUE:
		m_red = 0;
		m_green = 0;
		m_blue = INT16_MAX;
		break;
	case laser::Color::RED:
		m_red = INT16_MAX;
		m_green = 0;
		m_blue = 0;
		break;
	case laser::Color::PURPLE:
		m_red = INT16_MAX;
		m_green = 0;
		m_blue = INT16_MAX;
		break;
	default:
		m_red = 0;
		m_green = INT16_MAX;
		m_blue = 0;
		break;
	}
}

laser::Color::Color(int screenRed, int screenGreen, int screenBlue) {
	m_red = clamp(static_cast<int>( (double) screenRed / 255.0 * INT16_MAX ), 0, INT16_MAX);
	m_green = clamp(static_cast<int>( (double) screenGreen / 255.0 * INT16_MAX ), 0, INT16_MAX);
	m_blue = clamp(static_cast<int>( (double) screenBlue / 255.0 * INT16_MAX ), 0, INT16_MAX);
}


int laser::Color::red() const
{
	return m_red;
}

int laser::Color::green() const
{
	return m_green;
}

int laser::Color::blue() const
{
	return m_blue;
}
