// main.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <map>  // Include the <map> header for variable storage
#include <queue>
#include "calculation.h"
#include "add.cpp"
#include "subtract.cpp"
#include "multiply.cpp"
#include "divide.cpp"
#include "mod.cpp"
#include "exp.cpp"
#include "tokenize.cpp"
#include "parse.cpp"

using namespace std;

int main() {
    string input;
    double operand1, operand2;
    char operation;

    map<std::string, double> variables;  // Map to store variables

    // intro line. adjust to your liking.
    cout << "-=-=-=-=- Enter expression (e.g., A=2, A + 3), 'help' for menu, or 'exit' to terminate -=-=-=-=-" << endl;
    while (true) {
        cout << "Input: ";
        //cin >> input;
        getline(cin,input);

        // exit code to end program
        if (input == "exit") {
            cout << "Exiting calculator. Goodbye!" << endl;
            break;
        // help code to get this little prompt. adjust to your liking.
        } else if (input == "help") {
            cout << "Supported operations: +, -, *, /, % (modulus), ^ or ** (exponentiation)" << endl;
            cout << "You can also assign variables using character=number. Ex: A=12" << endl;
            cout << endl;
            continue;
        }

        // Check if the input contains '=' for variable assignment
        size_t assignmentPos = input.find('=');
        if (assignmentPos != string::npos) {
            // Variable assignment
            string variableName = input.substr(0, assignmentPos);

            // Check if the variable name contains only alphabetic characters
            if (variableName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {
                cerr << "Invalid variable name: " << variableName << endl;
                continue;
            }

            string valueStr = input.substr(assignmentPos + 1);

            // Convert the value to a double
            double value;
            try {
                value = stod(valueStr);
            } catch (const invalid_argument& e) {
                cerr << "Invalid value for variable assignment: " << valueStr << endl;
                continue;
            }

            // Store the variable in the map
            variables[variableName] = value;
            cout << "Variable " << variableName << " assigned the value " << value << endl;

            continue;  // Skip the rest of the loop and ask for another input
        }


        //tokenize input
        Tokenizer t; //create parser instance
        std::queue<std::string> q = t.tokenize(input,variables); //parse input
        
        //parse input
        Parser parser; //create parser object
        Parser::ASTNode *root = parser.parseExpression(q); //parse the expression to create AST tree

        double result = parser.evaluate(root); //evaluate the expression
        cout<<"Result: "<<result<<std::endl;

        // Print the variable table for debugging. this will be deleted in final product (unless you guys like it)
        cout << "Variable Table:" << endl;
        for (const auto& pair : variables) {
            cout << pair.first << " = " << pair.second << endl;
        }
    }

    return 0;
}
