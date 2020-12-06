#include <iostream>
#include<ostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<memory>
#include"LispTypes.h"

auto const promptString = "lisp";

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istream;
using std::ostream;
using std::vector;
using std::istringstream;

using std::shared_ptr;
using std::dynamic_pointer_cast;

typedef shared_ptr<LispElement> ElemPtr;
typedef shared_ptr<LispList> ListPtr;
#define ListCast dynamic_pointer_cast<LispList> ;
typedef shared_ptr<LispSymbol> SymPtr;
//#define SymCast dynamic_pointer_cast<LispSymbol>;

void printPrompt();
string read();
ListPtr parse(string);
ElemPtr eval(ListPtr);
void printResult(ElemPtr);

istream& currentInput = cin;
ostream& currentOutput = cout;

int main()
{
 
    printPrompt();
    string userInput = read();
    ListPtr parsed = parse(userInput);
    ElemPtr result = eval(parsed);
    //ElemPtr result = ElemPtr(new LispSymbol("a"));
    printResult(result);
}


string read()
{
    string userInput;
    std::getline(currentInput, userInput);
    return userInput;
}

ElemPtr eval(ListPtr list)
{
    return list;
}

void printPrompt()
{
    currentOutput << promptString << "> ";
}

void printResult(ElemPtr element)
{
    if (element->DataType == LispDataType::SYMBOL)
    {
        SymPtr sym = dynamic_pointer_cast<LispSymbol>(element);
        cout << sym->Name;
    }
    else if (element->DataType == LispDataType::LIST)
    {
        ListPtr list = dynamic_pointer_cast<LispList>(element);
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



void parse(istream& sin, ListPtr list)
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
            ListPtr innerList = ListPtr(new LispList());
            parse(sin, innerList);
            list->Value.push_back(innerList);
            parse(sin, list);
            return;
        }
        else if (listEnd != string::npos)
        {
            return;
        }
        else //is Symbol
        { 
            if (token == "'")
            {
                ListPtr quoteList = ListPtr(new LispList());
                quoteList->Value.push_back(SymPtr(new LispSymbol("QUOTE")));
                parse(sin, quoteList);
                list->Value.push_back(quoteList);
            }
            else
            {
                list->Value.push_back(SymPtr(new LispSymbol(token)));
                parse(sin, list);

            }
        }
    }
}

ListPtr parse(istream& sin)
{
    ListPtr result = ListPtr(new LispList());
    parse(sin, result);
    return result;
}

ListPtr parse(string input)
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