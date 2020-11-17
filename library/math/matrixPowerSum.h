#pragma once

#include "matrixMod.h"

/*
     n
    SUM A^(p*i) , n >= 0, A^0 = I
    i=0
*/
template <int mod = 1'000'000'007>
struct MatrixPowerSumMod {
    int K;                          // A is K x K matrix
    vector<MatrixMod<mod>> baseDP;  // A^p
    vector<MatrixMod<mod>> sumDP;   // 

    // O(K^3 * logMAXN)
    void prepare(const MatrixMod<mod>& A, long long p, long long maxN) {
        K = A.N;

        int logN = 1;
        while ((1ll << logN) <= maxN)
            logN++;

        MatrixMod<mod> base(K), curr(K);

        base = MatrixMod<mod>::pow(A, p);
        curr = base;

        baseDP.resize(logN);
        sumDP.resize(logN);

        baseDP[0] = curr;
        sumDP[0] = curr;

        for (int i = 1; i < logN; i++) {
            auto next = curr * curr;
            baseDP[i] = next;
            sumDP[i] = sumDP[i - 1] + sumDP[i - 1] * curr;
            swap(next.mat, curr.mat);
        }
    }

    // O(K^3 * logN)
    MatrixMod<mod> powerSum(long long N) {
        MatrixMod<mod> res(K);
        if (N < 0)
            return res;

        res.identity();

        MatrixMod<mod> scale(K);
        scale.identity();
        for (int i = 0; N; i++, N >>= 1) {
            if (N & 1) {
                res += sumDP[i] * scale;
                scale *= baseDP[i];
            }
        }

        return res;
    }

    static MatrixMod<mod> powerSum(const MatrixMod<mod>& A, long long N) {
        int K = A.N;

        MatrixMod<mod> res(K);
        if (N < 0)
            return res;
        res.identity();

        MatrixMod<mod> scale(K);
        scale.identity();

        MatrixMod<mod> sum(A), curr(A);
        for (int i = 0; N; i++, N >>= 1) {
            if (N & 1) {
                res += sum * scale;
                scale *= curr;
            }
            sum = sum + sum * curr;
            curr = curr * curr;
        }

        return res;
    }
};
