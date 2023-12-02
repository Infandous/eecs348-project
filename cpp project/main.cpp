// main.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <map>  // Include the <map> header for variable storage
#include "calculation.h"
#include "add.cpp"
#include "subtract.cpp"
#include "multiply.cpp"
#include "divide.cpp"
#include "mod.cpp"
#include "exp.cpp"

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
        cin >> input;

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

        // Parse the input for calculation
        char operation;
        string operand1Str, operand2Str;

        // Find the position of the operator using simple check. This will need to be updated for both PEMDAS and multiple operands
        size_t pos = input.find_first_of("+-*/%^");
        if (pos != string::npos) {
            operation = input[pos];
            
            // Separate the operands
            operand1Str = input.substr(0, pos);
            operand2Str = input.substr(pos + 1);

            try {
                // Check if the operands are variables and substitute their values
                if (variables.find(operand1Str) != variables.end()) {
                    operand1 = variables[operand1Str];
                } else {
                    operand1 = stod(operand1Str);
                }

                if (variables.find(operand2Str) != variables.end()) {
                    operand2 = variables[operand2Str];
                } else {
                    operand2 = stod(operand2Str);
                }
            
        // } else {
            } catch (const exception& e) {
            cerr << "Invalid input format: " << input << endl;
            continue;
            }
        }


        // create calculator instance to overload and process function of found operand

        Calculation* calculator = nullptr;

        if (operation == '^' || (input.find("**") != string::npos)) {
            calculator = new Exp();
        } else {
            switch (operation) {
                case '+':
                    calculator = new Addition();
                    break;
                case '-':
                    calculator = new Subtraction();
                    break;
                case '*':
                    calculator = new Multiplication();
                    break;
                case '/':
                    calculator = new Division();
                    break;
                case '%':
                    calculator = new Modulus();
                    break;
                case '^':
                    calculator = new Exp();
                    break;
                default:
                    cerr << "Invalid operation! Enter 'help' for supported operations." << endl;
                    continue;
            }
        }

        // parse expression here with each operand. This will need to be updated with tokens or a more robust parser to allow more than one result.
        if (calculator) {
            double result = calculator->calculate(operand1, operand2);
            cout << "Result: " << result << endl;
            delete calculator;
        }

        // Print the variable table for debugging. this will be deleted in final product (unless you guys like it)
        cout << "Variable Table:" << endl;
        for (const auto& pair : variables) {
            cout << pair.first << " = " << pair.second << endl;
        }
    }

    return 0;
}