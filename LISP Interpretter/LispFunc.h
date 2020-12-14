#pragma once
#include "LispElement.h"
#include "LispList.h"
#include "Environment.h"
using std::map;


typedef LispElement* (*Func)(const LispList* list);

class LispFunc : public LispElement
{
public:
	LispFunc(Func);
	Func func;
	Environment* env;
};

