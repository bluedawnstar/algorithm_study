#pragma once

#include "intMod.h"
#include "gcd.h"

// finds all solutions to ax = b (mod M)
template <typename T, typename U>
inline vector<T> solveModularEq(T a, T b, U M) {
    vector<T> res;

    T x, y;
    T g = extGcd(a, M, x, y);
    if (!(b % g)) {
        x = (x * (b / g) % M + M) % M;
        for (int i = 0; i < g; i++)
            res.push_back((x + i * (M / g) % M + M) % M);
    }
    sort(res.begin(), res.end());

    return res;
}

// find (x, y) satisfying ax + by = c (diophantine equation)
// return (x, y, whether the solution exists)
template <typename T>
inline bool solveDiophantineEq(T a, T b, T c, T& x, T& y) {
    if (!a && !b) {
        if (c)
            return false;
        x = 0;
        y = 0;
        return true;
    }

    if (!a) {
        if (c % b)
            return false;
        x = 0;
        y = c / b;
        return true;
    }

    if (!b) {
        if (c % a)
            return false;
        x = c / a;
        y = 0;
        return true;
    }

    T g = gcd(a, b);
    if (c % g)
        return false;

    x = c / g * modInvIter(a / g, b / g);
    y = (c - a * x) / b;
    
    return true;
}
