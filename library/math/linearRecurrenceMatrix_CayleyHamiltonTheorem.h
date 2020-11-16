#pragma once

#include "matrixMod.h"

/*
 <Cayley–Hamilton theorem>
  A = N x N matrix

  1. Determinant and inverse matrix
   - https://en.wikipedia.org/wiki/Cayley%E2%80%93Hamilton_theorem#Determinant_and_inverse_matrix
   
    - A^-1 can be written as an (n-1)th order polynomial expression in A

    - p(A) = A^n + c[n-1] * A^(n-1) + ... + c[1] * A + (-1)^n * det(A) * I = 0
   
  2. power of matrix
    - If A is a square matrix (N x N), then A^k satisfies a linear recurrence of order N

    - A^k = c[0] * A^(k-n) + c[1] * A^(k-n+1) + c[2] * A^(k-n+2) + ... + c[n-2] * A^(k-2) + c[n-1] * A^(k-1)

*/

// Linear recurrence relation
// - A^k = c[0] * A^(k-n) + c[1] * A^(k-n+1) + c[2] * A^(k-n+2) + ... + c[n-2] * A^(k-2) + c[n-1] * A^(k-1)
// - mod must be a prime number.

template <typename T, int mod = 1'000'000'007>
struct MatrixLinearRecurrence {
    vector<MatrixMod<mod>> X;   // initial values
    vector<T> C;                // coefficients of linear recurrence relation

    MatrixLinearRecurrence() {
    }

    MatrixLinearRecurrence(const vector<MatrixMod<mod>>& x, const MatrixMod<mod>& base) {
        build(x, base);
    }

    // Berlekamp-Massey, O(n*K + n*logMOD), N = the number of initial Xs
    // - N(the size of x) >= 2*k
    void build(const vector<MatrixMod<mod>>& x, const MatrixMod<mod>& base) {
        this->X = x;
        do {
            C = matrixRecurrence(base);
        } while (!verify(base, C));
    }

    // Kitamasa, O(K^2*logN)
    MatrixMod<mod> getNth(long long n) {
        if (n < int(X.size()))
            return X[n];

        int m = int(C.size());
        if (m == 0)
            return MatrixMod<mod>(X[0].N);

        vector<T> s(m), t(m);
        s[0] = 1;
        if (m != 1)
            t[1] = 1;
        else
            t[0] = C[0];

        for (; n; n >>= 1) {
            if (n & 1)
                s = mult(s, t);
            t = mult(t, t);
        }

        MatrixMod<mod> res(X[0].N);
        for (int i = 0; i < m; i++)
            res += X[i] * s[i];

        return res;
    }

private:
    vector<T> doBerlekampMassey(const vector<T>& x) {
        int N = int(x.size());

        C = vector<T>(N);

        int size = 0, m = 0;
        vector<T> B(N);

        C[0] = B[0] = 1;
        T b = 1;
        for (int i = 0; i < N; i++) {
            ++m;

            T d = x[i];
            for (int j = 1; j <= size; j++) {
                d += T(1ll * C[j] * x[i - j] % mod);
                if (d >= mod)
                    d -= mod;
            }
            if (d == 0)
                continue;

            auto t = C;
            T coef = T(1ll * d * modInv(b) % mod);
            for (int j = m; j < N; j++) {
                C[j] -= T(1ll * coef * B[j - m] % mod);
                if (C[j] < 0)
                    C[j] += mod;
            }
            if (2 * size > i)
                continue;

            size = i + 1 - size;
            B = t;
            b = d;
            m = 0;
        }
        C.resize(size + 1);
        for (int i = 1; i <= size; i++) {
            if (C[i] > 0)
                C[i - 1] = mod - C[i];
            else
                C[i - 1] = C[i];
        }
        C.pop_back();

        return C;
    }

    vector<T> matrixRecurrence(const MatrixMod<mod>& base) {
        static mt19937 rng(0xCDEF);

        int N = base.N;

        vector<T> rnd1, rnd2;
        for (int i = 0; i < N; i++) {
            rnd1.push_back(rng() % (mod - 1) + 1);
            rnd2.push_back(rng() % (mod - 1) + 1);
        }

        vector<T> v;
        for (int k = 0; k < int(X.size()); k++) {
            vector<T> sum(N);
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    sum[i] += T(1ll * X[k].mat[i][j] * rnd2[j] % mod);
                    if (sum[i] >= mod)
                        sum[i] -= mod;
                }
            }
            T tmp = 0;
            for (int j = 0; j < N; j++) {
                tmp += T(1ll * sum[j] * rnd1[j] % mod);
                if (tmp >= mod)
                    tmp -= mod;
            }
            v.push_back(tmp);
        }

        return doBerlekampMassey(v);
    }

    bool verify(const MatrixMod<mod>& base, const vector<T>& C) {
        int N = base.N;

        int k = int(X.size());
        int sz = int(C.size());

        MatrixMod<mod> ans(N);
        for (int i = 0; i < sz; i++) {
            ans += X[k - i - 2] * C[i];
        }
        if (ans.mat != X[k - 1].mat)
            return false;

        return true;
    }

    //---

    static T modPow(T x, T n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return T(res);
    }

    static T modInv(T x) {
        return modPow(x, mod - 2);
    }

    vector<T> mult(const vector<T>& v, const vector<T>& w) {
        int m = int(v.size());
        vector<T> t(2 * m);

        //TODO: optimize with FFT for big K
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < m; k++) {
                t[j + k] += T(1ll * v[j] * w[k] % mod);
                if (t[j + k] >= mod)
                    t[j + k] -= mod;
            }
        }
        for (int j = 2 * m - 1; j >= m; j--) {
            for (int k = 1; k <= m; k++) {
                t[j - k] += T(1ll * t[j] * C[k - 1] % mod);
                if (t[j - k] >= mod)
                    t[j - k] -= mod;
            }
        }
        t.resize(m);
        return t;
    }
};
