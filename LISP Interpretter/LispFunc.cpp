#include "LispFunc.h"

LispFunc::LispFunc(Func function)
{
	func = function;
	DataType = LispDataType::FUNC;
}
