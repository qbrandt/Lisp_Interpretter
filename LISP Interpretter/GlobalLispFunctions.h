#pragma once
#include"LispTypes.h"

extern LispSymbol* nil;
extern LispSymbol* T;
extern bool quit;



LispElement* quote(const LispList* args);
LispList* makeList(LispElement* element);
LispElement* cons(LispElement* arg1, LispElement* arg2);
LispElement* append(LispElement* arg1, LispElement* arg2);
LispElement* copy(const LispElement* element);
LispElement* frontOfList(const LispList* args);
LispElement* restOfList(const LispList* args);
LispElement* firstOfList(const LispList* args);
LispElement* secondOfList(const LispList* args);
LispElement* thirdOfList(const LispList* args);
LispElement* car(const LispList* args);
LispElement* cdr(const LispList* args);
LispElement* first(const LispList* args);
LispElement* second(const LispList* args);
LispElement* third(const LispList* args);
LispElement* cons(const LispList* args);
LispElement* append(const LispList* args);
LispElement* list(const LispList* args);
LispElement* add(const LispList* args);
LispElement* subtract(const LispList* args);
LispElement* multiply(const LispList* args);
LispElement* divide(const LispList* args);
LispElement* ifFunc(const LispList* args);
LispElement* atom(const LispList* args);
LispElement* listp(const LispList* args);
LispElement* eq(const LispList* args);
LispElement* andFunc(const LispList* args);
LispElement* orFunc(const LispList* args);
LispElement* notFunc(const LispList* args);
LispElement* equal(const LispList* args);
LispElement* isNull(const LispList* args);
LispElement* exit(const LispList* args);