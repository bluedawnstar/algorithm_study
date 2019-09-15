#pragma once

// https://leetcode.com/problems/reverse-substrings-between-each-pair-of-parentheses/

struct ReverseSubstringsBetweenParentheses {
    // O(N^2)
    static string reverseParenthesesNaive(string s) {
        int N = int(s.length());

        vector<int> stk;
        for (int i = 0; i < N; i++) {
            if (s[i] == '(')
                stk.push_back(i);
            else if (s[i] == ')') {
                int left = stk.back();
                reverse(s.begin() + left + 1, s.begin() + i);
                stk.pop_back();
            }
        }

        string res;
        for (auto c : s) {
            if (c != '(' && c != ')')
                res.push_back(c);
        }
        return res;
    }

    // O(N)
    static string reverseParentheses(string s) {
        int N = int(s.length());

        vector<int> par(N, -1);
        vector<int> stk;
        for (int i = 0; i < N; i++) {
            if (s[i] == '(')
                stk.push_back(i);
            else if (s[i] == ')') {
                int left = stk.back();
                stk.pop_back();

                par[left] = i;
                par[i] = left;
            }
        }

        string res;
        int delta = 1;  // 1: forward, -1: backward

        for (int i = 0; i < N; ) {
            if (s[i] == '(' || s[i] == ')') {
                delta = -delta;
                i = par[i] + delta;
            } else {
                res.push_back(s[i]);
                i += delta;
            }
        }

        return res;
    }
};
