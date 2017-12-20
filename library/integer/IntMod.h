#pragma once

#include "gcd.h"

//--- int version -------------------------------

inline int modAdd(int a, int b, int M) {
    return ((a % M) + (b % M)) % M;
}

inline int modSub(int a, int b, int M) {
    return ((a % M) - (b % M) + M) % M;
}

inline int modMul(int a, int b, int M) {
    return int((long long)(a % M) * (b % M) % M);
}

inline int modPow(int x, int n, int M) {
    if (n == 0)
        return 1;

    int p = modPow(x, n / 2, M) % M;
    p = int((long long)p * p % M);

    return ((n & 1) == 0) ? p : int((long long)p * x % M);
}

// M is a prime number
inline int modPowPrime(int x, long long n, int M) {
    return modPow(x, int(n % (M - 1)), M);
}

// a and M are coprime.
inline int modInv(int a, int M) {
    int x, y;
    extGcd(a, M, x, y);
    //if (extGcd(a, M, x, y) > 1)
    //    return -1;
    return (x % M + M) % M;
}

// a and M are coprime
inline int modInvIter(int a, int M) {
    int b = M;
    int y = 0, x = 1;

    if (M == 1)
        return 0;

    while (a > 1 && b != 0) {
        int q = a / b;

        int t = b;
        b = a % b;
        a = t;

        t = y;
        y = x - q * y;
        x = t;
    }

    return (x % M + M) % M;
}

// M is a prime number.
inline int modInvPrime(int a, int M) {
    return modPow(a, M - 2, M);
}

// a and M are coprime
inline int modDiv(int a, int b, int M) {
    return int((long long)(a % M) * modInv(b, M) % M);
}

// a and M are coprime
inline int modDivIter(int a, int b, int M) {
    return int((long long)(a % M) * modInvIter(b, M) % M);
}

// M is a prime number.
inline int modDivPrime(int a, int b, int M) {
    return int((long long)(a % M) * modInvPrime(b, M) % M);
}


//http://codeforces.com/blog/entry/13908#comment-188854
// M is a prime number and n < M
inline vector<int> modInvPrimeRange(int n, int M) {
    vector<int> res(n + 1);
    res[0] = 0;
    res[1] = 1;
    for (int i = 2; i <= n; i++) {
        res[i] = (M - (M / i) * res[M % i] % M) % M;
    }

    return res;
}


//--- long long version -------------------------

inline long long modAdd(long long a, long long b, long long M) {
    return ((a % M) + (b % M)) % M;
}

inline long long modSub(long long a, long long b, long long M) {
    return ((a % M) - (b % M) + M) % M;
}


inline long long modMul(long long a, long long b, long long M) {
#ifdef __GNUC__
    return long long((__int128_t)(a % M) * (b % M) % M);
#else
        int base = (int)1e9;
        long long aLow = a % base, aHigh = a / base;
        long long bLow = b % base, bHigh = b / base;

        long long result = (aHigh * bHigh) % M;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % M;

        result = (result + aLow * bHigh % M + bLow * aHigh % M) % M;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % M;

        result = (result + aLow * bLow % M) % M;

        return result;
#endif
}

inline long long modPow(long long x, long long n, long long M) {
#ifdef __GNUC__
    if (n == 0)
        return 1;

    long long p = modPow(x, n / 2, M) % M;
    p = long long((__int128_t)p * p % M);

    return ((n & 1) == 0) ? p : long long((__int128_t)p * x % M);
#else
    if (n == 0)
        return 1;

    long long p = modPow(x, n / 2, M) % M;
    p = modMul(p, p, M);

    return ((n & 1) == 0) ? p : modMul(p, x, M);
#endif
}

// M is a prime number
inline long long modPowPrime(long long x, long long n, long long M) {
    return modPow(x, n % (M - 1), M);
}

// a and M are coprime.
inline long long modInv(long long a, long long M) {
    long long x, y;
    extGcd(a, M, x, y);
    //if (extGcd(a, M, x, y) > 1)
    //    return -1;
    return (x % M + M) % M;
}

// a and M are coprime
inline long long modInvIter(long long a, long long M) {
    long long b = M;
    long long y = 0, x = 1;

    if (M == 1)
        return 0;

    while (a > 1 && b != 0) {
        long long q = a / b;

        long long t = b;
        b = a % b;
        a = t;

        t = y;
        y = x - q * y;
        x = t;
    }

    return (x % M + M) % M;
}

// M is a prime number.
inline long long modInvPrime(long long a, long long M) {
    return modPow(a, M - 2, M);
}

// a and M are coprime
inline long long modDiv(long long a, long long b, long long M) {
#ifdef __GNUC__
    return long long((__int128_t)(a % M) * modInv(b, M) % M);
#else
    return modMul(a % M, modInv(b, M), M);
#endif
}

// a and M are coprime
inline long long modDivIter(long long a, long long b, long long M) {
#ifdef __GNUC__
    return long long((__int128_t)(a % M) * modInvIter(b, M) % M);
#else
    return modMul(a % M, modInvIter(b, M), M);
#endif
}

// M is a prime number.
inline long long modDivPrime(long long a, long long b, long long M) {
#ifdef __GNUC__
    return long long((__int128_t)(a % M) * modInvPrime(b, M) % M);
#else
    return modMul(a % M, modInvPrime(b, M), M);
#endif
}
