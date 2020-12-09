#pragma once
#include "LispElement.h"
#include "LispList.h"
#include "Environment.h"
using std::map;
//using std::shared_ptr;

//typedef shared_ptr<Environment> EnvPtr;
//typedef shared_ptr<LispElement> ElemPtr;
//typedef shared_ptr<LispList> ListPtr;

typedef LispElement* (*Func)(const LispList* list);

class LispFunc : public LispElement
{
public:
	LispFunc(Func);
	Func func;
	Environment env;
};

