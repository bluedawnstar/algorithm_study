#pragma once

// https://www.codechef.com/JUNE19A/problems/COUNTIT
// https://discuss.codechef.com/t/countit-editorial/27153

/*
       k
      SUM c_i * i^n = a_(n+1) * k^(n+1) + a_n * k^n + ... + a_1 * k + a_0
      i=1

       => a polynomial with degree n+1 in k
          (https://math.stackexchange.com/questions/18983/why-is-sum-limits-k-1n-km-a-polynomial-with-degree-m1-in-n)
*/
struct PowerSumPolyMod {
    int maxK;
    int mod;                    // prime number
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    PowerSumPolyMod() {
    }

    PowerSumPolyMod(int maxK, int mod) { 
        build(maxK, mod);
    }

    // O(maxK)
    void build(int maxK, int mod) {
        maxK += 2;
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
    }

    /*
                n
        f(n) = SUM { c_i * i^k } = a_(k+1) * n^(k+1) + a_k * n^k + ... + a_1 * n + a_0
               i=1
           => a polynomial with degree k+1 in n

        Y = { f(1), f(2), ..., f(k + 2) }
    */
    // O(k*logMOD), solve with Lagrange polynomial with degree k + 1 in n
    int solve(const vector<int>& Y, long long n, int k) {
        if (n <= k + 2)
            return int(Y[n - 1]);

        long long q = 1;
        for (int i = 1; i <= k + 2; i++)
            q = 1ll * q * (n - i) % mod;    // q = (n - 1)(n - 2)...(n - k - 2)

        long long ans = 0;
        for (int i = 1; i <= k + 2; i++) {
            auto prefix = factInverse[i - 1];
            auto suffix = factInverse[k + 2 - i];
            if ((k + 2 - i) & 1)
                suffix = mod - suffix;
            // ans += q * Y[i - 1] / { (n - i) * (i - 1)! * (k + 2 - i)! * (-1)^(k + 2 - i) }
            ans = (ans + q * modInv(n - i, mod) % mod * prefix % mod * suffix % mod * Y[i - 1] % mod) % mod;
        }

        return int(ans);
    }

private:
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
