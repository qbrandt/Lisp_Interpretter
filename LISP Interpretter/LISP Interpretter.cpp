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
//I tried references, but using them with inheritance and 
//the STL don't always mix well. Then I tried smart 
//pointers and all my functions kept geting deallocated, 
//so now I'm using regular pointers and leak memory. But it works.

auto const promptString = "lisp";

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istream;
using std::ostream;
using std::vector;
using std::istringstream;

typedef std::pair<LispSymbol, LispElement*> EnvPair;

void printPrompt();
string read();
LispList* parse(string);
LispElement* eval(LispElement*);
void printResult(LispElement*);
void initializeEnvironment(Environment*);
LispElement* append(LispElement* arg1, LispElement* arg2);
LispElement* copy(const LispElement* element);

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
        try
        {
            LispList* parsed = parse(userInput);
            /*printResult(parsed);
            cout << endl;*/
            parsed = (LispList*)frontOfList(parsed);
            LispElement* result = eval(parsed);
            printResult(result);
        }
        catch (...)
        {
            cout << endl << "Break";
        }
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
        //LispElement* i = environment->Lookup(*sym);
        return environment->Lookup(*sym);
    }
    if (elem->DataType == LispDataType::NUMBER)
    {
        return elem;

    }
    if (elem->DataType == LispDataType::LIST)
    {
        LispList* list = (LispList*)(elem);
        LispElement* front = frontOfList(list);
        //SetQ and LAMBDA are not seperate functions because they require access to the environment
        if (front->DataType == LispDataType::SYMBOL && ((LispSymbol*)front)->Name == "SETQ")
        {
            LispSymbol* name = (LispSymbol*)secondOfList(list);
            LispElement* val = eval(thirdOfList(list),environment);
            auto newValue = VarPair(*name, val);
            environment->Insert(newValue);
            return val;

        }
        if (front->DataType == LispDataType::SYMBOL && ((LispSymbol*)front)->Name == "LAMBDA")
        {
            LispLambda* lambda = new LispLambda();
            lambda->env = new Environment(environment);
            lambda->parameters = (LispList*)secondOfList(list);
            lambda->forms = (LispList*)restOfList((LispList*)restOfList(list));
            return lambda;
        }
        if (front->DataType == LispDataType::SYMBOL && ((LispSymbol*)front)->Name == "DEFUN")
        {
            LispList* setq = makeList(new LispSymbol("SETQ"));
            LispList* name = makeList(secondOfList(list));
            LispList* lambda = makeList(new LispSymbol("LAMBDA"));
            append(setq, name);
            append(lambda, restOfList((LispList*)restOfList(list)));
            append(setq, makeList(lambda));
            return eval(setq);
        }
        else
        {
            auto evalResult = eval(front, environment);
            LispList* arguments = (LispList*)copy(restOfList(list));

            if (front->DataType != LispDataType::SYMBOL || ((LispSymbol*)front)->Name != "QUOTE")
            {
                LispElement* lispArgs = arguments;
                while ((lispArgs) != nil)
                {                
                    auto arg = ((LispList*)lispArgs);
                    arg->Value = eval(arg->Value,environment);
                    lispArgs = restOfList((LispList*)lispArgs);
                }
            }
            if (evalResult->DataType == LispDataType::FUNC)
            {
                auto lispFunc = (LispFunc*)(evalResult);
                auto result = lispFunc->func(arguments);
                return result;
            }
            else //Lambdas
            {
                auto lispLambda = (LispLambda*)(evalResult);
                LispElement* args = arguments;
                LispElement* params = lispLambda->parameters;
                while ((params) != nil)
                {
                    auto param = (LispList*)params;
                    LispSymbol* sym = (LispSymbol*)(param->Value);
                    auto arg = ((LispList*)args)->Value;
                    auto pair = VarPair(*sym, arg);
                    lispLambda->env->Insert(pair);
                    params = restOfList((LispList*)params);
                    args = restOfList((LispList*)args);
                }
                LispElement* result = nil;
                LispElement* forms = lispLambda->forms;
                while ((forms) != nil)
                {
                    auto form = ((LispList*)forms)->Value;
                    result = eval(form,lispLambda->env);
                    forms = restOfList((LispList*)forms);
                }
                return result;
            }
        }

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
    else if (element->DataType == LispDataType::LAMBDA)
    {
        cout << "<LAMBDA>";
    }
    else if (element->DataType == LispDataType::LIST)
    {
        LispElement* list = copy(element);
        cout << "(";
        bool first = true;
        while (list != nil && list->DataType == LispDataType::LIST)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                cout << " ";
            }
            printResult(frontOfList((LispList*)list));
            list = ((LispList*)list)->NextValue;
        }
        if (list != nil)
        {
            cout << " . ";
            printResult(list);
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
                quoteList = (LispList*)append(quoteList, makeList(frontOfList(rest)));
                quoteList = (LispList*)cons(quoteList, restOfList(rest));
                
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

    addFunction(env, "exit", exit);

    //Math Functions
    addFunction(env, "+", add);
    addFunction(env, "-", subtract);
    addFunction(env, "*", multiply);
    addFunction(env, "/", divide);

    //Logic Functions
    addFunction(env, "quote", quote);
    addFunction(env, "if", ifFunc);

    //List Functions
    addFunction(env, "car", car);
    addFunction(env, "cdr", cdr);
    addFunction(env, "cons", cons);
    addFunction(env, "append", append);
    addFunction(env, "list", list);
    addFunction(env, "first", first);
    addFunction(env, "second", second);
    addFunction(env, "third", third);

    //Predicate Functions
    addFunction(env, "atom", atom);
    addFunction(env, "listp", listp);
    addFunction(env, "null", isNull);
    addFunction(env, "equal", equal);
    addFunction(env, "eq", eq);
    addFunction(env, "and", andFunc);
    addFunction(env, "or", orFunc);
    addFunction(env, "not", notFunc);
}

