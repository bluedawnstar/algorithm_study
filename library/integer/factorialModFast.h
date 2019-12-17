#pragma once

// https://acmcairoscience.wordpress.com/2015/04/06/factorial-calculation-modulo/
// n! % P, O(P*log_P(n))
inline int factorialMod(int n, int P) {
    long long res = 1;
    while (n > 1) {
        res = (res * ((n / P) % 2 ? P - 1 : 1)) % P;
        for (int i = 2; i <= n % P; ++i)
            res = (res * i) % P;
        n /= P;
    }
    return int(res % P);
}

// https://www.geeksforgeeks.org/compute-n-under-modulo-p/
// n! % p, O(n*loglogn)
struct FactorialModWithSieve {
    // n! % mod, O(n*loglogn)
    static int factorial(int n, int mod) {
        if (n >= mod)
            return 0;

        // find all primes smaller than n
        vector<bool> isPrime(n + 1, true);
        for (int i = 2; i * i <= n; i++) {
            if (isPrime[i]) {
                for (int j = 2 * i; j <= n; j += i)
                    isPrime[j] = false;
            }
        }

        int res = 1;
        for (int i = 2; i <= n; i++) {
            if (isPrime[i]) {
                int k = largestPower(n, i);
                res = int(1ll * res * modPow(i, k, mod) % mod);
            }
        }
        return res;
    }

private:
    // returns largest power of p that divides n!
    static int largestPower(int n, int p) {
        int res = 0;

        // res = n/p + n/(p^2) + n/(p^3) + ....
        while (n) {
            n /= p;
            res += n;
        }
        return res;
    }

    static int modPow(int x, int n, int p) {
        if (n == 0)
            return 1;

        long long t = x % p;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % p;
            t = t * t % p;
        }
        return int(res);
    }
};
