#pragma once
//#include"LispDataType.h"
#include "LispElement.h"
#include "LispList.h"
#include "Environment.h"

class LispLambda : public LispElement
{
public :
	LispLambda();
	LispList* parameters;
	LispList* forms;
	Environment* env;
};

