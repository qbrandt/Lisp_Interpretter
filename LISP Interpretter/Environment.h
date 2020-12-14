#pragma once
#include<map>
#include"LispElement.h"
#include"LispSymbol.h"

using std::map;

class Environment;

typedef map<LispSymbol, LispElement*> VarMap;
typedef std::pair<LispSymbol, LispElement*> VarPair;

class Environment
{
public:
	Environment(Environment* = nullptr);
	LispElement* Lookup(LispSymbol&);
	LispElement* operator[](LispSymbol&);
	void Insert(VarPair);
private:
	VarMap env;
	Environment* outer;
	
};