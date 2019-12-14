#pragma once

// f(x) = x * (x - 1) * (x - 2) * ... * 2 * 1
inline long long countTrailingZeroesOfFactorial(long long x) {
    long long res = 0;

    while (x) {
        x /= 5;
        res += x;
    }
    return res;
}

// f(L, R) = R * (R - 1) * (R - 2) * ... * (L + 1) * L
inline long long countTrailingZeroesOfFactorial(long long L, long long R) {
    long long res = 0;

    long long xL2 = max(0ll, L - 1);
    long long xR2 = R;
    long long xL5 = xL2;
    long long xR5 = xR2;
    while (xL2 < xR2 && xL5 < xR5) {
        xL2 /= 2; xR2 /= 2;
        xL5 /= 5; xR5 /= 5;
        res += min(xR2 - xL2, xR5 - xL5);
    }
    return res;
}
