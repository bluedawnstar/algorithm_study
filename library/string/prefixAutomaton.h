#pragma once

// ref: https://cp-algorithms.com/string/prefix-function.html

template <int MaxCharN = 26, int BaseChar = 'a'>
struct PrefixAutomaton {
    static constexpr int ch2i(int c) {
        return c - BaseChar;
    }

    static constexpr char i2ch(int c) {
        return c + BaseChar;
    }

    int N;
    vector<int> pi;
    vector<array<int,MaxCharN>> automaton;  // [oldPi][c] = newPi

    PrefixAutomaton() : N(0) {
    }

    PrefixAutomaton(const char* s, int n) {
        build(s, n);
    }

    explicit PrefixAutomaton(const string& s) {
        build(s);
    }

    // O(|s|)
    void build(const char* s, int n) {
        pi = prefixFunction(s, n);  // s + '#'

        N = n;
        automaton.resize(n + 1);
        for (int i = 0; i <= n; i++) {
            int curr = ch2i(s[i]);
            for (int c = 0; c < MaxCharN; c++) {
                if (i > 0 && c != curr)
                    automaton[i][c] = automaton[pi[i - 1]][c];
                else
                    automaton[i][c] = i + (c == curr);
            }
        }
    }

    // O(|s|)
    void build(const string& s) {
        build(s.c_str(), int(s.length()));
    }

    //---

    // get PI of T in S + '#' + T, O(|T|)
    vector<int> getPI(const char* t, int len) {
        vector<int> pi(len);

        pi[0] = automaton[0][ch2i(t[0])];
        for (int i = 1; i < len; i++)
            pi[i] = automaton[pi[i - 1]][ch2i(t[i])];

        return pi;
    }

    vector<int> getPI(const string& t) {
        return getPI(t.c_str(), int(t.length()));
    }

    // find all occurrences of the string S in the string T (S + '#' + T), O(|T|)
    // - return starting points in T
    vector<int> findAll(const char* t, int len) {
        vector<int> res;

        int pi = automaton[0][ch2i(t[0])];
        if (pi == N)
            res.push_back(0);

        for (int i = 1; i < len; i++) {
            pi = automaton[pi][ch2i(t[i])];
            if (pi == N)
                res.push_back(i - N + 1);
        }

        return res;
    }

    vector<int> findAll(const string& t) {
        return findAll(t.c_str(), int(t.length()));
    }

private:
    // s + '#'
    static vector<int> prefixFunction(const char* s, int n) {
        vector<int> pi(n + 1);

        int j = 0;
        for (int i = 1; i < n; i++) {
            while (j > 0 && s[j] != s[i])
                j = pi[j - 1];

            if (s[j] == s[i])
                j++;
            pi[i] = j;
        }
        // '#'
        pi[n] = 0;

        return pi;
    }
};
