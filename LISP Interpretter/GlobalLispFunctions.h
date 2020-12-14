#pragma once
#include"LispTypes.h"

LispElement* quote(const LispList* args);

LispElement* frontOfList(const LispList* args);

LispElement* restOfList(const LispList* args);

LispElement* firstOfList(const LispList* args);

LispElement* secondOfList(const LispList* args);

LispElement* thirdOfList(const LispList* args);

LispElement* cons(const LispList* args);

LispElement* append(const LispList* args);

LispElement* list(const LispList* args);

LispElement* add(const LispList* args);