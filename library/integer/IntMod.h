#pragma once

#include "gcd.h"

template <typename T, typename U>
inline U modAdd(T a, T b, U M) {
    return U(((a % M) + (b % M)) % M);
}

template <typename T, typename U>
inline U modSub(T a, T b, U M) {
    return U(((a % M) - (b % M) + M) % M);
}

template <typename T, typename U>
inline U modMul(T a, T b, U M) {
    return U(((a % M) * (b % M)) % M);
}

template <typename T, typename U>
inline U modPow(T x, T n, U M) {
    if (n == 0)
        return U(1);

    T p = modPow(x, n / 2, M) % M;
    p = p * p % M;

    return U((n & 1) == 0 ? p : p * x % M);
}


// a and M are coprime.
template <typename T, typename U>
inline U modInv(T a, U M) {
    T x, y;
    extGcd(a, (T)M, x, y);
    return U((x % M + M) % M);
}

// a and M are coprime
template <typename T, typename U>
inline U modInvIter(T a, U M) {
    T b = M;
    T y = 0, x = 1;

    if (M == 1)
        return 0;

    while (a > 1 && b != 0) {
        T q = a / b;

        T t = b;
        b = a % b;
        a = t;

        t = y;
        y = x - q * y;
        x = t;
    }

    return U((x % M + M) % M);
}

// M is a prime number.
template <typename T, typename U>
inline U modInvP(T a, U M) {
    return modPow(a, T(M - 2), M);
}


// a and M are coprime
template <typename T, typename U>
inline U modDiv(T a, T b, U M) {
    return U(((a % M) * modInverse(b, M)) % M);
}

// a and M are coprime
template <typename T, typename U>
inline U modDivIter(T a, T b, U M) {
    return U(((a % M) * modInvIter(b, M)) % M);
}

// M is a prime number.
template <typename T, typename U>
inline U modDivP(T a, T b, U M) {
    return U(((a % M) * modInvP(b, M)) % M);
}
