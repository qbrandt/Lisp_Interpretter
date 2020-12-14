#pragma once
#include<list>
//#include<memory>
#include"LispElement.h"

using std::list;


class LispList : public LispElement
{
public:
	LispList();
	~LispList();
	LispElement* Value; 
	LispElement* NextValue;
};

