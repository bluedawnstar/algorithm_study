#pragma once

/*
  <Important Properties>

    1) f(p - 1, k) = 1^k + 2^k + 3^k + ... + (p - 1)^k (mod p)
       - f(p - 1, 0) = p - 1 = -1 (mod p)
       - f(p - 1, k) = 0 (mod p), 0 < k < p - 1

    2) primitive root
       { 1, 2, 3, ..., p - 1 } = { g^0, g^1, g^2, ..., g^(p - 2) }, g is a primitive root of prime number p
*/

struct SumOfPowerMod {
    static const int INF = 0x7f7f7f7f; //0x3f3f3f3f;

    int maxK;
    int mod;

    vector<vector<long long>> comb; // nCr
    vector<long long> B;            // bernoulli

    SumOfPowerMod(int maxK, int mod)
        : maxK(maxK + 1), mod(mod), comb(maxK + 2, vector<long long>(maxK + 2)), B(maxK + 2) {
        makeTable();
    }

    // faulhaberMode(n, k) = 1^k + 2^k + 3^k + ... + n^k
    // O(k^2)
    int faulhaberMod(long long n, int k) {
        long long ans = powMod(n, k + 1, mod);
        if (k >= 1)
            ans = (ans - comb[k + 1][1] * bernoulliMod(1) % mod * powMod(n, k, mod) % mod + mod) % mod;
        for (int i = 2; i <= k; i += 2)
            ans = (ans + comb[k + 1][i] * bernoulliMod(i) % mod * powMod(n, k + 1 - i, mod) % mod) % mod;

        ans = ans * modInverse(k + 1, mod) % mod;

        return int(ans);
    }

private:
    void makeTable() {
        makeCombMod();
        for (int i = 0; i <= maxK; i++) {
            if (i & 1)
                B[i] = 0;
            else
                B[i] = -INF;
        }
        B[0] = 1;
        B[1] = mod - modInverse(2, mod);
    }

    //---

    long long bernoulliMod(int n) {
        if (B[n] == -INF) {
            long long sum = (1 + comb[n + 1][1] * B[1]) % mod;
            for (int i = 2; i < n; i += 2)
                sum = (sum + (comb[n + 1][i] * bernoulliMod(i) % mod)) % mod;
            B[n] = sum * modInverse(n + 1, mod) % mod;
            B[n] *= -1;
            while (B[n] < 0)
                B[n] += mod;
        }
        return B[n];
    }

    void makeCombMod() {
        for (int i = 0; i <= maxK; i++)
            comb[i][0] = 1;

        for (int i = 1; i <= maxK; i++)
            comb[0][i] = 0;

        for (int i = 1; i <= maxK; i++) {
            for (int j = 1; j <= i; j++)
                comb[i][j] = (comb[i - 1][j - 1] + comb[i - 1][j]) % mod;
        }
    }

    static long long powMod(long long x, int n, int mod) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return res;
    }

    static long long modInverse(int a, int mod) {
        return powMod(a, mod - 2, mod);
    }
};
