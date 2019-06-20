#pragma once

/*
    Y = f(X) = a_n * X^n + a_(n-1) * X^(n - 1) + ... + a_0
        Y0 = f(X0),
        Y1 = f(X1),
         . . .
        Yn = f(Xn)

            n
    f(X) = SUM { Yj * PRODUCT (X - Xm) / (Xj - Xm) }
           j=0        0<=m<=n
                        m!=j
*/
struct LagrangePolynomialMod {
    // O(n^2 + n*logMOD)
    // samples = { ..., (Xi, Yi), ... }
    static int interpolate(const pair<int, int> samples[], int n, long long x, int mod) {
        long long res = 0;

        x %= mod;
        for (int i = 0; i < n; i++) { 
            long long termQ = samples[i].second;
            long long termP = 1;
            for (int j = 0; j < n; j++) { 
                if (j != i) {
                    termQ = termQ * (x - samples[j].first + mod) % mod;
                    termP = termP * (samples[i].first - samples[j].first + mod) % mod;
                }
            } 
            res = (res + termQ * modInv(termP, mod) % mod) % mod;
        } 

        return int(res);
    }

    // O(n^2)
    // samples = { ..., (Xi, Yi), ... }
    static int interpolate(const vector<pair<int, int>>& samples, long long x, int mod) {
        return interpolate(samples.data(), int(samples.size()), x, mod);
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

/*
    TODO:
        1) implement modular Lagrange polynomial

        2) implement solution of below formula

             n
            SUM { a_i * i^k }
            i=1

           (see sumOfPowerFast.h)
*/
// https://www.codechef.com/JUNE19A/problems/COUNTIT
// https://discuss.codechef.com/t/countit-editorial/27153
