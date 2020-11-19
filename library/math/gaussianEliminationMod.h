#pragma once

#include "matrixMod.h"

// SLAE = System of n linear algebraic equations
// - A*x = b
template <int mod = 1'000'000'007>
struct SLAEMod {
    // O(N^3), 'a' is a N x N matrix
    static vector<int> gauss(MatrixMod<mod> A, vector<int> b) {
        int N = A.N;
        for (int row = 0, col = 0; col < N && row < N; col++) {
            int best = -1;
            for (int i = row; i < N; i++) {
                if (A.mat[i][col]) {
                    best = i;
                    break;
                }
            }
            if (best == -1)
                continue;
            if (row != best) {
                swap(A.mat[row], A.mat[best]);
                swap(b[row], b[best]);
            }

            auto xInv = modPow(A[row][col], mod - 2);
            for (int i = col; i < N; i++)
                A[row][i] = int(1ll * A[row][i] * xInv % mod);
            b[row] = int(1ll * b[row] * xInv % mod);

            for (int i = 0; i < N; i++) {
                auto x = A[i][col] ? (mod - A[i][col]) : 0;
                if (i != row && x != 0) {
                    for (int j = col; j < N; j++)
                        A[i][j] = int((A[i][j] + 1ll * A[row][j] * x) % mod);
                    b[i] = int((b[i] + 1ll * b[row] * x) % mod);
                }
            }

            row++;
        }

        return b;
    }

private:
    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }
};
