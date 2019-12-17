#pragma once

#include "intMod.h"

// Returns value of n! % P
inline int factorialModNaive(int n, int P) {
    if (n >= P)
        return 0;

    int result = 1;
    for (int i = 1; i <= n; i++)
        result = int((long long)result * i % P);

    return result;
}

/*
<Wilson's Theorem>
        (p - 1)! กี -1 (mod p)    (p > 1, p is a prime number)
    OR  (p - 1)! กี (p - 1) (mod p)
*/

// returns n! % M using Wilson's Theorem
// It is efficient when n is close to M

// O((p - n)*logn)
// PRECONDITION: M is a prime number
inline int factorialModWilson(int n, int M) {
    // n! % M is 0 if n >= M
    if (n >= M)
        return 0;

    // Initialize result as (M - 1)! which is -1 or (M - 1)
    int res = (M - 1);
    for (int i = n + 1; i < M; i++)
        res = int((long long)res * modInvPrime(i, M) % M);

    return res;
}

inline long long factorialModWilson(long long n, long long M) {
    // n! % M is 0 if n >= M
    if (n >= M)
        return 0;

    // Initialize result as (M - 1)! which is -1 or (M - 1)
    long long res = (M - 1);
    for (long long i = n + 1; i < M; i++)
        res = res * modInvPrime(i, M) % M;

    return res;
}
