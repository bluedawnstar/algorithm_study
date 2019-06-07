#pragma once

// Berlekamp-Massey
// https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
// https://codeforces.com/blog/entry/61306
// https://koosaga.com/231
// https://brilliant.org/wiki/linear-recurrence-relations/

// Kitamasa
// https://cubelover.tistory.com/21
// https://casterian.net/archives/254

// Minimal polynomial
// https://en.wikipedia.org/wiki/Minimal_polynomial_(linear_algebra)
// 

// Reference:
//   https://github.com/zimpha/algorithmic-library/blob/master/mathematics/linear-recurrence.cc
//   https://gist.github.com/koosaga/d4afc4434dbaa348d5bef0d60ac36aa4

// Linear recurrence relation : x(n) = c(0)*x(n - 1) + c(1)*x(n - 2) + ... + c(k - 1)*x(n - k), k < n
// - MOD must be a prime number.
template <typename T>
struct LinearRecurrence {
    T MOD;
    vector<T> X;    // initial values
    vector<T> C;    // coefficients of linear recurrence relation

    explicit LinearRecurrence(T mod) : MOD(mod) {
    }

    LinearRecurrence(T mod, const vector<T>& x) : MOD(mod) {
        build(x);
    }

    // Berlekamp-Massey, O(n*K + n*logMOD), n = the number of initial Xs
    // The size of x must be more than 2*k, x(1) ... x(2k)
    void build(const vector<T>& x) {
        auto update = [this](vector<T>& A, const vector<T>& B, int m, T b, T d) {
            if (B.size() + m > A.size())
                A.resize(B.size() + m);
            long long coef = 1ll * d * modPow(b, MOD - 2, MOD) % MOD;
            for (int j = 0; j < int(B.size()); j++) {
                A[j + m] -= T(1ll * coef * B[j] % MOD);
                if (A[j + m] < 0)
                    A[j + m] += MOD;
            }
        };

        vector<T> A{ 1 }, B{ 1 };
        T b = x[0];
        for (int i = 1, m = 1; i < int(x.size()); i++, m++) {
            long long d = 0;
            for (int j = 0; j < int(A.size()); j++)
                d = (d + 1ll * A[j] * x[i - j]) % MOD;

            if (d == 0)
                continue;

            if (2 * (int(A.size()) - 1) <= i) {
                auto temp = A;
                update(A, B, m, b, T(d));
                B = temp;
                b = T(d);
                m = 0;
            } else {
                update(A, B, m, b, T(d));
            }
        }

        //---

        if (A.empty())
            A.push_back(0);
        int m = int(A.size()) - 1;
        C.resize(m);
        for (int i = 0; i < m; i++)
            C[i] = (MOD - A[i + 1]) % MOD;
        this->X.assign(x.begin(), x.begin() + m);
    }

    // Kitamasa, O(K^2*logN)
    T getNth(long long n) {
        if (n < int(X.size()))
            return X[n];

        int m = int(C.size());

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

        long long res = 0;
        for (int i = 0; i < m; i++)
            res += 1ll * s[i] * X[i] % MOD;

        return T(res % MOD);
    }

    static T guessNthTerm(const vector<T>& x, T mod, long long n) {
        if (n < x.size())
            return x[n];

        LinearRecurrence<T> rec(mod, x);
        return rec.getNth(n);
    }

    // Minimal polynomial of matrix A
    // 
    // <input>
    //   A stands for a N x N sparse matrix.
    //     A[i] = (x, y, val), it means A(x, y) = val
    //
    // <output>
    //   Smallest poly P such that
    //     P = SUM { C(i) * A^i } = 0
    //        i=0..k
    //
    // O(N*size(A))
    static vector<T> getMinPoly(int N, const vector<tuple<T,T,T>>& A, T MOD) {
        vector<T> rnd1, rnd2;
        mt19937 rng(0xCDEF);
        auto randint = [&rng](T lo, T hi) {
            return uniform_int_distribution<T>(lo, hi)(rng);
        };
        for (int i = 0; i < N; i++) {
            rnd1.push_back(randint(1, MOD - 1));
            rnd2.push_back(randint(1, MOD - 1));
        }

        vector<T> gobs;
        for (int i = 0; i < 2 * N + 2; i++) {
            long long tmp = 0;
            for (int j = 0; j < N; j++) {
                tmp += 1ll * rnd2[j] * rnd1[j] % MOD;
                if (tmp >= MOD)
                    tmp -= MOD;
            }
            gobs.push_back(T(tmp));
            vector<T> nxt(N);
            for (auto& i : A) {
                nxt[get<0>(i)] += 1ll * get<2>(i) * rnd1[get<1>(i)] % MOD;
                if (nxt[get<0>(i)] >= MOD)
                    nxt[get<0>(i)] -= MOD;
            }
            rnd1 = nxt;
        }

        LinearRecurrence<T> rec(MOD, gobs);
        reverse(rec.C.begin(), rec.C.end());    // reverse!
        return rec.C;
    }

    // Determinant of matrix A
    // 
    // <input>
    //   A stands for a N x N sparse matrix.
    //     A[i] = (x, y, val), it means A(x, y) = val
    //
    // O(N*size(A))
    static T det(int N, vector<tuple<T,T,T>> A, T MOD) {
        vector<T> rnd;
        mt19937 rng(0xCDEF);
        auto randint = [&rng](T lo, T hi) {
            return uniform_int_distribution<int>(lo, hi)(rng);
        };

        for (int i = 0; i < N; i++)
            rnd.push_back(randint(1, MOD - 1));

        for (auto& i : A)
            get<2>(i) = T(1ll * get<2>(i) * rnd[get<1>(i)] % MOD);

        auto sol = getMinPoly(N, A, MOD)[0];

        if ((N & 1) == 0)
            sol = MOD - sol;

        for (auto& i : rnd)
            sol = T(1ll * sol * modPow(i, MOD - 2, MOD) % MOD);

        return T(sol);
    }

private:
    static T modPow(T x, T n, T MOD) {
        if (n == 0)
            return 1;

        long long t = x % MOD;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % MOD;
            t = t * t % MOD;
        }
        return T(res);
    }

    vector<T> mult(const vector<T>& v, const vector<T>& w) {
        int m = int(v.size());
        vector<T> t(2 * m);

        //TODO: optimize with FFT for big K
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < m; k++) {
                t[j + k] += T(1ll * v[j] * w[k] % MOD);
                if (t[j + k] >= MOD)
                    t[j + k] -= MOD;
            }
        }
        for (int j = 2 * m - 1; j >= m; j--) {
            for (int k = 1; k <= m; k++) {
                t[j - k] += T(1ll * t[j] * C[k - 1] % MOD);
                if (t[j - k] >= MOD)
                    t[j - k] -= MOD;
            }
        }
        t.resize(m);
        return t;
    }
};
