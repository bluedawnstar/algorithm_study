#pragma once

#include "prefixFunction.h"

// return max overlapping length between a's suffix and b's prefix
inline int maxOverlapLength(const string& a, const string& b) {
    int n = int(a.length()), m = int(b.length());
    PrefixFunction bPref(b);

    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && a[i] != b[j])
            j = bPref.pi[j - 1];

        if (a[i] == b[j])
            j++;
    }

    return j;
}

inline int maxOverlapLength(const string& a, const PrefixFunction& b) {
    int n = int(a.length()), m = int(b.p.length());

    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && a[i] != b.p[j])
            j = b.pi[j - 1];

        if (a[i] == b.p[j])
            j++;
    }

    return j;
}

inline vector<int> countAllPrefix(const PrefixFunction& pf) {
    int n = int(pf.p.length());

    vector<int> res(n + 1);
    // longest proper prefix-suffix
    for (int i = 0; i < n; i++)
        res[pf.pi[i]]++;
    // all proper subset of prefix-suffix
    for (int i = n - 1; i > 0; i--)
        res[pf.pi[i - 1]] += res[i];
    // each prefix itself
    for (int i = 0; i <= n; i++)
        res[i]++;

    res[0] = 0;

    return res;
}