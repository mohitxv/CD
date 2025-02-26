#include <iostream>
#include <stack>
#include <cctype>
#include <sstream>
using namespace std;

// Function to determine precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to perform arithmetic operations
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : throw runtime_error("Division by zero");
    }
    return 0;
}

// Function to evaluate an expression with parentheses
double evaluate(string expression) {
    stack<double> values;
    stack<char> ops;
    
    for (size_t i = 0; i < expression.length(); i++) {
        if (isspace(expression[i])) continue;
        
        if (isdigit(expression[i])) {
            double val = 0;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    i++;
                    double decimal = 0.1;
                    while (i < expression.length() && isdigit(expression[i])) {
                        val += (expression[i] - '0') * decimal;
                        decimal *= 0.1;
                        i++;
                    }
                    continue;
                }
                val = (val * 10) + (expression[i] - '0');
                i++;
            }
            values.push(val);
            i--;
        } else if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expression[i]);
        }
    }
    
    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }
    
    return values.top();
}

int main() {
    string expression;
    cout << "Enter an expression: ";
    getline(cin, expression);
    try {
        double result = evaluate(expression);
        cout << "Result: " << result << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}
