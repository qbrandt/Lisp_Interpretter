#include "LispSymbol.h"
#include <algorithm>

LispSymbol::LispSymbol(string name)
{
	DataType = LispDataType::SYMBOL;
	//https://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	Name = name;
}

LispSymbol::~LispSymbol()
{
}

bool operator==(const LispSymbol& l, const LispSymbol& r)
{
	return l.Name == r.Name;
}

bool operator<(const LispSymbol& l, const LispSymbol& r)
{
	return l.Name < r.Name;
}
