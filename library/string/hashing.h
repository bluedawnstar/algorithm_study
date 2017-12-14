#pragma once

inline unsigned long long genHash(const char* s, int len) {
    const int p = 31;
    unsigned long long res = 0, pow = 1;
    for (int i = 0; i < len; i++) {
        res += (s[i] - 'a' + 1) * pow;
        pow *= p;
    }
    return res;
}

inline unsigned long long genHash(const string& s) {
    return genHash((const char*)&s[0], (int)s.length());
}