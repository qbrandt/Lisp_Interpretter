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

//For full disclosure, I looked at the following lisp interpretter
//to help with some problems I encountered. However I never copy/pasted 
//parts or directly used parts of it. However, some parts of my architecture
//do share some resemblance as a result. Enough of the code that follows
//however, is mine and mine alone, so I feel confident to call it my own.
//The other interpretter: 
//https://gist.github.com/ofan/721464/e1893899739abf7f3e4167bbb722b0420799047f

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
        parsed = (LispList*)(parsed->Value.front());
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
    if (elem->DataType == LispDataType::LIST)
    {
        LispList* list = (LispList*)(elem);
        LispElement* front = list->Value.front();
        list->Value.pop_front();
        auto evalResult = eval(front);
        auto lispFunc = (LispFunc*)(evalResult);
        auto result = lispFunc->func(list);

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
        LispList* list = (LispList*)(element);
        cout << "(";
        for (auto i = list->Value.begin(); i != list->Value.end();i++)
        {
            if (i != list->Value.begin())
            {
                cout << " ";
            }
            printResult(*i);
        }
        cout << ")";
    }
    //currentOutput << result << endl;
}

//https://stackoverflow.com/questions/29169153/how-do-i-verify-a-string-is-valid-double-even-if-it-has-a-point-in-it/29169235
bool isNumber(const std::string& s)
{
    char* end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}


void parse(istream& sin, LispList* list)
{
    if (!sin.eof())
    {
        string token;
        sin >> token;
        if (token.empty())
        {
            return;
        }

        unsigned int listStart = token.find('(');
        unsigned int listEnd = token.find(')');
        if (listStart != string::npos)
        {
            LispList* innerList = new LispList();
            parse(sin, innerList);
            list->Value.push_back(innerList);
            parse(sin, list);
            return;
        }
        else if (listEnd != string::npos)
        {
            return;
        }
        else if (isNumber(token))
        {
            list->Value.push_back(new LispNumber(std::stod(token)));
            parse(sin, list);
        }
        else //is Symbol
        { 
            if (token == "'")
            {
                LispList* quoteList = new LispList();
                quoteList->Value.push_back(new LispSymbol("QUOTE"));
                parse(sin, quoteList);
                list->Value.push_back(quoteList);
            }
            else
            {
                list->Value.push_back(new LispSymbol(token));
                parse(sin, list);
            }
        }
    }
}

LispList* parse(istream& sin)
{
    LispList* result = new LispList();
    parse(sin, result);
    return result;
}

LispList* parse(string input)
{
    string specialSpaceChars[] = {"(",")"};

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

LispElement* add(const LispList* args)
{
    auto n = new LispNumber(0);
    for (auto i = args->Value.begin(); i != args->Value.end(); i++)
    {
        n->Value += ((LispNumber*)(eval(*i)))->Value;
    }
    return n;
}

LispElement* subtract(const LispList* args)
{
    auto n = new LispNumber(0);
    for (auto i = args->Value.begin(); i != args->Value.end(); i++)
    {
        if (i == args->Value.begin())
        {
            n->Value += ((LispNumber*)(eval(*i)))->Value;
        }
        else
        {
            n->Value -= ((LispNumber*)(eval(*i)))->Value;
        }
    }
    return n;
}

void initializeEnvironment(Environment* env)
{
    auto nilPair = EnvPair(*nil, nil);
    env->Insert(nilPair);

    auto TPair = EnvPair(*T, T);
    env->Insert(TPair);

    auto addSym = new LispFunc(add);
    auto addPair = EnvPair(LispSymbol("+"), addSym);
    env->Insert(addPair);

    auto subSym = new LispFunc(subtract);
    auto subPair = EnvPair(LispSymbol("-"), subSym);
    env->Insert(subPair);
}