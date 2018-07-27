#pragma once

#include "prefixFunction.h"

// return max overlapping length between a's suffix and b's prefix
int maxOverlapLength(const string& a, const string& b) {
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

int maxOverlapLength(const string& a, const PrefixFunction& b) {
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
