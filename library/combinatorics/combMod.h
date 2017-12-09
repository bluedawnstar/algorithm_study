#pragma once

#include "../integer/intMod.h"

//--- Naive method ------------------------------------------------------------

inline int combModNaive(int n, int r, int M) {
    if (n < r)
        return 0;

    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1, d = 1;
    for (int i = 1; i <= r; i++) {
        q = q * n-- % M;
        d = d * i % M;
    }

    return int(q * modInvIter((int)d, M) % M);
}

inline int combModPrimeNaive(int n, int r, int M) {
    if (n < r)
        return 0;

    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1, d = 1;
    for (int i = 1; i <= r; i++) {
        q = q * n-- % M;
        d = d * i % M;
    }

    return int(q * modInvPrime((int)d, M) % M);
}

// Lucas' Theorem (http://bowbowbow.tistory.com/2)
template <typename T, typename U>
inline long long combLucasModNaive(T n, U r, int M) {
    if (n == 0 || r == 0 || n == r)
        return 1;

    return combLucasModNaive(n / M, r / M, M) * combModPrimeNaive(int(n % M), int(r % M), M) % M;
}

//--- Calculation with multiplicative inverse ---------------------------------

inline long long combMod(int n, int r, int M) {
    if (n < r)
        return 0;
    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1;
    for (int i = 1; i <= r; i++) {
        q = (q * n-- % M) * modInvIter(i, M) % M;
    }

    return q;
}

inline long long combModPrime(int n, int r, int M) {
    if (n < r)
        return 0;
    if (n == 0 || r == 0 || n == r)
        return 1;

    if (n - r < r)
        r = n - r;

    long long q = 1;
    for (int i = 1; i <= r; i++) {
        q = (q * n-- % M) * modInvPrime(i, M) % M;
    }

    return q;
}

template <typename T, typename U>
long long combLucasMod(T n, U r, int M) {
    if (n < r)
        return 0;
    if (n == 0 || r == 0 || n == r)
        return 1;

    return combLucasMod(n / M, r / M, M) * combModPrime(int(n % M), int(r % M), M) % M;
}

//--- Calculation with DP -----------------------------------------------------

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
