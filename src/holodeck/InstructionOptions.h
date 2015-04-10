#pragma once

#include "../objects/CompositeObject.h"

namespace laser { namespace holodeck { namespace opts {

class Digit: public CompositeObject
{
public:
	static CompositeObjectPtr get(int digit);

private:
	int m_digit;

	Digit(int digit);
	virtual void tick();
};

static const double assumedNumberWidth = 933.0;
static const double assumedNumberHeight = 1709.0;

static const double Number0Right = 1716;
static const double Number0Bottom = 2103;

static const double IndicatorWidth = 700;
static const double IndicatorHeight = 750;
static const double IndicatorDistance = 850;

static const double SwitchHandleSize = 1000.0;

static const double ZipLineInnerCircle = 500.0;
static const double ZipLineOuterCircle = 1000.0;


}}}
