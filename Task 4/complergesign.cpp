#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <stdexcept>

using namespace std;

string expr;
int pos = 0;
map<string, double> variables;

void skipWhitespace() {
    while (pos < expr.length() && isspace(expr[pos])) pos++;
}

double parseExpression();  // Forward declaration

// Parse a number or variable
double parseFactor() {
    skipWhitespace();

    if (expr[pos] == '(') {
        pos++;
        double val = parseExpression();
        if (expr[pos] != ')') throw runtime_error("Expected ')'");
        pos++;
        return val;
    }

    // Negative sign
    if (expr[pos] == '-') {
        pos++;
        return -parseFactor();
    }

    // Variable or number
    if (isalpha(expr[pos])) {
        string varName;
        while (pos < expr.length() && isalnum(expr[pos])) {
            varName += expr[pos++];
        }
        if (variables.count(varName)) {
            return variables[varName];
        } else {
            throw runtime_error("Undefined variable: " + varName);
        }
    }

    // Number
    double number = 0;
    while (pos < expr.length() && isdigit(expr[pos])) {
        number = number * 10 + (expr[pos++] - '0');
    }
    if (expr[pos] == '.') {
        pos++;
        double frac = 0.1;
        while (pos < expr.length() && isdigit(expr[pos])) {
            number += (expr[pos++] - '0') * frac;
            frac *= 0.1;
        }
    }
    return number;
}

double parseTerm() {
    double val = parseFactor();
    skipWhitespace();
    while (pos < expr.length() && (expr[pos] == '*' || expr[pos] == '/')) {
        char op = expr[pos++];
        double right = parseFactor();
        if (op == '*') val *= right;
        else val /= right;
        skipWhitespace();
    }
    return val;
}

double parseExpression() {
    double val = parseTerm();
    skipWhitespace();
    while (pos < expr.length() && (expr[pos] == '+' || expr[pos] == '-')) {
        char op = expr[pos++];
        double right = parseTerm();
        if (op == '+') val += right;
        else val -= right;
        skipWhitespace();
    }
    return val;
}

void handleStatement() {
    skipWhitespace();
    if (isalpha(expr[pos])) {
        int start = pos;
        while (pos < expr.length() && isalnum(expr[pos])) pos++;
        string varName = expr.substr(start, pos - start);
        skipWhitespace();
        if (expr[pos] == '=') {
            pos++;
            double val = parseExpression();
            variables[varName] = val;
            cout << varName << " = " << val << endl;
            return;
        } else {
            pos = start;
        }
    }

    double result = parseExpression();
    cout << "Result: " << result << endl;
}

int main() {
    cout << "Enter expressions (type 'exit' to quit):\n";
    while (true) {
        cout << "> ";
        getline(cin, expr);
        if (expr == "exit") break;
        try {
            pos = 0;
            handleStatement();
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    return 0;
}