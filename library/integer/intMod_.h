#pragma once

#include "gcd.h"

//--- int version -------------------------------

inline int modAdd(int a, int b, int mod) {
    return ((a % mod) + (b % mod)) % mod;
}

inline int modSub(int a, int b, int mod) {
    return ((a % mod) - (b % mod) + mod) % mod;
}

inline int modMul(int a, int b, int mod) {
    return int((long long)(a % mod) * (b % mod) % mod);
}

#if 0
// recursive version
inline int modPow(int x, int n, int mod) {
    if (n == 0)
        return 1;

    int p = modPow(x, n / 2, mod) % mod;
    p = int((long long)p * p % mod);

    return ((n & 1) == 0) ? p : int((long long)p * x % mod);
}
#else
// iterative version
inline int modPow(int x, int n, int mod) {
    if (n == 0)
        return 1;

    long long t = x % mod;
    long long res = 1;
    for (; n > 0; n >>= 1) {
        if (n & 1)
            res = res * t % mod;
        t = t * t % mod;
    }
    return int(res);
}
#endif

// mod is a prime number
inline int modPowPrime(int x, long long n, int mod) {
    return modPow(x, int(n % (mod - 1)), mod);
}

// a and mod are coprime.
inline int modInv(int a, int mod) {
    int x, y;
    extGcd(a, mod, x, y);
    //if (extGcd(a, mod, x, y) > 1)
    //    return -1;
    return (x % mod + mod) % mod;
}

// a and mod are coprime
inline int modInvIter(int a, int mod) {
    int b = mod;
    int y = 0, x = 1;

    if (mod == 1)
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

    return (x % mod + mod) % mod;
}

// mod is a prime number.
inline int modInvPrime(int a, int mod) {
    return modPow(a, mod - 2, mod);
}

// a and mod are coprime
inline int modDiv(int a, int b, int mod) {
    return int((long long)(a % mod) * modInv(b, mod) % mod);
}

// a and mod are coprime
inline int modDivIter(int a, int b, int mod) {
    return int((long long)(a % mod) * modInvIter(b, mod) % mod);
}

// mod is a prime number.
inline int modDivPrime(int a, int b, int mod) {
    return int((long long)(a % mod) * modInvPrime(b, mod) % mod);
}


//http://codeforces.com/blog/entry/13908#comment-188854
// mod is a prime number and n < mod
inline vector<int> modInvPrimeRange(int n, int mod) {
    vector<int> res(n + 1);
    res[0] = 0;
    res[1] = 1;
    for (int i = 2; i <= n; i++) {
        res[i] = int((mod - 1ll * (mod / i) * res[mod % i] % mod) % mod);
    }

    return res;
}


//--- long long version -------------------------

inline long long modAdd(long long a, long long b, long long mod) {
    return ((a % mod) + (b % mod)) % mod;
}

inline long long modSub(long long a, long long b, long long mod) {
    return ((a % mod) - (b % mod) + mod) % mod;
}


inline long long modMul(long long a, long long b, long long mod) {
#ifdef __GNUC__
    return (long long)((__int128_t)(a % mod) * (b % mod) % mod);
#else
    int base = int(1e9);
    long long aLow = a % base, aHigh = a / base;
    long long bLow = b % base, bHigh = b / base;

    long long result = (aHigh * bHigh) % mod;
    for (int i = 0; i < 9; i++)
        result = (result * 10) % mod;

    result = (result + aLow * bHigh % mod + bLow * aHigh % mod) % mod;
    for (int i = 0; i < 9; i++)
        result = (result * 10) % mod;

    result = (result + aLow * bLow % mod) % mod;

    return result;
#endif
}

inline long long modMul2(long long a, long long b, long long mod) {
#ifdef __GNUC__
    return (long long)((__int128_t)(a % mod) * (b % mod) % mod);
#else
    if (b == 0)
        return 0;

    long long res = modMul2(a, b / 2, mod);
    res = (res + res) % mod;

    return ((b & 1) == 0) ? res : (res + a) % mod;
#endif
}

#if 0
// recursive version
inline long long modPow(long long x, long long n, long long mod) {
#ifdef __GNUC__
    if (n == 0)
        return 1;

    long long p = modPow(x, n / 2, mod) % mod;
    p = (long long)((__int128_t)p * p % mod);

    return ((n & 1) == 0) ? p : (long long)((__int128_t)p * x % mod);
#else
    if (n == 0)
        return 1;

    long long p = modPow(x, n / 2, mod) % mod;
    p = modMul(p, p, mod);

    return ((n & 1) == 0) ? p : modMul(p, x, mod);
#endif
}
#else
// iterative version
inline long long modPow(long long x, long long n, long long mod) {
    if (n == 0)
        return 1ll;

    long long res = 1ll;
#ifdef __GNUC__
    long long t = x % mod;
    for (; n > 0; n >>= 1) {
        if (n & 1)
            res = (long long)((__int128_t)t * res % mod);
        t = (long long)((__int128_t)t * t % mod);
    }
#else
    long long t = x % mod;
    for (; n > 0; n >>= 1) {
        if (n & 1)
            res = modMul(res, t, mod);
        t = modMul(t, t, mod);
    }
#endif
    return res;
}
#endif

// mod is a prime number
inline long long modPowPrime(long long x, long long n, long long mod) {
    return modPow(x, n % (mod - 1), mod);
}

// a and mod are coprime.
inline long long modInv(long long a, long long mod) {
    long long x, y;
    extGcd(a, mod, x, y);
    //if (extGcd(a, mod, x, y) > 1)
    //    return -1;
    return (x % mod + mod) % mod;
}

// a and mod are coprime
inline long long modInvIter(long long a, long long mod) {
    long long b = mod;
    long long y = 0, x = 1;

    if (mod == 1)
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

    return (x % mod + mod) % mod;
}

// mod is a prime number.
inline long long modInvPrime(long long a, long long mod) {
    return modPow(a, mod - 2, mod);
}

// a and mod are coprime
inline long long modDiv(long long a, long long b, long long mod) {
#ifdef __GNUC__
    return (long long)((__int128_t)(a % mod) * modInv(b, mod) % mod);
#else
    return modMul(a % mod, modInv(b, mod), mod);
#endif
}

// a and mod are coprime
inline long long modDivIter(long long a, long long b, long long mod) {
#ifdef __GNUC__
    return (long long)((__int128_t)(a % mod) * modInvIter(b, mod) % mod);
#else
    return modMul(a % mod, modInvIter(b, mod), mod);
#endif
}

// mod is a prime number.
inline long long modDivPrime(long long a, long long b, long long mod) {
#ifdef __GNUC__
    return (long long)((__int128_t)(a % mod) * modInvPrime(b, mod) % mod);
#else
    return modMul(a % mod, modInvPrime(b, mod), mod);
#endif
}
