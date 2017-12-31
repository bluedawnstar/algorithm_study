#pragma once

// Shunting-yard algorithm
struct ExpressionParserShuntingYard {
    static int eval(string s) {
        vector<int> st;
        string op;
        for (int i = 0; i < (int)s.length(); i++) {
            char c = s[i];
            if (isDelim(c))
                continue;
            if (c == '(')
                op += '(';
            else if (c == ')') {
                while (op.back() != '(') {
                    processOperator(st, op.back());
                    op.pop_back();
                }
                op.pop_back();
            } else if (isOperator(c)) {
                while (!op.empty() && priority(c) <= priority(op.back())) {
                    processOperator(st, op.back());
                    op.pop_back();
                }
                op += c;
            } else {
                string operand = "";
                while (i < (int)s.length() && isdigit(s[i]))
                    operand += s[i++];

                --i;
                st.push_back(stoi(operand));
            }
        }
        while (!op.empty()) {
            processOperator(st, op.back());
            op.pop_back();
        }
        return st[0];
    }

private:
    static bool isDelim(char c) {
        return c == ' ';
    }

    static bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
    }

    static int priority(char op) {
        switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return -1;
        }
    }

    static void processOperator(vector<int>& st, char op) {
        int r = st.back(); st.pop_back();
        int l = st.back(); st.pop_back();
        switch (op) {
        case '+':
            st.push_back(l + r);
            break;
        case '-':
            st.push_back(l - r);
            break;
        case '*':
            st.push_back(l * r);
            break;
        case '/':
            st.push_back(l / r);
            break;
        case '%':
            st.push_back(l % r);
            break;
        }
    }
};

// Recursive descent algorithm
struct ExpressionParserRecursiveDescent {
    double eval(const string& exp) {
        double res = 0;

        init(exp);

        next();
        while (token != 255) {
            if (token == '\n') {
                skip('\n');
                continue;
            }
            res = expression();
        }

        return res;
    }

private:
    string s;
    int pos;
    int token;
    double tokval;

    void init(const string& exp) {
        pos = 0;
        token = 0;
        tokval = 0;
        s = exp;
    }

    int next() {
        static const string s_operators("+-*/^()\n");

        while (true) {
            if (pos == s.length())
                return token = 255;

            char c = s[pos++];
            if (s_operators.find(c) != string::npos)
                return token = c;

            if (isspace(c))
                continue;

            if (isdigit(c) || c == '.') {
                string operand(1, c);
                while (pos < (int)s.length() && isDigitOrDot(s[pos]))
                    operand += c = s[pos++];
                tokval = stod(operand);
                return token = 'n';
            }

            return -1; // bad character
        }
    }

    void skip(int ch) {
        if (token == ch)
            next();
    }

    // number ::= number | '(' expression ')'
    double number() {
        if (token == 'n') {
            double v = tokval;
            skip('n');
            return v;
        }
        skip('(');
        double v = expression();
        skip(')');
        return v;
    }

    // factor ::= number | number '^' factor
    double factor() {
        double v = number();
        if (token == '^') {
            skip('^');
            v = pow(v, factor());
        }
        return v;
    }

    // term ::= factor | term '*' factor | term '/' factor
    double term() {
        double v = factor();
        while (true) {
            if (token == '*') {
                skip('*');
                v *= factor();
            } else if (token == '/') {
                skip('/');
                v /= factor();
            } else
                return v;
        }
    }

    // expression ::= term | expression '+' term | expression '-' term
    double expression() {
        double v = term();
        while (true) {
            if (token == '+') {
                skip('+');
                v += term();
            } else if (token == '-') {
                skip('-');
                v -= term();
            } else
                return v;
        }
    }


    static bool isDigitOrDot(char x) {
        return isdigit(x) || x == '.';
    }
};
