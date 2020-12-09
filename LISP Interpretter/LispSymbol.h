#pragma once
#include <string>
#include"LispElement.h"
#include "LispDataType.h"

using std::string;

class LispSymbol : public LispElement
{
public:
	LispSymbol(string);
	~LispSymbol();
	
	string Name;
};

bool operator==(const LispSymbol& l, const LispSymbol& r);
bool operator<(const LispSymbol& l, const LispSymbol& r);