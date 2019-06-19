#pragma once

/*
<Important Properties>

1) f(p - 1, k) = 1^k + 2^k + 3^k + ... + (p - 1)^k (mod p)
- f(p - 1, 0) = p - 1 = -1 (mod p)
- f(p - 1, k) = 0 (mod p), 0 < k < p - 1

2) primitive root
{ 1, 2, 3, ..., p - 1 } = { g^0, g^1, g^2, ..., g^(p - 2) }, g is a primitive root of prime number p
*/

struct FastSumOfPowerMod {
    static const int INF = 0x7f7f7f7f; //0x3f3f3f3f;

    int maxK;
    int mod;                    // prime number
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    vector<long long> B;        // bernoulli

    FastSumOfPowerMod() {
    }

    FastSumOfPowerMod(int maxK, int mod) {
        build(maxK, mod);
    }

    // O(N)
    void build(int maxK, int mod) {
        maxK++;
        this->maxK = max(1, maxK);
        this->mod = mod;

        factorial.resize(maxK + 1);
        factInverse.resize(maxK + 1);
        inverse.resize(maxK + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxK; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxK; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            factInverse[i] = int(1ll * factInverse[i - 1] * inverse[i] % mod);
        }

        // bernoulli number

        B.resize(maxK + 1);
        for (int i = 0; i <= maxK; i++) {
            if (i & 1)
                B[i] = 0;
            else
                B[i] = -INF;
        }
        B[0] = 1;
        B[1] = mod - inverse[2];
    }


    // faulhaberMode(n, k) = 1^k + 2^k + 3^k + ... + n^k
    int faulhaberMod(long long n, int k) {
        long long ans = modPow(n, k + 1, mod);
        if (k >= 1)
            ans = (ans - comb(k + 1, 1) * bernoulliMod(1) % mod * modPow(n, k, mod) % mod + mod) % mod;
        for (int i = 2; i <= k; i += 2)
            ans = (ans + comb(k + 1, i) * bernoulliMod(i) % mod * modPow(n, k + 1 - i, mod) % mod) % mod;

        ans = ans * inverse[k + 1] % mod;

        return int(ans);
    }

private:
    long long bernoulliMod(int n) {
        if (B[n] == -INF) {
            long long sum = (1 + comb(n + 1, 1) * B[1]) % mod;
            for (int i = 2; i < n; i += 2)
                sum = (sum + (comb(n + 1, i) * bernoulliMod(i) % mod)) % mod;
            B[n] = sum * inverse[n + 1] % mod;
            B[n] *= -1;
            while (B[n] < 0)
                B[n] += mod;
        }
        return B[n];
    }

    //---

    // n!, O(1)
    int fact(int n) {
        return factorial[n];
    }

    // 1 / n!, O(1)
    int factInv(int n) {
        return factInverse[n];
    }

    // nPr, O(1)
    int perm(int n, int r) {
        return int(1ll * factorial[n] * factInverse[n - r] % mod);
    }

    // 1 / nPr, O(1)
    int permInv(int n, int r) {
        return int(1ll * factInverse[n] * factorial[n - r] % mod);
    }

    // nCr, O(1)
    int comb(int n, int r) {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * factorial[n] * factInverse[n - r] % mod * factInverse[r] % mod);
    }

    // nCr, O(1)
    int combInv(int n, int r) {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * factInverse[n] * factorial[n - r] % mod * factorial[r] % mod);
    }

    //---

    // O(logn), mod must be a prime number
    static int modPow(long long x, int n, int mod) {
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

    // O(logn), mod must be a prime number
    static int modInv(long long a, int mod) {
        return modPow(a, mod - 2, mod);
    }
};
