#pragma once

template <int mod = 1000000007>
struct FastModOp {
    int N;
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    FastModOp() {
    }

    explicit FastModOp(int maxN) {
        build(maxN);
    }

    // O(N)
    void build(int maxN) {
        N = max(1, maxN);

        factorial.resize(maxN + 1);
        factInverse.resize(maxN + 1);
        inverse.resize(maxN + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxN; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxN; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            factInverse[i] = int(1ll * factInverse[i - 1] * inverse[i] % mod);
        }
    }

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
    static int modPow(long long x, int n) {
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
    static int modInv(long long a) {
        return modPow(a, mod - 2);
    }
};
