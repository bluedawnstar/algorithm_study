#pragma once

/*
  <Important Properties>

    1) f(p - 1, k) = 1^k + 2^k + 3^k + ... + (p - 1)^k (mod p)
       - f(p - 1, 0) = p - 1 = -1 (mod p)
       - f(p - 1, k) = 0 (mod p), 0 < k < p - 1

    2) primitive root
       { 1, 2, 3, ..., p - 1 } = { g^0, g^1, g^2, ..., g^(p - 2) }, g is a primitive root of prime number p

    3)
       k
      SUM i^n = a_(n+1) * k^(n+1) + a_n * k^n + ... + a_1 * k + a_0
      i=1

       => a polynomial with degree n+1 in k
          (https://math.stackexchange.com/questions/18983/why-is-sum-limits-k-1n-km-a-polynomial-with-degree-m1-in-n)
*/
struct FastSumOfPowerMod {
    int maxK;
    int mod;                    // prime number
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    FastSumOfPowerMod() {
    }

    FastSumOfPowerMod(int maxK, int mod) {
        build(maxK, mod);
    }

    // O(N)
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


    // faulhaberMode(n, k) = 1^k + 2^k + 3^k + ... + n^k
    // O(k*logMOD), solve with Lagrange polynomial with degree n + 1 in k
    int faulhaberMod(long long n, int k) {
        // degree is n + 1

        vector<long long> Y(k + 3);
        Y[0] = 0;
        for (int i = 1; i <= k + 2; i++)
            Y[i] = (Y[i - 1] + modPow(i, k, mod)) % mod;    // Y[i] = SUM_(1..i) { i^k }

        if (n <= k + 2)
            return int(Y[n]);

        long long q = 1;
        for (int i = 1; i <= k + 2; i++)
            q = 1ll * q * (n - i) % mod;                    // q = (n - 1)(n - 2)...(n - k - 2)

        long long ans = 0;
        for (int i = 1; i <= k + 2; i++) {
            auto pre = factInverse[i - 1];
            auto suf = factInverse[k + 2 - i];
            if ((k + 2 - i) & 1)
                suf = mod - suf;
            // ans += q * Y[i] / { (n - i) * (i - 1)! * (k + 2 - i)! * (-1)^(k + 2 - i) }
            ans = (ans + q * modInv(n - i, mod) % mod * pre % mod * suf % mod * Y[i] % mod) % mod;
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
