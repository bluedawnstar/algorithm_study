#pragma once

#include "ntt_fast.h"
#include "polyFFTMod3.h"
#include "lagrangeModFast.h"

/*
  1. problem
      F(x) = (x - C[0])(x - C[1])...(x - C[n-1])

      G(k) = F(0) + F(1) + F(2) + ... + F(k)

           = a[n+1] * x^(n+1) + a[n] * x^(n) + ... + a[1] * x + a[0]

      calculate G(k)

  2. how to solve
    1) build polynomial F(x) = (x - C[0])(x - C[1])...(x - C[n-1])

    2) calculate Y = { F(0), F(1), F(2), ..., F(n), F(n + 1) }

    3) interpolate G(m)
       X = { 0, 1, 2, 3, ..., n, n + 1 }
       Y = { G(0), G(1), G(2), ..., G(n), G(n + 1) }
         = { F(0), F(0) + F(1), F(0) + F(1) + F(2), ..., F(0) + F(1) + ... + F(n), F(0) + F(1) + ... + F(n) + F(n + 1) }
*/

template <int mod = 998'244'353, int root = 3>
struct PolynomialSumNTT {
    FastLagrangePolynomialMod<mod> lagrange;

    explicit PolynomialSumNTT(int maxN) : lagrange(maxN) {
    }

    // return G(k) = SUM F(i), 0 <= i <= k
    // O(N*logN)
    int solve(const vector<int>& C, long long k) {
        int N = int(C.size());

        // step #1 : F(x) = (x - C[0])(x - C[1])...(x - C[n-1])
        FastNTT<mod,root>::EvaluationTree F;
        F.build(C);

        // step #2-1 : build eval polynomial
        vector<int> X(N + 2);
        iota(X.begin(), X.end(), 0);
        FastNTT<mod,root>::EvaluationTree evalTree;
        evalTree.build(X);

        // step #2-2 : multipoint evaluation
        auto Y = evalTree.evaluate(F.tree[1], X);

        // step #3 : evaluate G(m)
        vector<int> GY(N + 2);
        GY[0] = Y[0];
        if (GY[0] >= mod)
            GY[0] -= mod;
        for (int i = 1; i < N + 2; i++) {
            GY[i] = GY[i - 1] + Y[i];
            if (GY[i] >= mod)
                GY[i] -= mod;
        }

        return lagrange.interpolate0(GY, k);
    }

    int solve(const int A[], int N, long long k) {
        return solve(vector<int>(A, A + N), k);
    }
};

template <int mod = 1'000'000'007>
struct PolynomialSumFFT {
    FastLagrangePolynomialMod<mod> lagrange;

    explicit PolynomialSumFFT(int maxN) : lagrange(maxN) {
    }

    // return G(k) = SUM F(i), 0 <= i <= k
    // O(N*logN)
    int solve(const vector<int>& C, long long k) {
        int N = int(C.size());

        // step #1 : F(x) = (x - C[0])(x - C[1])...(x - C[n-1])
        PolyFFTMod3<mod>::EvaluationTree F;
        F.build(C);

        // step #2-1 : build eval polynomial
        vector<int> X(N + 2);
        iota(X.begin(), X.end(), 0);
        PolyFFTMod3<mod>::EvaluationTree evalTree;
        evalTree.build(X);

        // step #2-2 : multipoint evaluation
        auto Y = evalTree.evaluate(F.tree[1], X);

        // step #3 : evaluate G(m)
        vector<int> GY(N + 2);
        GY[0] = Y[0];
        if (GY[0] >= mod)
            GY[0] -= mod;
        for (int i = 1; i < N + 2; i++) {
            GY[i] = GY[i - 1] + Y[i];
            if (GY[i] >= mod)
                GY[i] -= mod;
        }

        return lagrange.interpolate0(GY, k);
    }

    int solve(const int A[], int N, long long k) {
        return solve(vector<int>(A, A + N), k);
    }
};
