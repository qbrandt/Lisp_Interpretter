#include "Environment.h"
#include<exception>

using std::exception;


Environment::Environment(Environment* outerEnv)
{
	outer = outerEnv;
}

LispElement* Environment::Lookup(LispSymbol& sym)
{
	if (env.find(sym) != env.end())
	{
		return env[sym];
	}
	else if (outer != nullptr)
	{
		return outer->operator[](sym);
	}
	else
	{
		throw new exception("Symbol Does Not Exist");
	}
}

LispElement* Environment::operator[](LispSymbol& sym)
{
	return env[sym];
}

void Environment::Insert(VarPair pair)
{
	env.insert(pair);
}
