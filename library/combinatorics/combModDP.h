#pragma once

#include "../integer/intMod.h"

//--- Calculation with DP -----------------------------------------------------

// nCr % M
struct CombAllModDP {
    vector<vector<long long>> C;

    CombAllModDP() {
    }

    CombAllModDP(int n, int M) {
        build(n, M);
    }

    void build(int n, int M) {
        C.assign(n + 1, vector<long long>(n + 1));

        C[0][0] = 1;

        for (int i = 1; i <= n; i++) {
            C[i][0] = 1;
            for (int j = 1; j < i; j++)
                C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % M;
            C[i][i] = 1;
        }
    }

    long long comb(int n, int r) {
        return C[n][r];
    }
};

//---

// nCr % M  (n is fixed)
struct CombCompactModDP {
    vector<long long> C;

    CombCompactModDP() {
    }

    CombCompactModDP(int n, int M) {
        build(n, M);
    }

    void build(int n, int M) {
        C.assign(n + 1, 0);

        C[0] = 1;

        for (int i = 1; i <= n; i++) {
            C[i] = 1;
            for (int j = i - 1; j > 0; j--)
                C[j] = (C[j] + C[j - 1]) % M;
        }
    }

    long long comb(int r) {
        return C[r];
    }
};

// nCr % M  (n is fixed)
struct FastCombCompactModDP {
    vector<long long> C;

    FastCombCompactModDP() {
    }

    FastCombCompactModDP(int n, int M) {
        build(n, M);
    }

    void build(int n, int M) {
        C.assign(n + 1, 0);

        C[0] = 1;

        if (n > 0) {
            C[1] = n % M;

            vector<int> inv(n + 1);
            inv[1] = 1;
            for (int i = 2; i <= n; i++) {
                inv[i] = int((M - 1ll * (M / i) * inv[M % i] % M) % M);
                C[i] = C[i - 1] * (n - i + 1) % M * inv[i] % M;
            }
        }
    }

    long long comb(int r) {
        return C[r];
    }
};

//---

// nCr % M
// M is a prime number
struct CombLucasModDP {
    int M;
    vector<vector<long long>> C;

    CombLucasModDP() {
    }

    CombLucasModDP(int M) {
        build(M);
    }

    void build(int M) {
        this->M = M;
        C.assign(M + 1, vector<long long>(M + 1));

        C[0][0] = 1;

        for (int i = 1; i <= M; i++) {
            C[i][0] = 1;
            for (int j = 1; j < i; j++)
                C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % M;
            C[i][i] = 1;
        }
    }

    template <typename T, typename U>
    long long comb(T n, U r) {
        if (n < r)
            return 0;
        if (n == 0 || r == 0 || n == r)
            return 1;

        return comb(n / M, r / M) * C[int(n % M)][int(r % M)] % M;
    }
};
