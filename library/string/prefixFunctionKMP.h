#pragma once

#include "prefixFunction.h"

// return matched lengths at each position
vector<int> kmpLength(const string& s, const PrefixFunction& pref) {
    vector<int> res;

    int n = (int)s.size(), m = (int)pref.p.length(), j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != pref.p[j])
            j = pref.pi[j - 1];

        if (s[i] == pref.p[j]) {
            res.push_back(j + 1);
            if (j == m - 1) {
                //P is found at i - m + 1
                j = pref.pi[j];
            } else {
                j++;
            }
        } else {
            res.push_back(j);
        }
    }

    return res;
}

vector<int> kmp(const string& s, const PrefixFunction& pref) {
    vector<int> res;

    int n = (int)s.length(), m = (int)pref.p.length(), j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != pref.p[j])
            j = pref.pi[j - 1];

        if (s[i] == pref.p[j]) {
            if (j == m - 1) {
                res.push_back(i - m + 1);
                j = pref.pi[j];
            } else {
                j++;
            }
        }
    }

    return res;
}
