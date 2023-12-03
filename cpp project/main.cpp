#include <iostream>
#include <stack>
#include <cmath>
#include <unordered_map>

using namespace std;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;  // Parentheses
}

double applyOperator(char op, double operand1, double operand2) {
    switch (op) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/': return operand1 / operand2;
        case '^': return pow(operand1, operand2);
        case '%': return fmod(operand1, operand2);
        default: return 0;
    }
}

double evaluateExpression(const string& expression, unordered_map<string, double>& variables) {
    stack<char> operators;
    stack<double> operands;

    for (size_t i = 0; i < expression.size(); ++i) {
        if (isspace(expression[i])) {
            continue;
        } else if (isdigit(expression[i]) || expression[i] == '.') {
            size_t j = i;
            while (j < expression.size() && (isdigit(expression[j]) || expression[j] == '.')) {
                ++j;
            }
            string num_str = expression.substr(i, j - i);
            i = j - 1;
            double num = stod(num_str);
            operands.push(num);
        } else if (isalpha(expression[i])) {
            size_t j = i;
            while (j < expression.size() && (isalnum(expression[j]) || expression[j] == '_')) {
                ++j;
            }
            string var_name = expression.substr(i, j - i);
            i = j - 1;

            if (var_name == "pi" || var_name == "e") {
                operands.push((var_name == "pi") ? M_PI : M_E);
            } else if (variables.find(var_name) != variables.end()) {
                operands.push(variables[var_name]);
            } else {
                cerr << "Error: Variable '" << var_name << "' not defined." << endl;
                return 0;
            }
        } else if (expression[i] == '(') {
            operators.push('(');
        } else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double operand2 = operands.top();
                operands.pop();
                double operand1 = operands.top();
                operands.pop();
                char op = operators.top();
                operators.pop();
                operands.push(applyOperator(op, operand1, operand2));
            }
            operators.pop();  // Pop '('
        } else if (isOperator(expression[i])) {
            // Handle unary minus
            if ((expression[i] == '-' || expression[i] == '+') && (i == 0 || expression[i - 1] == '(' || isOperator(expression[i - 1]))) {
                int j = i + 1;
                while (j < expression.size() && (isdigit(expression[j]) || expression[j] == '.')) {
                    ++j;
                }
                string num_str = expression.substr(i, j - i);
                i = j - 1;
                double num = stod(num_str);
                operands.push(num);
            } else {
                while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(expression[i])) {
                    double operand2 = operands.top();
                    operands.pop();
                    double operand1 = operands.top();
                    operands.pop();
                    char op = operators.top();
                    operators.pop();
                    operands.push(applyOperator(op, operand1, operand2));
                }
                operators.push(expression[i]);
            }
        } else {
            cerr << "Error: Invalid character in expression." << endl;
            return 0;
        }
    }

    while (!operators.empty()) {
        double operand2 = operands.top();
        operands.pop();
        double operand1 = operands.top();
        operands.pop();
        char op = operators.top();
        operators.pop();
        operands.push(applyOperator(op, operand1, operand2));
    }

    if (operands.size() != 1 || !operators.empty()) {
        cerr << "Error: Invalid expression." << endl;
        return 0;
    }

    return operands.top();
}

int main() {
    string input;
    unordered_map<string, double> variables;

    while (true) {
        cout << "Enter expression or variable assignment (or 'exit' to quit): ";
        getline(cin, input);

        if (input == "exit") {
            break;
        }

        size_t assignmentPos = input.find('=');

        if (assignmentPos != string::npos) {
            // Variable assignment
            string var_name = input.substr(0, assignmentPos);
            string expression = input.substr(assignmentPos + 1);
            
            // Prevent assigning to "pi" and "e"
            if (var_name == "pi" || var_name == "e") {
                cerr << "Error: Cannot assign to constant variable '" << var_name << "'." << endl;
            } else {
                double result = evaluateExpression(expression, variables);
                variables[var_name] = result;

                // Show current variable values
                cout << "Current variable values:" << endl;
                for (const auto& entry : variables) {
                    cout << entry.first << " = " << entry.second << endl;
                }
            }
        } else {
            // Expression evaluation
            double result = evaluateExpression(input, variables);
            cout << "Result: " << result << endl;
        }
    }

    return 0;
}
