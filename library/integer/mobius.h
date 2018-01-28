#pragma once

#include "primeFactor.h"
#include "primeNumberEratosthenes.h"

///////////////////////////////////////////////////////////////////////////////
// Mobius Inversion Formula
/*
    g(n) =  sum{f(d)}
             d|n
    f(n) =  sum{g(d) * mu(n/d)}
             d|n

    d is n's factors, including 1
*/

///////////////////////////////////////////////////////////////////////////////
// Mobius Function

/*
1. Mobius Function
  mu(n) = 1        , n = 1
          0        , n > 1, n has a squared prime factor
          (-1)^k   , n > 1, n is the product of k distinct primes

2. Properties
  1) mu(ab) = mu(a) * mu(b)    , gcd(a,b) = 1

  2) sum{mu(d)} = 1            , n = 1
      d|n

     sum{mu(d)} = 0            , n > 1
      d|n
*/

// PRECONDITION: n >= 1
// return mu(n), O(sqrt(N))
inline int mobius(int n) {
    if (n == 1)
        return 1;

    int k = 0;
    if (n % 2 == 0) {
        n /= 2;
        k++;

        if (n % 2 == 0)
            return 0;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (n % i == 0) {
            n /= i;
            k++;

            if (n % i == 0)
                return 0;
        }
    }

    if (n > 1)
        k++;

    return (k & 1) ? -1 : 1;
}

// PRECONDITION: n >= 1
// mobius function in [0, n], inclusive, O(N)
inline vector<int> mobiusSeive(int n, const MinFactors& minFactors) {
    vector<int> res(n + 1);
    if (n > 0)
        res[1] = 1;

    for (int i = 2; i <= n; i++) {
        int w = minFactors.minFactors[i];
        res[i] = (minFactors.minFactors[i / w] == w) ? 0 : -res[i / w];
    }

    return res;
}

// PRECONDITION: n >= 1
// mobius function in [0, n], inclusive, O(N loglogN)
inline vector<int> mobiusSeive(int n) {
    MinFactors factors;
    factors.build(n);
    return mobiusSeive(n, factors);
}

// PRECONDITION: 1 <= left <= right
// mobius function in [0, n], inclusive
inline vector<int> mobiusSeive(int left, int right) {
    int root = (int)sqrt(right);
    vector<bool> primes = eratosthenes(root);

    vector<int> mu(right - left + 1, 1);
    vector<int> m(right - left + 1, 1);

    if (right >= 4) {
        int delta = 2 * 2;
        for (int j = ((left + delta - 1) / delta) * delta; j >= 0 && j <= right; j += delta)
            mu[j - left] = 0;
        for (int j = ((left + 2 - 1) / 2) * 2; j >= 0 && j <= right; j += 2) {
            mu[j - left] = -mu[j - left];
            m[j - left] = m[j - left] * 2;
        }
    }

    for (int i = 3; i <= root; i += 2) {
        if (!primes[i])
            continue;
        int delta = i * i;
        for (int j = ((left + delta - 1) / delta) * delta; j >= 0 && j <= right; j += delta)
            mu[j - left] = 0;
        for (int j = ((left + i - 1) / i) * i; j >= 0 && j <= right; j += i) {
            mu[j - left] = -mu[j - left];
            m[j - left] = m[j - left] * i;
        }
    }

    for (int i = left; i <= right; i++) {
        if (m[i - left] < i)
            mu[i - left] = -mu[i - left];
    }

    return mu;
}


///////////////////////////////////////////////////////////////////////////////
// Square Free Numbers

// sqaure free numbers in [0, n], inclusive
inline vector<bool> squareFree(int n) {
    vector<bool> res(n + 1, true);
    res[0] = false;

    if (n >= 4) {
        for (int j = 2 * 2; j <= n; j += 2 * 2)
            res[j] = false;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (res[i]) {
            int delta = i * i;
            for (int j = delta; j >= 0 && j <= n; j += delta)
                res[j] = false;
        }
    }

    return res;
}

// PRECONDITION: 'mu' must have the results of mobius function in [0, sqrt(n)] at least
// the number of sqaure free numbers in [0, n], inclusive
inline int countSquareFree(int n, const vector<int>& mu) {
    int res = 0;

    int root = (int)sqrt(n);
    for (int i = 1; i <= root; i++)
        res += mu[i] * (n / (i * i));

    return res;
}

// <the number of square free numbers>
//  [1, 10] : 7
//  [1, 100] : 61
//  [1, 1000] : 608
//  [1, 10000] : 6083
//  [1, 100000] : 60794
//  [1, 1000000] : 607926
//  [1, 10000000] : 6079291
//  [1, 100000000] : 60792694
//  [1, 1000000000] : 607927124
//  [1, 2000000000] : 1215854216
// the number of sqaure free numbers in [0, n], inclusive
inline int countSquareFree(int n) {
    int res = 0;

    int root = (int)sqrt(n);
    vector<int> mu = mobiusSeive(root);
    for (int i = 1; i <= root; i++)
        res += mu[i] * (n / (i * i));

    return res;
}

// PRECONDITION: 'mu' must have the results of mobius function in [0, sqrt(n)] at least
// 0 <= nth
inline int nthSquareFree(int nth, const vector<int>& mu) {
    nth++;

    int lo = 1;
    int hi = INT_MAX;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        int cnt = countSquareFree(mid, mu);
        if (cnt >= nth)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}

// 0 <= nth
inline int nthSquareFree(int nth) {
    nth++;

    int lo = 1;
    int hi = INT_MAX;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        int cnt = countSquareFree(mid);
        if (cnt >= nth)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}
