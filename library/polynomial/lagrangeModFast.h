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

    Y = { y(1), y(2), ..., y(k + 1) }
    X = { 1, 2, ..., k + 1 }
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

    // O(n*logMOD)
    // Y = { y(1), y(2), ..., y(n + 1) }
    // X = { 1, 2, ..., n + 1 }, x > 0
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
    // Y = { y(1), y(2), ..., y(n + 1) }
    // X = { 1, 2, ..., n + 1 }
    int interpolate(const vector<int>& Y, long long x) {
        return interpolate(Y.data(), int(Y.size()) - 1, x);
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
