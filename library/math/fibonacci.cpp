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

#include <vector>
#include <algorithm>
#include "matrix.h"

using namespace std;

// fibonacci(n) = | F(n + 1)   F(n)   |
//                |   F(n)   F(n - 1) |

// n >= 0
template <typename T>
Matrix2x2<T> fibonacci(int n) {
    static Matrix2x2<T> fm{ 1ll, 1ll, 1ll, 0ll };
    return Matrix2x2<T>::pow(fm, n);
}

// n >= 0
template <typename T, int mod>
Matrix2x2Mod<T, mod> fibonacciMod(int n) {
    static Matrix2x2Mod<T, mod> fm{ 1ll, 1ll, 1ll, 0ll };
    return Matrix2x2Mod<T, mod>::pow(fm, n);
}



/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define MOD     1000000007

long long fibonacciSlow(int n) {
    if (n == 0)
        return 0ll;
    else if (n == 1)
        return 1ll;
    else if (n == 2)
        return 1ll;

    long long a = 1ll, b = 1ll;
    for (int i = 3; i <= n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }

    return b;
}

void testFibonacci() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Fibonacci ------------------------------" << endl;

    for (int i = 0; i <= 10000; i++) {
        assert(fibonacci<long long>(i).a01 == fibonacciSlow(i));
    }

    PROFILE_START(0);
    for (int i = 0; i <= 10000; i++) {
        auto t = fibonacci<long long>(i);
    }
    PROFILE_STOP(0);

    PROFILE_START(1);
    for (int i = 0; i <= 10000; i++)
        auto t = fibonacciSlow(i);
    PROFILE_STOP(1);

    cout << "OK!" << endl;
}
