#pragma once

inline vector<int> makeZArray(const char* s, int len) {
    vector<int> Z(len);
    Z[0] = 0;

    int L, R, k;

    L = R = 0;
    for (int i = 1; i < len; ++i) {
        if (i > R) {
            L = R = i;

            while (R < len && s[R - L] == s[R])
                R++;
            Z[i] = R - L;
            R--;
        } else {
            k = i - L;
            if (Z[k] < R - i + 1)
                Z[i] = Z[k];
            else {
                L = i;
                while (R < len && s[R - L] == s[R])
                    R++;
                Z[i] = R - L;
                R--;
            }
        }
    }

    return Z;
}

inline vector<int> makeZArray(const string& s) {
    return makeZArray(&s[0], (int)s.length());
}

inline vector<int> searchZ(const string& text, const string& pattern) {
    vector<int> res;

    string concat = pattern + "$" + text;
    int len = (int)concat.length();

    vector<int> Z = makeZArray(concat);
    for (int i = 0; i < len; i++) {
        if (Z[i] == pattern.length())
            res.push_back(i - (int)pattern.length() - 1);
    }

    return res;
}
