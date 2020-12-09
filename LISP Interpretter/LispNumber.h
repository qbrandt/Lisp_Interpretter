#pragma once
#include "LispTypes.h"
class LispNumber : public LispElement
{
public:
	LispNumber(double);
	double Value;
};

