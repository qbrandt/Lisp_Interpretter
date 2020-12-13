#pragma once
#include<list>
//#include<memory>
#include"LispElement.h"

using std::list;

//using std::shared_ptr;
//using std::dynamic_pointer_cast;
//
//typedef shared_ptr<LispElement> ElemPtr;
//typedef shared_ptr<LispList> ListPtr;
// ListCast dynamic_pointer_cast<LispList> ;
// shared_ptr<LispSymbol> SymPtr;
//#define SymCast dynamic_pointer_cast<LispSymbol>;

class LispList : public LispElement
{
public:
	LispList();
	~LispList();
	LispElement* Value; 
	LispElement* NextValue;
};

