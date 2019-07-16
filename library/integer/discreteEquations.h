#pragma once

#include "intMod.h"

// Linear Congruence Equation
// finds all solutions to ax = b (mod mod)
template <typename T>
struct LinearCongruence {
    static vector<T> solve(T a, T b, T mod) {
        vector<T> res;

        T x, y;
        T g = extGcd(a, mod, x, y);
        if (!(b % g)) {
            x = (x * (b / g) % mod + mod) % mod;
            for (T i = 0; i < g; i++)
                res.push_back((x + i * (mod / g) % mod + mod) % mod);
        }
        sort(res.begin(), res.end());

        return res;
    }

private:
    // a * x + b * y = gcd(a, b)
    static T extGcd(T a, T b, T& x, T& y) {
        if (b == 0) {
            x = T(1);
            y = T(0);
            return a;
        }

        T x1, y1;
        T g = extGcd(b, a % b, x1, y1);

        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }
};
