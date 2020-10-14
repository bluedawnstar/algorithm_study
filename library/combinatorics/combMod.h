#pragma once

#include "../integer/intMod.h"

// C(n,r) % 2
template <typename T>
inline T combMod2(T n, T r) {
    if (r & (n - r))
        return 0;
    else
        return 1;
}

template <typename T>
inline T combMod(int n, int r, T M) {
    if (n < r)
        return 0;

    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1, d = 1;
    for (int i = 1; i <= r; i++) {
        q = q * n-- % M;
        d = d * i % M;
    }

    return T(q * modInvIter((T)d, M) % M);
}

template <typename T>
inline T combModPrime(int n, int r, T M) {
    if (n < r)
        return 0;

    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1, d = 1;
    for (int i = 1; i <= r; i++) {
        q = q * n-- % M;
        d = d * i % M;
    }

    return T(q * modInvPrime((T)d, M) % M);
}

// Lucas' Theorem (http://bowbowbow.tistory.com/2)
template <typename T, typename U>
inline long long combLucasMod(T n, U r, int M) {
    if (n == 0 || r == 0 || n == r)
        return 1;

    return combLucasMod(n / M, r / M, M) * combModPrime(int(n % M), int(r % M), M) % M;
}
