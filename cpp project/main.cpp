#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <cctype>
#include <stack>
#include <vector>
#include <memory>  // Add this line for std::unique_ptr
#include "calculation.h"
#include "add.cpp"
#include "subtract.cpp"
#include "multiply.cpp"
#include "divide.cpp"
#include "mod.cpp"
#include "exp.cpp"

using namespace std;

class ExpressionParser {
private:
    std::map<char, std::unique_ptr<Calculation>> operators;
    std::map<std::string, double> variables;

public:
    ExpressionParser() {
        initializeOperators();
    }

    void initializeOperators() {
        operators['+'] = std::unique_ptr<Calculation>(new Addition());
        operators['-'] = std::unique_ptr<Calculation>(new Subtraction());
        operators['*'] = std::unique_ptr<Calculation>(new Multiplication());
        operators['/'] = std::unique_ptr<Calculation>(new Division());
        operators['%'] = std::unique_ptr<Calculation>(new Modulus());
        operators['^'] = std::unique_ptr<Calculation>(new Exp());
    }

    double parseExpression(const std::string& expression) {
        std::istringstream stream(expression);
        return parseExpression(stream);
    }

    double parseExpression(std::istringstream& stream) {
        std::stack<double> values;
        std::stack<char> ops;

        while (!stream.eof()) {
            if (isdigit(stream.peek()) || stream.peek() == '.') {
                double value;
                stream >> value;
                values.push(value);
            } else if (isalpha(stream.peek())) {
                std::string variableName;
                stream >> variableName;

                auto it = variables.find(variableName);
                if (it != variables.end()) {
                    values.push(it->second);
                } else {
                    throw std::invalid_argument("Undefined variable: " + variableName);
                }
            } else if (stream.peek() == '(') {
                stream.ignore();  // Consume '('
                values.push(parseExpression(stream));
                stream.ignore();  // Consume ')'
            } else if (isOperator(stream.peek())) {
                char op;
                stream >> op;

                while (!ops.empty() && hasHigherPrecedence(ops.top(), op)) {
                    applyOperation(values, ops);
                }

                ops.push(op);
            } else {
                stream.ignore();  // Ignore unexpected characters
            }
        }

        while (!ops.empty()) {
            applyOperation(values, ops);
        }

        if (values.size() == 1 && ops.empty()) {
            return values.top();
        } else {
            throw std::invalid_argument("Invalid expression");
        }
    }

    bool isOperator(char op) {
        return operators.find(op) != operators.end();
    }

    int getPrecedence(char op) {
        switch (op) {
            case '^':
                return 3;
            case '*':
            case '/':
            case '%':
                return 2;
            case '+':
            case '-':
                return 1;
            default:
                return 0;
        }
    }

    bool hasHigherPrecedence(char op1, char op2) {
        return getPrecedence(op1) >= getPrecedence(op2);
    }

    void applyOperation(std::stack<double>& values, std::stack<char>& ops) {
        if (values.size() < 2 || ops.empty()) {
            throw std::invalid_argument("Invalid expression");
        }

        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();

        auto it = operators.find(op);
        if (it != operators.end()) {
            double result = it->second->calculate(a, b);
            values.push(result);
        } else {
            throw std::invalid_argument("Invalid operator: " + std::string(1, op));
        }
    }

    void assignVariable(const std::string& variable, double value) {
        variables[variable] = value;
    }

    void printVariableTable() const {
        std::cout << "Variable Table:" << std::endl;
        for (const auto& pair : variables) {
            std::cout << pair.first << " = " << pair.second << std::endl;
        }
    }
};

int main() {
    ExpressionParser parser;  // Initialize the expression parser

    cout << "-=-=-=-=- Enter expression (e.g., A=2, A + 3), 'help' for menu, or 'exit' to terminate -=-=-=-=-" << endl;
    while (true) {
        cout << "Input: ";
        string input;
        getline(cin, input);

        if (input == "exit") {
            cout << "Exiting calculator. Goodbye!" << endl;
            break;
        } else if (input == "help") {
            cout << "Supported operations: +, -, *, /, % (modulus), ^ or ** (exponentiation)" << endl;
            cout << "You can also assign variables using character=number. Ex: A=12" << endl;
            cout << endl;
            continue;
        }

        try {
            double result = parser.parseExpression(input);
            cout << "Result: " << result << endl;
            parser.printVariableTable();
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}