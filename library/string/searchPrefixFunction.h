#pragma once

inline vector<int> prefixFunction(const string& s) {
    vector<int> pi(s.size());
    int j = 0;
    for (int i = 1; i < int(s.size()); i++) {
        while (j > 0 && s[j] != s[i])
            j = pi[j - 1];

        if (s[j] == s[i])
            j++;
        pi[i] = j;
    }
    return pi;
}

inline vector<int> prefixFunction(const string& s, int start) {
    int n = int(s.size()) - start;

    vector<int> pi(n);
    int j = 0;
    for (int i = 1; i < n; i++) {
        while (j > 0 && s[j + start] != s[i + start])
            j = pi[j - 1];

        if (s[j + start] == s[i + start])
            j++;
        pi[i] = j;
    }
    return pi;
}


//--- KMP

// return matched lengths at each position
inline vector<int> kmpL(const string& s, const string& p) {
    vector<int> res;
    vector<int> pi = prefixFunction(p);

    int n = int(s.size()), m = int(p.size()), j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != p[j])
            j = pi[j - 1];

        if (s[i] == p[j]) {
            res.push_back(j + 1);
            if (j == m - 1) {
                // p is found at (i - m + 1)
                j = pi[j];
            } else {
                j++;
            }
        } else {
            res.push_back(j);
        }
    }

    return res;
}

inline vector<int> kmp(const string& s, const string& p) {
    vector<int> res;
    vector<int> pi = prefixFunction(p);

    int n = int(s.size());
    int m = int(p.size());
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != p[j])
            j = pi[j - 1];

        if (s[i] == p[j]) {
            if (j == m - 1) {
                res.push_back(i - m + 1);
                j = pi[j];
            } else {
                j++;
            }
        }
    }

    return res;
}

inline vector<int> kmp(const string& s, const string& p, const vector<int>& pi) {
    vector<int> res;

    int n = int(s.size());
    int m = int(p.size());
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != p[j])
            j = pi[j - 1];

        if (s[i] == p[j]) {
            if (j == m - 1) {
                res.push_back(i - m + 1);
                j = pi[j];
            } else {
                j++;
            }
        }
    }

    return res;
}
