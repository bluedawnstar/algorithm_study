#pragma once

#include "matrix2x2.h"
#include "matrix2x2Mod.h"

/*
<Important properties>

F(n + 2) = F(n) + F(n + 1)     (F(0) = 0, F(1) = 1, F(2) = 1, ...)
F(n - 2) = F(n) - F(n - 1)
F(-n) = (-1)^(n+1) * F(n)

| F(k + 2) | = | 1  1 | | F(k + 1) |
| F(k + 1) |   | 1  0 | |   F(k)   |

A^n = | 1  1 |n = | F(n + 1)    F(n)   |
      | 1  0 |    |   F(n)    F(n - 1) |

| F(m + n + 1)  F(m + n)   | = A^(m + n) = | F(m + 1)  F(m)   | | F(n + 1)  F(n)   |
|   F(m + n)  F(m + n - 1) |               |   F(m)  F(m - 1) | |   F(n)  F(n - 1) |

F(m + n - 1) = F(m) * F(n) + F(m - 1) * F(n - 1)
F(m + n) = F(m) * F(n + 1) + F(m - 1) * F(n)
*/

// fibonacci(n) = | F(n + 1)   F(n)   |
//                |   F(n)   F(n - 1) |

// n >= 0
template <typename T>
inline T fibonacci(long long n) {
    static Matrix2x2<T> fm{ 1, 1, 1, 0 };
    return Matrix2x2<T>::pow(fm, n).a01;
}

// n >= 0
template <typename T, int mod>
inline T fibonacciMod(long long n) {
    static Matrix2x2Mod<T, mod> fm{ 1, 1, 1, 0 };
    return Matrix2x2Mod<T, mod>::pow(fm, n).a01;
}

// O(logN)
template <typename T, int mod>
inline T fibonacciModFast(long long n) {
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;
    else if (n == 2)
        return 1;

    int a = 0;
    int b = 1;

    long long i = 1ll;
    while ((i << 1) <= n)
        i <<= 1;

    for (i >>= 1; i; i >>= 1) {
        int na = (1ll * a * a + 1ll * b * b) % mod;
        int nb = int((2ll * a + b) * b % mod);
        a = na;
        b = nb;

        if (n & i) {
            int c = a + b;
            if (c >= mod)
                c -= mod;
            a = b;
            b = c;
        }
    }

    return b;
}
