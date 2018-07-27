#pragma once

struct LyndonFactorization {
    // Duval algorithm, O(n)
    static vector<string> factorize(const char* s, int n) {
        vector<string> factorization;

        int i = 0;
        while (i < n) {
            int j = i + 1, k = i;
            while (j < n && s[k] <= s[j]) {
                if (s[k] < s[j])
                    k = i;
                else
                    k++;
                j++;
            }
            while (i <= k) {
                factorization.push_back(string(s + i, j - k));
                i += j - k;
            }
        }
        return factorization;
    }

    static vector<string> factorize(const string& s) {
        return factorize(&s[0], int(s.length()));
    }


    // min cyclic shift of a string
    static string minCyclicShift(string s) {
        s += s;
        int n = int(s.size());

        int i = 0, ans = 0;
        while (i < n / 2) {
            ans = i;
            int j = i + 1, k = i;
            while (j < n && s[k] <= s[j]) {
                if (s[k] < s[j])
                    k = i;
                else
                    k++;
                j++;
            }
            while (i <= k)
                i += j - k;
        }
        return s.substr(ans, n / 2);
    }
};
