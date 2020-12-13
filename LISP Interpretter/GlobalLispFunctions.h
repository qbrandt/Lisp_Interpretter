#pragma once
#include"LispTypes.h"

LispElement* quote(const LispList* args);

LispElement* car(const LispList* args);

LispElement* cdr(const LispList* args);

LispElement* first(const LispList* args);

LispElement* second(const LispList* args);

LispElement* cons(const LispList* args);

LispElement* append(const LispList* args);

LispElement* list(const LispList* args);

LispElement* add(const LispList* args);