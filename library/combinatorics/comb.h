#pragma once

// Naive calculation
// PRECONDITION: n <= 61
inline long long comb(int n, int r) {
    if (n < r)
        return 0;
    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1;
    for (int i = 1; i <= r; i++)
        q = q * n-- / i;

    return q;
}
