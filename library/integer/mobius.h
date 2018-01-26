#pragma once

#include "primeFactor.h"

/*
1. Mobius Function
  mu(n) = 1        , n = 1
          0        , n > 1, n has a squared prime factor
          (-1)^k   , n > 1, n is the product of k distinct primes
*/

// O(n*loglogn)
inline int mobius(int n) {
    if (n == 1)
        return 1;

    int k = 0;
    if (n % 2 == 0) {
        n /= 2;
        k++;

        if (n % 2 == 0)
            return 0;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (n % i == 0) {
            n /= i;
            k++;

            if (n % i == 0)
                return 0;
        }
    }

    if (n > 1)
        k++;

    return (k & 1) ? -1 : 1;
}

// O(n)
inline vector<int> mobiusSeive(int n, const MinFactors& minFactors) {
    vector<int> res(n + 1);
    if (n > 0)
        res[1] = 1;

    for (int i = 2; i <= n; i++) {
        int w = minFactors.minFactors[i];
        res[i] = (minFactors.minFactors[i / w] == w) ? 0 : -res[i / w];
    }

    return res;
}

inline vector<bool> squareFree(int n) {
    vector<bool> res(n + 1, true);

    if (n >= 4) {
        for (int j = 2 * 2; j <= n; j += 2 * 2)
            res[j] = false;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (res[i]) {
            int delta = i * i;
            for (int j = delta; j >= 0 && j <= n; j += delta)
                res[j] = false;
        }
    }

    return res;
}
