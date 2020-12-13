//Author: Quinn Brandt

#include <iostream>
#include<ostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<memory>
#include <cstdlib>
#include"LispTypes.h"
#include"Environment.h"
#include"GlobalLispFunctions.h"

//For full disclosure, I looked at the following lisp interpretter
//to help with some problems I encountered. However I never copy/pasted 
//parts or directly used parts of it. However, some parts of my architecture
//do share some resemblance as a result. Enough of the code that follows
//however, is mine and mine alone, so I feel confident to call it my own.
//The other interpretter: 
//https://gist.github.com/ofan/721464/e1893899739abf7f3e4167bbb722b0420799047f

//Also, I know that there are memory leaks everywhere

auto const promptString = "lisp";

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istream;
using std::ostream;
using std::vector;
using std::istringstream;
//
//using std::shared_ptr;
//using std::dynamic_pointer_cast;
//
//typedef shared_ptr<LispElement> ElemPtr;
//typedef shared_ptr<LispList> LispList*;
//typedef shared_ptr<LispSymbol> LispSymbol*;
//typedef shared_ptr<LispFunc> FuncPtr;
//typedef shared_ptr<LispNumber> NumPtr;

typedef std::pair<LispSymbol, LispElement*> EnvPair;

void printPrompt();
string read();
LispList* parse(string);
LispElement* eval(LispElement*);
void printResult(LispElement*);
void initializeEnvironment(Environment*);

istream& currentInput = cin;
ostream& currentOutput = cout;

Environment* GlobalEnvironment = new Environment();
LispSymbol* nil = new LispSymbol("nil");
LispSymbol* T = new LispSymbol("T");
bool quit = false;

int main()
{
    initializeEnvironment(GlobalEnvironment);
    LispElement* i = GlobalEnvironment->Lookup(*(new LispSymbol("+")));
    while (!quit)
    {
        printPrompt();
        string userInput = read();
        LispList* parsed = parse(userInput);
        parsed = (LispList*)car(parsed);
        LispElement* result = eval(parsed);
        printResult(result);
        cout << endl;
    }
}


string read()
{
    string userInput;
    std::getline(currentInput, userInput);
    return userInput;
}

LispElement* eval(LispElement* elem, Environment* environment)
{
    if (elem->DataType == LispDataType::SYMBOL)
    {
        LispSymbol* sym = (LispSymbol*)(elem);
        LispElement* i = environment->Lookup(*sym);
        return environment->Lookup(*sym);
    }
    if (elem->DataType == LispDataType::NUMBER)
    {
        return elem;

    }
    if (elem->DataType == LispDataType::LIST)
    {
        LispList* list = (LispList*)(elem);
        LispElement* front = car(list);
        auto evalResult = eval(front);
        auto lispFunc = (LispFunc*)(evalResult);
        auto result = lispFunc->func((LispList*)cdr(list));
    }
}

LispElement* eval(LispElement* elem)
{
    return eval(elem, GlobalEnvironment);
}

void printPrompt()
{
    currentOutput << promptString << "> ";
}

//https://stackoverflow.com/questions/29169153/how-do-i-verify-a-string-is-valid-double-even-if-it-has-a-point-in-it/29169235
bool isNumber(const std::string& s)
{
    char* end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
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
        copyList->Value = copy(car(list));
        copyList->NextValue = copy(cdr(list));
        return copyList;
    }
}

void printResult(LispElement* element)
{
    if (element->DataType == LispDataType::SYMBOL)
    {
        LispSymbol* sym = (LispSymbol*)(element);
        cout << sym->Name;
    }
    else if (element->DataType == LispDataType::NUMBER)
    {
        LispNumber* num = (LispNumber*)(element);
        cout << num->Value;
    }
    else if (element->DataType == LispDataType::LIST)
    {
        LispElement* list = copy(element);
        cout << "(";
        bool first = true;
        while (list != nil)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                cout << " ";
            }
            printResult(car((LispList*)list));
            list = ((LispList*)list)->NextValue;
        }
        cout << ")";
    }
    //currentOutput << result << endl;
}



LispList* parse(istream& sin, LispList* list)
{
    if (!sin.eof())
    {
        string token;
        sin >> token;
        if (token.empty())
        {
            return list;
        }

        unsigned int listStart = token.find('(');
        unsigned int listEnd = token.find(')');
        if (listStart != string::npos)
        {
            LispElement* innerList = parse(sin, NULL);
            if (list == NULL)
            {
                list = makeList(innerList);
            }
            else
            {
                append(list, makeList(innerList));
            }
            parse(sin, list);
        }
        else if (listEnd != string::npos)
        {

        }
        else if (isNumber(token))
        {
            LispNumber* num = new LispNumber(std::stod(token));
            if (list == NULL)
            {
                list = makeList(num);
            }
            else
            {
                append(list, makeList(num));
            }
            parse(sin, list);
        }
        else //is Symbol
        { 
            if (token == "'")
            {
                LispList* quoteList = makeList(new LispSymbol("QUOTE"));
                LispList* rest = (LispList*)parse(sin, NULL);
                quoteList = (LispList*)append(quoteList, makeList(car(rest)));
                quoteList = (LispList*)cons(quoteList, cdr(rest));
                
                if (list == NULL)
                {
                    list = quoteList;
                }
                else
                {
                    append(list, quoteList);
                }
                parse(sin, list);
            }
            else
            {
                LispSymbol* sym = new LispSymbol(token);
                if (list == NULL)
                {
                    list = makeList(sym);
                }
                else
                {
                    append(list, makeList(sym));
                }
                parse(sin, list);
            }
        }
    }
    return list;
}

LispList* parse(istream& sin)
{
    LispList* result = parse(sin, NULL);
    return result;
}

LispList* parse(string input)
{
    string specialSpaceChars[] = {"(",")","'"};

    for (string x : specialSpaceChars)
    {
        string y = " " + x + " ";
        https://stackoverflow.com/questions/20406744/how-to-find-and-replace-all-occurrences-of-a-substring-in-a-string
        std::string::size_type n = 0;
        while ((n = input.find(x, n)) != std::string::npos)
        {
            input.replace(n, x.size(), y);
            n += y.size();
        }
    }
    istringstream sin(input);
    return parse(sin);
}

LispElement* quote(const LispList* args)
{
    return first(args);
}

LispElement* car(const LispList* args)
{
    return args->Value;
}

LispElement* cdr(const LispList* args)
{
    return args->NextValue;
}

LispElement* first(const LispList* args)
{
    return car(args);
}

LispElement* second(const LispList* args)
{
    return car((LispList*)cdr(args));
}

LispElement* third(const LispList* args)
{
    return car((LispList*)cdr((LispList*)cdr(args)));
}

LispElement* cons(const LispList* args)
{
    return cons(first(args),second(args));
}

LispElement* append(const LispList* args)
{
    return append(first(args), second(args));
}

LispElement* list(const LispList* args)
{
    return (LispElement*)args;
}

LispElement* add(const LispList* args)
{
    LispElement* second = car(((LispList*)cdr(args)));
    auto value = ((LispNumber*)(eval(car(args))))->Value + ((LispNumber*)(eval(second)))->Value;
    return new LispNumber(value);
}
//
//LispElement* subtract(const LispList* args)
//{
//    auto n = new LispNumber(0);
//    for (auto i = args->Value.begin(); i != args->Value.end(); i++)
//    {
//        if (i == args->Value.begin())
//        {
//            n->Value += ((LispNumber*)(eval(*i)))->Value;
//        }
//        else
//        {
//            n->Value -= ((LispNumber*)(eval(*i)))->Value;
//        }
//    }
//    return n;
//}
//
//LispElement* multiply(const LispList* args)
//{
//    auto n = new LispNumber(1);
//    for (auto i = args->Value.begin(); i != args->Value.end(); i++)
//    {
//        n->Value *= ((LispNumber*)(eval(*i)))->Value;
//    }
//    return n;
//}
//
//LispElement* divide(const LispList* args)
//{
//    auto n = new LispNumber(1);
//    for (auto i = args->Value.begin(); i != args->Value.end(); i++)
//    {
//        if (i == args->Value.begin())
//        {
//            n->Value *= ((LispNumber*)(eval(*i)))->Value;
//        }
//        else
//        {
//            n->Value /= ((LispNumber*)(eval(*i)))->Value;
//        }
//    }
//    return n;
//}

LispElement* ifFunc(const LispList* args)
{
    auto cond = first(args);
    if (cond->DataType == LispDataType::SYMBOL && *(LispSymbol*)cond == *nil)
    {
        return eval(third(args));
    }
    else
    {
        return eval(second(args));
    }
}


void addFunction(Environment* env, string name, Func function)
{
    auto sym = new LispFunc(function);
    auto pair = EnvPair(LispSymbol(name), sym);
    env->Insert(pair);
}

void initializeEnvironment(Environment* env)
{
    //Constant Symbols
    auto nilPair = EnvPair(*nil, nil);
    env->Insert(nilPair);

    auto TPair = EnvPair(*T, T);
    env->Insert(TPair);

    //Math Functions
    addFunction(env, "+", add);/*
    addFunction(env, "-", subtract);
    addFunction(env, "*", multiply);
    addFunction(env, "/", divide);*/

    //Logic Functions
    addFunction(env, "quote", quote);
    addFunction(env, "if", ifFunc);

}

