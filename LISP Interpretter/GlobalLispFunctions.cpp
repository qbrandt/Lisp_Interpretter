#include"GlobalLispFunctions.h"



LispElement* quote(const LispList* args)
{
    return firstOfList(args);
}

LispList* makeList(LispElement* element)
{
    LispList* list = new LispList();
    list->Value = element;
    list->NextValue = nil;
    return list;
}

LispElement* cons(LispElement* arg1, LispElement* arg2)
{
    LispList* outerList = makeList(arg1);
    outerList->NextValue = arg2;
    return outerList;
}

LispElement* append(LispElement* arg1, LispElement* arg2)
{
    if (arg1 == nil)
    {
        return arg2;
    }
    LispList* fullList = (LispList*)arg1;
    LispElement* list = fullList;
    LispList* trailer = (LispList*)list;
    while (list != nil)
    {
        trailer = (LispList*)list;
        list = trailer->NextValue;
    }
    trailer->NextValue = arg2;
    return fullList;
}

LispElement* copy(const LispElement* element)
{
    if (element->DataType == LispDataType::SYMBOL)
    {
        LispSymbol* sym = (LispSymbol*)(element);
        return sym;
    }
    else if (element->DataType == LispDataType::NUMBER)
    {
        LispNumber* num = (LispNumber*)(element);
        return num;
    }
    else if (element->DataType == LispDataType::LIST)
    {
        LispList* list = (LispList*)(element);
        LispList* copyList = new LispList();
        copyList->Value = copy(frontOfList(list));
        copyList->NextValue = copy(restOfList(list));
        return copyList;
    }
}

LispElement* frontOfList(const LispList* args)
{
    //printResult((LispElement*)args);

    return args->Value;
}

LispElement* restOfList(const LispList* args)
{
    return args->NextValue;
}

LispElement* firstOfList(const LispList* args)
{
    return frontOfList(args);
}

LispElement* secondOfList(const LispList* args)
{
    return frontOfList((LispList*)restOfList(args));
}

LispElement* thirdOfList(const LispList* args)
{
    return frontOfList((LispList*)restOfList((LispList*)restOfList(args)));
}

LispElement* car(const LispList* args)
{
    //printResult((LispElement*)args);

    return ((LispList*)(args->Value))->Value;
}

LispElement* cdr(const LispList* args)
{
    return ((LispList*)(args->Value))->NextValue;
}

LispElement* first(const LispList* args)
{
    return car(args);
}

LispElement* second(const LispList* args)
{
    return firstOfList((LispList*)cdr(args));
}

LispElement* third(const LispList* args)
{
    return firstOfList((LispList*)restOfList((LispList*)cdr(args)));
}

LispElement* cons(const LispList* args)
{
    return cons((firstOfList(args)), (secondOfList(args)));
}

LispElement* append(const LispList* args)
{
    return append((firstOfList(args)), (secondOfList(args)));
}

LispElement* list(const LispList* args)
{
    return (LispElement*)args;
}

LispElement* add(const LispList* args)
{
    auto firstVal = ((LispNumber*)(firstOfList(args)))->Value;
    auto secondVal = ((LispNumber*)(secondOfList(args)))->Value;
    auto value = firstVal + secondVal;
    return new LispNumber(value);
}

LispElement* subtract(const LispList* args)
{
    auto firstVal = ((LispNumber*)(firstOfList(args)))->Value;
    auto secondVal = ((LispNumber*)(secondOfList(args)))->Value;
    auto value = firstVal - secondVal;
    return new LispNumber(value);
}

LispElement* multiply(const LispList* args)
{
    auto firstVal = ((LispNumber*)(firstOfList(args)))->Value;
    auto secondVal = ((LispNumber*)(secondOfList(args)))->Value;
    auto value = firstVal * secondVal;
    return new LispNumber(value);
}

LispElement* divide(const LispList* args)
{
    auto firstVal = ((LispNumber*)(firstOfList(args)))->Value;
    auto secondVal = ((LispNumber*)(secondOfList(args)))->Value;
    auto value = firstVal / secondVal;
    return new LispNumber(value);
}

LispElement* ifFunc(const LispList* args)
{
    auto cond = firstOfList(args);
    if (cond->DataType == LispDataType::SYMBOL && *(LispSymbol*)cond == *nil)
    {
        return thirdOfList(args);
    }
    else
    {
        return secondOfList(args);
    }
}

LispElement* atom(const LispList* args)
{
    auto atom = firstOfList(args);
    auto rest = restOfList(args);
    if ((atom->DataType == LispDataType::SYMBOL || atom->DataType == LispDataType::NUMBER)
        && *(LispSymbol*)rest == *nil)
    {
        return T;
    }
    else
    {
        return nil;
    }
}

LispElement* listp(const LispList* args)
{
    auto atom = firstOfList(args);
    auto rest = restOfList(args);
    if ((atom->DataType != LispDataType::SYMBOL && atom->DataType != LispDataType::NUMBER)
        || (rest->DataType == LispDataType::SYMBOL && *(LispSymbol*)rest == *nil))
    {
        return T;
    }
    else
    {
        return nil;
    }
}

LispElement* eq(const LispList* args)
{
    auto l = firstOfList(args);
    auto r = secondOfList(args);
    if (l == r)
    {
        return T;
    }
    else
    {
        return nil;
    }
}

LispElement* andFunc(const LispList* args)
{
    auto l = firstOfList(args);
    auto r = secondOfList(args);
    if (l == T && r == T)
    {
        return T;
    }
    else
    {
        return nil;
    }
}

LispElement* orFunc(const LispList* args)
{
    auto l = firstOfList(args);
    auto r = secondOfList(args);
    if (l == T || r == T)
    {
        return T;
    }
    else
    {
        return nil;
    }
}

LispElement* notFunc(const LispList* args)
{
    auto arg = firstOfList(args);
    if (arg == nil)
    {
        return T;
    }
    else
    {
        return nil;
    }
}

LispElement* equal(const LispList* args)
{
    auto l = firstOfList(args);
    auto r = secondOfList(args);
    if (l->DataType == LispDataType::SYMBOL && r->DataType == LispDataType::SYMBOL)
    {
        LispSymbol* lSym = (LispSymbol*)(l);
        LispSymbol* rSym = (LispSymbol*)(r);
        if (*lSym == *rSym)
        {
            return T;
        }
        else
        {
            return nil;
        }
    }
    else if (l->DataType == LispDataType::NUMBER && r->DataType == LispDataType::NUMBER)
    {
        LispNumber* lNum = (LispNumber*)(l);
        LispNumber* rNum = (LispNumber*)(r);
        if (lNum->Value == rNum->Value)
        {
            return T;
        }
        else
        {
            return nil;
        }
    }
    else if (l->DataType == LispDataType::LAMBDA && r->DataType == LispDataType::LAMBDA)
    {
        if (l == r)
        {
            return T;
        }
        else
        {
            return nil;
        }
    }
    else if (l->DataType == LispDataType::LIST && r->DataType == LispDataType::LIST)
    {
        LispList* lList = (LispList*)(l);
        LispList* rList = (LispList*)(r);

        LispElement* lFirst = makeList(firstOfList(lList));
        LispElement* rFirst = makeList(firstOfList(rList));
        LispElement* lRest = makeList(restOfList(lList));
        LispElement* rRest = makeList(restOfList(rList));

        LispElement* firstResult = equal((LispList*)append(lFirst, rFirst));
        LispElement* restResult = equal((LispList*)append(lRest, rRest));

        auto result = andFunc((LispList*)append(makeList(firstResult), makeList(restResult)));

        if (result == T)
        {
            return T;
        }
        else
        {
            return nil;
        }
    }
    else
    {
        return nil;
    }
}


LispElement* isNull(const LispList* args)
{
    auto atom = firstOfList(args);
    auto rest = restOfList(args);
    if (atom->DataType == LispDataType::SYMBOL && rest->DataType == LispDataType::SYMBOL
        && *(LispSymbol*)atom == *nil && *(LispSymbol*)rest == *nil)
    {
        return T;
    }
    else
    {
        return nil;
    }
}



LispElement* exit(const LispList* args)
{
    quit = true;
    return new LispSymbol("EXIT");
}