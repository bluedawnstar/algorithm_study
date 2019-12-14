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
inline Matrix2x2<T> fibonacci(T n) {
    static Matrix2x2<T> fm{ 1ll, 1ll, 1ll, 0ll };
    return Matrix2x2<T>::pow(fm, n);
}

// n >= 0
template <typename T, int mod>
inline Matrix2x2Mod<T, mod> fibonacciMod(T n) {
    static Matrix2x2Mod<T, mod> fm{ 1ll, 1ll, 1ll, 0ll };
    return Matrix2x2Mod<T, mod>::pow(fm, n);
}
