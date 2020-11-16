#pragma once

// Berlekamp-Massey
//  https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm
//  https://codeforces.com/blog/entry/61306
//  https://koosaga.com/231
//  https://brilliant.org/wiki/linear-recurrence-relations/

// Kitamasa
//  https://cubelover.tistory.com/21
//  https://casterian.net/archives/254

// Minimal polynomial
//  https://en.wikipedia.org/wiki/Minimal_polynomial_(linear_algebra)
// 

// Reference:
//  https://github.com/zimpha/algorithmic-library/blob/master/mathematics/linear-recurrence.cc
//  https://gist.github.com/koosaga/d4afc4434dbaa348d5bef0d60ac36aa4

/*
    | F(n+1) | = | c0 c1 c2 ... c(k-3) c(k-2) c(k-1) | | F(n)   |
    | F(n)   |   | 1  0  0  ...   0      0      0    | | F(n-1) |
    | F(n-1) |   | 0  1  0  ...   0      0      0    | | F(n-2) |
    | F(n-2) |   | 0  0  1  ...   0      0      0    | | F(n-3) |
    | ...    |   | ...                               | | ...    |
    | F(2)   |   | 0  0  0  ...   1      0      0    | | F(1)   |
    | F(1)   |   | 0  0  0  ...   0      1      0    | | F(0)   |
*/

// Linear recurrence relation : x(n) = c(0)*x(n - 1) + c(1)*x(n - 2) + ... + c(k - 1)*x(n - k), k < n
// - mod must be a prime number.
template <typename T, int mod = 1'000'000'007>
struct LinearRecurrence {
    vector<T> X;    // initial values
    vector<T> C;    // coefficients of linear recurrence relation

    LinearRecurrence() {
    }

    explicit LinearRecurrence(const vector<T>& x) {
        build(x);
    }

    // Berlekamp-Massey, O(N*K + N*logMOD), N = the number of initial Xs
    // - N(the size of x) >= 2*K
    void build(const vector<T>& x) {
        int N = int(x.size());

        this->X = x;
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
    }

    // Kitamasa, O(K^2*logN)
    T getNth(long long n) {
        if (n < int(X.size()))
            return X[n];

        int m = int(C.size());
        if (m == 0)
            return T(0);

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
            res += 1ll * s[i] * X[i] % mod;

        return T(res % mod);
    }

    static T guessNthTerm(const vector<T>& x, long long n) {
        if (n < (long long)x.size())
            return x[n];

        LinearRecurrence<T,mod> rec(x);
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
    static vector<T> getMinPoly(int N, const vector<tuple<T,T,T>>& A) {
        vector<T> rnd1, rnd2;
        mt19937 rng(0xCDEF);
        auto randint = [&rng](T lo, T hi) {
            return uniform_int_distribution<T>(lo, hi)(rng);
        };
        for (int i = 0; i < N; i++) {
            rnd1.push_back(randint(1, mod - 1));
            rnd2.push_back(randint(1, mod - 1));
        }

        vector<T> gobs;
        for (int i = 0; i < 2 * N + 2; i++) {
            long long tmp = 0;
            for (int j = 0; j < N; j++) {
                tmp += 1ll * rnd2[j] * rnd1[j] % mod;
                if (tmp >= mod)
                    tmp -= mod;
            }
            gobs.push_back(T(tmp));
            vector<T> nxt(N);
            for (auto& i : A) {
                nxt[get<0>(i)] += 1ll * get<2>(i) * rnd1[get<1>(i)] % mod;
                if (nxt[get<0>(i)] >= mod)
                    nxt[get<0>(i)] -= mod;
            }
            rnd1 = nxt;
        }

        LinearRecurrence<T,mod> rec(gobs);
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
    static T det(int N, vector<tuple<T,T,T>> A) {
        vector<T> rnd;
        mt19937 rng(0xCDEF);
        auto randint = [&rng](T lo, T hi) {
            return uniform_int_distribution<int>(lo, hi)(rng);
        };

        for (int i = 0; i < N; i++)
            rnd.push_back(randint(1, mod - 1));

        for (auto& i : A)
            get<2>(i) = T(1ll * get<2>(i) * rnd[get<1>(i)] % mod);

        auto sol = getMinPoly(N, A)[0];

        if ((N & 1) == 0)
            sol = mod - sol;

        for (auto& i : rnd)
            sol = T(1ll * sol * modInv(i) % mod);

        return T(sol);
    }

private:
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
