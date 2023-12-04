#include <iostream>
#include <stack>
#include <cmath>
#include <unordered_map>
#include <cctype>
#include <stdexcept>
#include <limits>

using namespace std;

// Represent NaN as a constant value. This helps to return 0 as a valid output and have an error code which isn't an int
const double NaN = numeric_limits<double>::quiet_NaN();

// Function to check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == '*';
}

// Function to get the precedence of an operator
int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;  // Parentheses
}

// Function to apply an operator to two operands
double applyOperator(char op, double operand1, double operand2) {
    switch (op) {
        // Simple expression for each operator using C++'s internal logic
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/':
            if (operand2 != 0) {
                return operand1 / operand2;
            } else {
                // check for divide by 0
                throw runtime_error("Error: Division by zero.");    
            }
        case '^': return pow(operand1, operand2);
        case '%':
            if (operand2 != 0) {
                return fmod(operand1, operand2);
            } else {
                // check for divide by 0
                throw runtime_error("Error: Modulo by zero.");
            }
        default: return 0;
    }
}

// Function to remove whitespace from a string
string removeWhitespace(const string& str) {
    string result;
    for (char c : str) {
        if (!isspace(c)) {
            result += c;
        }
    }
    return result;
}

// Function to check if a variable name is valid
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
    // Return true that name is valid if all tests pass
    return true;
}

// Function to check if an expression has balanced parentheses
bool isValidExpression(const string& expression) {
    // Set count to 0
    int parenCount = 0;
    for (char c : expression) {
        if (c == '(') {
            // If we encounter a left parenthesis then increment counter
            parenCount++;
        } else if (c == ')') {
            // Else if we encounter a right parenthesis then decrement
            parenCount--;
            if (parenCount < 0) {
                // If count is ever below 0 then a )( pattern has occured and parenthesis are invalid
                throw runtime_error("Error: Unmatched closing parenthesis.");
            }
        }
    }
    // Else if the count is greater than 0 a (() pattern has occured and parenthesis are invalid
    if (parenCount > 0) {
        throw runtime_error("Error: Unmatched opening parenthesis.");
    }

    // If no error has been thrown then all parentheses are matched and valid
    return parenCount == 0;  
}

// Function to evaluate an expression
double evaluateExpression(const string& expression, unordered_map<string, double>& variables) {

    // Create parsing stacks for both operators and operands
    stack<char> operators;
    stack<double> operands;

    try {
        // Check if the expression is valid
        if (!isValidExpression(expression)) {
            throw runtime_error("Error: Invalid expression. Unmatched parentheses.");
        }
        // If it is, then continue parsing
        for (size_t i = 0; i < expression.size(); ++i) {
            // Check for whitespace
            if (isspace(expression[i])) {
                continue;
            // Check if char is digit or . for float values
            } else if (isdigit(expression[i]) || expression[i] == '.') {
                size_t j = i;
                while (j < expression.size() && (isdigit(expression[j]) || expression[j] == '.')) {
                    ++j;
                }
                string num_str = expression.substr(i, j - i);
                i = j - 1;
                // Assign the current char as a num variable type and push to operands stack
                double num = stod(num_str);
                operands.push(num);
            // Check if char is alphabetic and used for variable assignment or parsing.
            } else if (isalpha(expression[i])) {
                size_t j = i;
                while (j < expression.size() && (isalnum(expression[j]) || expression[j] == '_')) {
                    ++j;
                }
                string var_name = expression.substr(i, j - i);
                i = j - 1;

                // Remove whitespace from the variable name
                string cleaned_var_name = removeWhitespace(var_name);
                // Check if variable name is pi or e
                if (cleaned_var_name == "pi" || cleaned_var_name == "e") {
                    // If they are, assign each to their mathematical value; pi=3.1415... and e=euler's number
                    operands.push((cleaned_var_name == "pi") ? M_PI : M_E);
                // Else check if they are already in the lookup table
                } else if (variables.find(cleaned_var_name) != variables.end()) {
                    // If they are in the table then use them in the expression as a operand
                    operands.push(variables[cleaned_var_name]);
                // Else throw missing variable error 
                } else {
                    throw runtime_error("Error: Variable '" + cleaned_var_name + "' not defined.");
                }
            // Check for parenthesis
            } else if (expression[i] == '(') {
                operators.push('(');
            } else if (expression[i] == ')') {
                // Activate inner parenthesis parsing separately to make sure PEMDAS is accurately followed
                while (!operators.empty() && operators.top() != '(') {
                    double operand2 = operands.top();
                    operands.pop();
                    double operand1 = operands.top();
                    operands.pop();
                    char op = operators.top();
                    operators.pop();
                    // Apply Expression function to get true value for inner expression
                    operands.push(applyOperator(op, operand1, operand2));
                }
                operators.pop();  // Pop '('
            // Check for exponents with either a * followed by another * or ^
            } else if (expression[i] == '*' && expression[i + 1] == '*') {
                operators.push('^');  // Treat "**" as exponentiation
                ++i;  // Skip the second '*' character
            } else if (isOperator(expression[i])) {
                // Handle unary minus
                if ((expression[i] == '-' || expression[i] == '+') && (i == 0 || expression[i - 1] == '(' || isOperator(expression[i - 1]))) {
                    // If unary operator is + just skip it
                    if (expression[i] == '+'){
                        continue;
                    }
                    // Else, push unary minus as multiplication by -1
                    else{
                        operators.push('*');
                        operands.push(-1.0);
                    }
                } else {
                    // Check for missing operand on the left
                    if (i == 0 || expression[i - 1] == '(' || isOperator(expression[i - 1])) {
                        throw runtime_error("Error: Missing operand on the left of operator.");
                    }

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

                    // Check for missing operand on the right
                    size_t j = i + 1;
                    if (j == expression.size() || expression[j] == ')' || isOperator(expression[j])) {
                        throw runtime_error("Error: Missing operand on the right of operator.");
                    }
                }
            } else {
                // Catch all other errors
                throw runtime_error("Error: Invalid character in expression.");
            }
        }
        // Once parsing is complete apply the matematical operators in order of PEMDAS
        while (!operators.empty()) {
            double operand2 = operands.top();
            operands.pop();
            double operand1 = operands.top();
            operands.pop();
            char op = operators.top();
            operators.pop();

            // Check for unary minus
            if (op == '*' && operand1 == -1.0) {
                operands.push(-operand2);  // Apply unary minus
            } else {
                operands.push(applyOperator(op, operand1, operand2));
            }
        }

        if (operands.size() != 1 || !operators.empty()) {
            // If there is a size mismatch between number of operands and operators then throw expression error
            throw runtime_error("Error: Invalid expression.");
        }

    // Catch all errors here and print them out to line. Return NaN value to result
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        while (!operators.empty()) operators.pop();
        while (!operands.empty()) operands.pop();
        return NaN;
    }

    // Otherwise, if parsing and expressing went well, return top of operands stack which should be final value of expression
    return operands.top();
}

// Main function
int main() {
    // String input as variable user input
    string input;
    // Map to store variable names and values
    unordered_map<string, double> variables;

    // Begin program loop
    cout << "-=-=-=-=- Enter expression (e.g., A=2, A + 3), 'help' for menu, or 'exit' to quit -=-=-=-=-" << endl;
    while (true) {
        // Prompt user for input
        cout << "Input: ";
        getline(cin, input);
        // If user inputs exit command break program loop
        if (input == "exit") {
            cout << "Exiting calculator. Goodbye!" << endl;
            cout << endl;
            break;
        // If user inputs help command, print help table
        } else if (input == "help") {
            cout << "Supported operations: +, -, *, /, % (modulus), ^ (exponentiation)" << endl;
            cout << "You can also assign variables using character=number. Ex: A=12 (which is case sensitive)" << endl;
            cout << endl;
            continue;
        }
        // Check for variable assignment in user input
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
                // Can't be pi or e as those are already taken and not displayed in variable table
            } else if (cleaned_var_name == "pi" || cleaned_var_name == "e") {
                cerr << "Error: Cannot assign to constant variable '" << cleaned_var_name << "'." << endl;
            } else {
                double result = evaluateExpression(expression, variables);
                if (result != 0) {
                    variables[cleaned_var_name] = result;

                    // Show current variable values after a variable has been assigned or updated
                    cout << "Current variable values:" << endl;
                    for (const auto& entry : variables) {
                        cout << entry.first << " = " << entry.second << endl;
                    }
                    cout << endl;
                }
            }
        } else {
            // Expression evaluation
            double result = evaluateExpression(input, variables);
            if (!isnan(result)) {
                // Print expression results to screen if successfully parsed
                cout << "Result: " << result << endl;
                cout << endl;
            }
        }
    }
    
    return 0;
}