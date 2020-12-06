#pragma once
#include"LispDataType.h"
class LispElement
{
public:
	virtual ~LispElement();
	LispDataType DataType;
};

