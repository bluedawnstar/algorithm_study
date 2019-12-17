#pragma once

#include "lagrangeModFast.h"

/*

  1. properties
    f(d, x) = (dx + 1) * (dx + 2) * ... * (dx + d)
    
    - f(d, 0) = d!

    - f(a, 0) * f(a, 1) * ... * f(a, b - 1) = f(ab, 0) = (ab)!
  
    - f(d, 2x) * f(d, 2x + 1) = f(2d, x)
    
  2. how to calculate N! (mod p)
    
    v = sqrt(N)

    N! (mod p) = (v * (v + 1))! * (v * (v + 1) + 1) * (v * (v + 1) + 2) * ... * N

               = f(v * (v + 1), 0)! * (v * (v + 1) + 1) * (v * (v + 1) + 2) * ... * N
               = { f(v, 0) * f(v, 1) * ... * f(v, v) } * (v * (v + 1) + 1) * (v * (v + 1) + 2) * ... * N

*/
template <int mod = 1000000007>
struct FastFactorialIntMod {
    FastLagrangePolynomialMod<mod> lagrange;

    FastFactorialIntMod() {
    }

    // N > sqrt(maxDegree) * 4
    explicit FastFactorialIntMod(int N) { 
        lagrange.build(N);
    }

    // N > sqrt(maxDegree) * 4
    void build(int N) {
        lagrange.build(N);
    }

    //---

    // O(sqrt(N) * logN)
    int factorial(int N) {
        if (N >= mod)
            return 0;
        else if (N <= lagrange.maxDegree)
            return lagrange.factorial[N];

        long long deg = 1;
        vector<int> factPart = { 1, 2 };
        while (N > deg * (deg + 1)) {
            factPart = squarepoly(factPart);
            deg *= 2;
        }

        long long res = 1;
        long long bucket = N / deg;

        for (int i = 0; i < bucket; i++)
            res = res * factPart[i] % mod;

        for (long long i = bucket * deg + 1; i <= N; i++)
            res = res * i % mod;

        return int(res);
    }

private:
    vector<int> squarepoly(const vector<int>& poly) {
        // y(0), y(1), y(2), ..., y(4 * d + 1)
        vector<int> ss = lagrange.interpolateRange0(poly, (int(poly.size()) - 1) * 4 + 1);
        vector<int> res(ss.size() / 2);
        for (int i = 0; i < (int)ss.size() / 2; ++i)
            res[i] = int(1ll * ss[2 * i] * ss[2 * i + 1] % mod);
        return res;
    }
};
