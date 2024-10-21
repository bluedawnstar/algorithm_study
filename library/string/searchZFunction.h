#pragma once

// An implementation of the algorithm as is, O(n)
inline vector<int> makeZArrayVerbose(const char* s, int len) {
    vector<int> z(len);
    z[0] = 0;

    int L = 0, R = 0;
    for (int i = 1; i < len; i++) {
        // update z[i]
        int j;
        if (i >= R) {
            j = 0;
            while (i + j < len && s[i + j] == s[j])
                j++;
        } else {
            if (i + z[i - L] < R)
                j = z[i - L];
            else {
                j = R - i;
                while (i + j < len && s[i + j] == s[j])
                    j++;
            }
        }
        z[i] = j;

        // update [L, R)
        if (i + j > R) {
            L = i;
            R = i + j;
        }
    }

    return z;
}

inline vector<int> makeZArrayVerbose(const string& s) {
    return makeZArrayVerbose(s.c_str(), int(s.length()));
}

//---

// O(n)
inline vector<int> makeZArray(const char* s, int len) {
    vector<int> z(len);
    z[0] = 0;

    int L = 0, R = 0;
    for (int i = 1; i < len; i++) {
        // update z[i]
        int j = 0;
        if (i < R)
            j = min(z[i - L], R - i);
        while (i + j < len && s[i + j] == s[j])
            j++;
        z[i] = j;

        // update [L, R)
        if (i + z[i] > R) {
            L = i;
            R = i + z[i];
        }
    }

    return z;
}

inline vector<int> makeZArray(const string& s) {
    return makeZArray(&s[0], int(s.length()));
}

//---

inline vector<int> searchZSimple(const string& text, const string& pattern) {
    vector<int> res;

    string concat = pattern + "$" + text;
    int len = int(concat.length());

    vector<int> Z = makeZArray(concat);
    for (int i = 0; i < len; i++) {
        if (Z[i] == pattern.length())
            res.push_back(i - int(pattern.length()) - 1);
    }

    return res;
}

inline vector<int> searchZ(const string& s, const string& p) {
    vector<int> z = makeZArray(p);
    vector<int> res;

    int n = int(s.length());
    int m = int(p.length());
    int L = 0, R = 0;
    for (int i = 0; i < n; i++) {
        // update z[i]
        int j = 0;
        if (i < R)
            j = min(z[i - L], R - i);
        while (i + j < n && j < m && s[i + j] == p[j])
            j++;

        if (j == m)
            res.push_back(i);

        // update [L, R)
        if (i + j > R) {
            L = i;
            R = i + j;
        }
    }

    return res;
}

inline vector<int> searchZ(const string& s, const string& p, const vector<int>& z) {
    vector<int> res;

    int n = int(s.length());
    int m = int(p.length());
    int L = 0, R = 0;
    for (int i = 0; i < n; i++) {
        // update z[i]
        int j = 0;
        if (i < R)
            j = min(z[i - L], R - i);
        while (i + j < n && j < m && s[i + j] == p[j])
            j++;

        if (j == m)
            res.push_back(i);

        // update [L, R)
        if (i + j > R) {
            L = i;
            R = i + j;
        }
    }

    return res;
}
