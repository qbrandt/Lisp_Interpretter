#include "LispNumber.h"

LispNumber::LispNumber(double number)
{
	Value = number;
	DataType = LispDataType::NUMBER;
}
