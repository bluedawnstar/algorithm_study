#pragma once

#include "polyFFTMod.h"

/*
1. Basic formula

    Y = f(X) = a_n * X^n + a_(n-1) * X^(n - 1) + ... + a_0
        Y0 = f(X0),
        Y1 = f(X1),
         . . .
        Yn = f(Xn)

            n
    f(X) = SUM { Yj * PRODUCT (X - Xm) / (Xj - Xm) }
           j=0        0<=m<=n
                        m!=j

2. Y = { y(1), y(2), ..., y(d + 1) }
   X = { 1, 2, ..., d + 1 }

            d+1          d+1
     y(x) = SUM { y(i) * PROD { (x - j) / (i - j) } }
            i=1          j=1
                         i!=j

              d+1              d+1
          = { PROD (x - j) } * SUM { y(i) / { (i - 1)! * (d + 1 - i)! * (-1)^(d + 1 - i)) * 1/(x - i) }
              j=1              i=1

      ==>
                                                                           d+1
     f(i) = y(i) / { (i - 1)! * (d + 1 - i)! * (-1)^(d + 1 - i) },  g(x) = PROD (x - j)
                                                                           j=1
                   d+1
     y(x) = g(x) * SUM { f(i) * 1/(x - i) }     <- convolution
                   i=1

3. Y = { y(0), y(1), y(2), ..., y(d) }
   X = { 0, 1, 2, ..., d }

             d            d
     y(x) = SUM { y(i) * PROD { (x - j) / (i - j) } }
            i=0          j=0
                         i!=j

               d                d
          = { PROD (x - j) } * SUM { y(i) / { i! * (d - i)! * (-1)^(d - i)) * 1/(x - i) }
              j=0              i=0

      ==>
                                                              d
     f(i) = y(i) / { i! * (d - i)! * (-1)^(d - i) },  g(x) = PROD (x - j)
                                                             j=0
                    d
     y(x) = g(x) * SUM { f(i) * 1/(x - i) }     <- convolution
                   i=0
*/
template <int mod>  // mod is a prime number
struct FastLagrangePolynomialMod {
    int maxDegree;
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    FastLagrangePolynomialMod() {
    }

    explicit FastLagrangePolynomialMod(int maxDegree) { 
        build(maxDegree);
    }

    // O(maxK)
    void build(int maxDegree) {
        maxDegree += 2;
        this->maxDegree = max(1, maxDegree);

        factorial.resize(maxDegree + 1);
        factInverse.resize(maxDegree + 1);
        inverse.resize(maxDegree + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxDegree; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxDegree; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            factInverse[i] = int(1ll * factInverse[i - 1] * inverse[i] % mod);
        }
    }

    //---

    // O(n*logMOD)
    // Y = { y(1), y(2), ..., y(degree + 1) }
    // X = { 1, 2, ..., degree + 1 }, x > 0
    int interpolate(const int Y[], int degree, long long x) {
        if (x <= degree + 1)
            return Y[int(x) - 1];

        long long q = 1;
        for (int i = 1; i <= degree + 1; i++)
            q = 1ll * q * (x - i) % mod;    // q = (x - 1)(x - 2)...(x - n - 1)

        x %= mod;

        long long ans = 0;
        for (int i = 1; i <= degree + 1; i++) {
            auto prefix = factInverse[i - 1];
            auto suffix = factInverse[degree + 1 - i];
            if ((degree + 1 - i) & 1)
                suffix = mod - suffix;
            // ans += q * Y[i - 1] / { (x - i) * (i - 1)! * (degree + 1 - i)! * (-1)^(degree + 1 - i) }
            ans = (ans + q * modInv(x - i) % mod * prefix % mod * suffix % mod * Y[i - 1] % mod) % mod;
        }

        return int(ans);
    }

    // O(n*logMOD)
    // Y = { y(1), y(2), ..., y(degree + 1) }
    // X = { 1, 2, ..., degree + 1 }
    int interpolate(const vector<int>& Y, long long x) {
        return interpolate(Y.data(), int(Y.size()) - 1, x);
    }

    /* O(n*logn)
        Y = { y(1), y(2), y(3), ..., y(degree + 1) }
        X = { 1, 2, 3, ..., degree + 1 }
        result = { y(1), y(2), y(3), ..., y(maxX) }, maxX >= degree + 1
    */
    vector<int> interpolateRange(const int Y[], int degree, int maxX) {
        vector<int> res(maxX);
        memcpy(res.data(), Y, sizeof(Y[0]) * min(degree + 1, maxX));
        if (maxX <= degree + 1)
            return res;

        vector<int> f(degree + 1);
        for (int i = 1; i <= degree + 1; i++) {
            auto prefix = factInverse[i - 1];
            auto suffix = factInverse[degree + 1 - i];
            if ((degree + 1 - i) & 1)
                suffix = mod - suffix;
            f[i - 1] = int(1ll * Y[i - 1] * prefix % mod * suffix % mod);
        }

        vector<int> conv = PolyFFTMod<mod>::multiply(f, vector<int>(inverse.begin() + 1, inverse.begin() + maxX + 1));

        int g = 1;
        for (int j = 1; j <= degree + 1; j++)
            g = int(1ll * g * (degree + 2 - j) % mod);

        res[degree + 1] = int(1ll * g * conv[degree] % mod);
        for (int i = degree + 3; i <= maxX; i++) {
            g = int(1ll * g * (i - 1) % mod * inverse[i - degree - 2] % mod);
            res[i - 1] = int(1ll * g * conv[i - 2] % mod);
        }

        return res;
    }

    /* O(n*logn)
        Y = { y(1), y(2), y(3), ..., y(degree + 1) }
        X = { 1, 2, 3, ..., degree + 1 }
        result = { y(1), y(2), y(3), ..., y(maxX) }, maxX >= degree + 1
    */
    vector<int> interpolateRange(const vector<int>& Y, int maxX) {
        return interpolateRange(Y.data(), int(Y.size()) - 1, maxX);
    }

    //---

    // O(n*logMOD)
    // Y = { y(0), y(1), y(2), ..., y(degree) }
    // X = { 0, 1, 2, ..., degree }, x > 0
    int interpolate0(const int Y[], int degree, long long x) {
        if (x <= degree)
            return Y[int(x)];

        long long q = 1;
        for (int i = 0; i <= degree; i++)
            q = 1ll * q * (x - i) % mod;    // q = x(x - 1)(x - 2)...(x - n)

        x %= mod;

        long long ans = 0;
        for (int i = 0; i <= degree; i++) {
            auto prefix = factInverse[i];
            auto suffix = factInverse[degree - i];
            if ((degree - i) & 1)
                suffix = mod - suffix;
            // ans += q * Y[i] / { (x - i) * i! * (degree - i)! * (-1)^(degree - i) }
            ans = (ans + q * modInv(x - i) % mod * prefix % mod * suffix % mod * Y[i] % mod) % mod;
        }

        return int(ans);
    }

    // O(n*logMOD)
    // Y = { y(0), y(1), y(2), ..., y(degree) }
    // X = { 0, 1, 2, ..., degree }
    int interpolate0(const vector<int>& Y, long long x) {
        return interpolate0(Y.data(), int(Y.size()) - 1, x);
    }

    /* multipoint evaluation - O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<int> interpolateRange0(const int Y[], int degree, int maxX) {
        vector<int> res(maxX + 1);
        memcpy(res.data(), Y, sizeof(Y[0]) * min(degree + 1, maxX + 1));
        if (maxX <= degree)
            return res;

        vector<int> f(degree + 1);
        for (int i = 0; i <= degree; i++) {
            auto prefix = factInverse[i];
            auto suffix = factInverse[degree - i];
            if ((degree - i) & 1)
                suffix = mod - suffix;
            f[i] = int(1ll * Y[i] * prefix % mod * suffix % mod);
        }

        vector<int> conv = PolyFFTMod<mod>::multiply(f, vector<int>(inverse.begin(), inverse.begin() + maxX + 1));

        int g = 1;
        for (int i = 0; i <= degree; i++)
            g = int(1ll * g * (degree + 1 - i) % mod);

        res[degree + 1] = int(1ll * g * conv[degree + 1] % mod);
        for (int i = degree + 2; i <= maxX; i++) {
            g = int(1ll * g * i % mod * inverse[i - degree - 1] % mod);
            res[i] = int(1ll * g * conv[i] % mod);
        }

        return res;
    }

    /* multipoint evaluation - O(n*logn)
        Y = { y(0), y(1), y(2), ..., y(degree) }
        X = { 0, 1, 2, ..., degree }
        result = { y(0), y(1), y(2), ..., y(maxX) }, maxX >= degree
    */
    vector<int> interpolateRange0(const vector<int>& Y, int maxX) {
        return interpolateRange0(Y.data(), int(Y.size()) - 1, maxX);
    }

private:
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
