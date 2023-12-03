#include <iostream>
#include <stack>
#include <cmath>
#include <unordered_map>
#include <cctype>

using namespace std;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == '*';
}

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;  // Parentheses
}

double applyOperator(char op, double operand1, double operand2, bool& error) {
    error = false;
    switch (op) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/':
            if (operand2 != 0) {
                return operand1 / operand2;
            } else {
                cerr << "Error: Division by zero." << endl;
                error = true;
                return 0;
            }
        case '^': return pow(operand1, operand2);
        case '%':
            if (operand2 != 0) {
                return fmod(operand1, operand2);
            } else {
                cerr << "Error: Modulo by zero." << endl;
                error = true;
                return 0;
            }
        default: return 0;
    }
}

string removeWhitespace(const string& str) {
    string result;
    for (char c : str) {
        if (!isspace(c)) {
            result += c;
        }
    }
    return result;
}

bool isValidVariableName(const string& var_name) {
    // Remove whitespace from the variable name
    string cleaned_var_name = removeWhitespace(var_name);

    // Check if the variable name is non-empty
    if (cleaned_var_name.empty()) {
        return false;
    }

    // Check if the first character is a letter or underscore
    if (!isalpha(cleaned_var_name[0]) && cleaned_var_name[0] != '_') {
        return false;
    }

    // Check if the rest of the characters are alphanumeric or underscores
    for (size_t i = 1; i < cleaned_var_name.size(); ++i) {
        if (!isalnum(cleaned_var_name[i]) && cleaned_var_name[i] != '_') {
            return false;
        }
    }

    return true;
}

bool isValidExpression(const string& expression) {
    int parenCount = 0;
    for (char c : expression) {
        if (c == '(') {
            parenCount++;
        } else if (c == ')') {
            parenCount--;
            if (parenCount < 0) {
                return false;  // Unmatched closing parenthesis
            }
        }
    }
    return parenCount == 0;  // All parentheses are matched
}

double evaluateExpression(const string& expression, unordered_map<string, double>& variables) {
    stack<char> operators;
    stack<double> operands;

    if (!isValidExpression(expression)) {
        cerr << "Error: Invalid expression. Unmatched parentheses." << endl;
        return 0;
    }

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

            // Remove whitespace from the variable name
            string cleaned_var_name = removeWhitespace(var_name);

            if (cleaned_var_name == "pi" || cleaned_var_name == "e") {
                operands.push((cleaned_var_name == "pi") ? M_PI : M_E);
            } else if (variables.find(cleaned_var_name) != variables.end()) {
                operands.push(variables[cleaned_var_name]);
            } else {
                cerr << "Error: Variable '" << cleaned_var_name << "' not defined." << endl;
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
                bool error;
                operands.push(applyOperator(op, operand1, operand2, error));
                if (error) {
                    return 0;
                }
            }
            operators.pop();  // Pop '('
        } else if (expression[i] == '*' && expression[i + 1] == '*') {
            operators.push('^');  // Treat "**" as exponentiation
            ++i;  // Skip the second '*' character
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
                    bool error;
                    operands.push(applyOperator(op, operand1, operand2, error));
                    if (error) {
                        return 0;
                    }
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
        bool error;
        operands.push(applyOperator(op, operand1, operand2, error));
        if (error) {
            return 0;
        }
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

            // Remove whitespace from the variable name
            string cleaned_var_name = removeWhitespace(var_name);

            // Check if the variable name is valid
            if (!isValidVariableName(cleaned_var_name)) {
                cerr << "Error: Invalid variable name '" << var_name << "'." << endl;
            } else if (cleaned_var_name == "pi" || cleaned_var_name == "e") {
                cerr << "Error: Cannot assign to constant variable '" << cleaned_var_name << "'." << endl;
            } else {
                double result = evaluateExpression(expression, variables);
                if (result != 0) {
                    variables[cleaned_var_name] = result;

                    // Show current variable values
                    cout << "Current variable values:" << endl;
                    for (const auto& entry : variables) {
                        cout << entry.first << " = " << entry.second << endl;
                    }
                }
            }
        } else {
            // Expression evaluation
            double result = evaluateExpression(input, variables);
            if (result != 0) {
                cout << "Result: " << result << endl;
            }
        }
    }

    return 0;
}
