#pragma once

struct SumOfPowerMod {
    static const int INF = (int)2e9;
    static const int MOD = 1000000009;
    static const int MAXK = 1001;

    int K;

    vector<vector<long long>> Comb; // nCr
    vector<long long> B;            // bernoulli

    SumOfPowerMod(int k)
        : K(k), Comb(k + 2, vector<long long>(k + 2)), B(k + 2) {
    }

    void makeTable() {
        makeCombMod();
        for (int i = 0; i <= MAXK; i++) {
            if (i & 1)
                B[i] = 0;
            else
                B[i] = -INF;
        }
        B[0] = 1;
        B[1] = -modInverse(2) + MOD;
    }

    // faulhaberMode(n, k) = 1^k + 2^k + 3^k + ... + n^k
    int faulhaberMod(long long n, int k) {
        long long ans = 0;

        for (int i = 0; i <= k; i++) {
            if (i & 1)
                ans = (ans - ((Comb[k + 1][i] * bernoulliMod(i) % MOD) * powMod(n, k + 1 - i) % MOD) + MOD) % MOD;
            else
                ans = (ans + ((Comb[k + 1][i] * bernoulliMod(i) % MOD) * powMod(n, k + 1 - i) % MOD)) % MOD;
        }
        ans = ans * modInverse(k + 1) % MOD;

        return (int)ans;
    }

private:
    long long bernoulliMod(int n) {
        if (B[n] == -INF) {
            long long sum = (1 + Comb[n + 1][1] * B[1]) % MOD;
            for (int i = 2; i < n; i += 2)
                sum = (sum + (Comb[n + 1][i] * bernoulliMod(i) % MOD)) % MOD;
            B[n] = sum * modInverse(n + 1) % MOD;
            B[n] *= -1;
            while (B[n] < 0)
                B[n] += MOD;
        }
        return B[n];
    }

    void makeCombMod() {
        for (int i = 0; i <= K; i++)
            Comb[i][0] = 1;

        for (int i = 1; i <= MAXK; i++)
            Comb[0][i] = 0;

        for (int i = 1; i <= MAXK; i++) {
            for (int j = 1; j <= i; j++)
                Comb[i][j] = (Comb[i - 1][j - 1] + Comb[i - 1][j]) % MOD;
        }
    }

    static long long powMod(long long x, int n) {
        if (n == 0)
            return 1;
        else if (n == 1)
            return x % MOD;

        long long p = powMod(x, n / 2) % MOD;
        p = p * p % MOD;

        return (n & 1) == 0 ? p : p * (x % MOD) % MOD;
    }

    static long long modInverse(int a) {
        return powMod(a, MOD - 2);
    }
};