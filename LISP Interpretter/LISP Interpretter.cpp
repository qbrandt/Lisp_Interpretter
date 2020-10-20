#include <iostream>
#include<ostream>
#include<string>

auto const promptString = "lisp";

using std::string;
using std::istream;
using std::ostream;

string read(ostream&, istream&);
void print(ostream&);

int main()
{
    istream& currentInput = std::cin;
    ostream& currentOutput = std::cout;

    read(currentOutput, currentInput);

    print(currentOutput);
}

string read(ostream& output, istream& input)
{
    string userInput;
    output << promptString << "> ";
    std::getline(input, userInput);
    return userInput;
}

void print(ostream& output)
{
    output << "Result";
}