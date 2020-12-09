#pragma once
//#include"LispTypes.h"
//#include<memory>
#include<map>
#include"LispElement.h"
#include"LispSymbol.h"

using std::map;
//using std::shared_ptr;

class Environment;
//
//typedef shared_ptr<Environment> EnvPtr;
//typedef shared_ptr<LispElement> ElemPtr;
//typedef shared_ptr<LispSymbol> SymPtr;

//struct cmpBySym {
//	bool operator()(const SymPtr& l, const SymPtr& r) const
//	{
//		return *l == *r;
//	}
//};

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