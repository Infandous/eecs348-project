// main.cpp
#include <iostream>
#include "calculation.h"
#include "add.cpp"       // Include all other subclasses as needed
#include "subtract.cpp"
#include "multiply.cpp"
#include "divide.cpp"
#include "mod.cpp"
#include "exp.cpp"

using namespace std;

int main() {
    double operand1, operand2;
    char operation;

    cout << "Enter expression (e.g., 2 + 3): ";
    cin >> operand1 >> operation >> operand2;

    Calculation* calculator = nullptr;

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
            cerr << "Invalid operation!" << endl;
            return 1;
    }

    if (calculator) {
        double result = calculator->calculate(operand1, operand2);
        cout << "Result: " << result << endl;
        delete calculator;
    }

    return 0;
}
