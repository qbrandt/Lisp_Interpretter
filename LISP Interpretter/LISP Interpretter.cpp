#include <iostream>
#include<ostream>
#include<string>

auto const promptString = "lisp";

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istream;
using std::ostream;

void printPrompt();
string read();
string eval();
void printResult(string);

istream& currentInput = cin;
ostream& currentOutput = cout;

int main()
{
 
    printPrompt();
    string userInput = read();
    string result = eval();
    printResult(result);
}


string read()
{
    string userInput;
    std::getline(currentInput, userInput);
    return userInput;
}

string eval()
{
    return "result";
}

void printPrompt()
{
    currentOutput << promptString << "> ";
}

void printResult(string result)
{
    currentOutput << result << endl;
}