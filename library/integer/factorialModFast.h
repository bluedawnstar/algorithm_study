#pragma once

// n!_{%P}  <--- It's not n! % P
struct FastFactorialMod {
    // https://acmcairoscience.wordpress.com/2015/04/06/factorial-calculation-modulo/
    // n!_{%P}, O(P*log_P(n))
    template <typename T>
    static int factorial(T n, int P) {
        long long res = 1;
        while (n > 1) {
            if ((n / P) & 1)
                res = P - res;
            for (int i = 2; i <= n % P; ++i)
                res = res * i % P;
            n /= P;
        }
        if (res >= P)
            res -= P;
        return int(res);
    }

    // https://cp-algorithms.com/algebra/factorial-modulo.html
    // n!_{%P}, O(P + log_P(n))
    template <typename T>
    static int factorialFast(T n, int P) {
        vector<int> f(P);
        f[0] = 1;
        for (int i = 1; i < P; i++)
            f[i] = int(1ll * f[i - 1] * i % P);

        long long res = 1;
        while (n > 1) {
            if ((n / P) & 1)
                res = P - res;
            res = res * f[n % P] % P;
            n /= P;
        }
        return int(res);
    }
};

// n!_{%P}  <--- It's not n! % P
struct FastFactorialMod2 {
    int mod;
    int blockSize;
    vector<int> blockF;  // blockF[i] = (i * blockSize)!

    explicit FastFactorialMod2(int mod, int blockSize = 0) {
        prepare(mod, blockSize);
    }

    void prepare(int mod, int blockSize = 0) {
        if (blockSize <= 0)
            blockSize = int(sqrt(mod));

        this->mod = mod;
        this->blockSize = blockSize;

        blockF.clear();
        blockF.reserve(mod / blockSize + 1);
        
        long long f = 1;
        for (int i = 0, j = 1; i < mod; i += blockSize) {
            for (; j <= i; j++)
                f = f * j % mod;
            blockF.push_back(int(f));
        }
    }

    // O(sqrt(P))
    int factorialSmall(int n) {
        int i = n / blockSize;
        long long res = blockF[i];

        for (i = i * blockSize + 1; i <= n; i++)
            res = res * i % mod;

        return int(res);
    }

    // https://cp-algorithms.com/algebra/factorial-modulo.html
    // n!_{%P}, O(sqrt(P) * log_P(n))
    template <typename T>
    int factorial(T n) {
        if (n < mod)
            return factorialSmall(n);

        long long res = 1;
        while (n > 1) {
            if ((n / mod) & 1)
                res = mod - res;
            res = res * factorialSmall(int(n % mod)) % mod;
            n /= mod;
        }
        return int(res);
    }
};

// https://www.geeksforgeeks.org/compute-n-under-modulo-p/
// n! % p, O(n*loglogn)
struct FactorialModWithSieve {
    // n! % mod, O((loglogn)^2)
    static int factorial(const int primes[], int primesN, int n, int mod) {
        if (n >= mod)
            return 0;

        long long res = 1;
        for (int i = 0; i < primesN && primes[i] <= n; i++) {
            int k = largestPower(n, primes[i]);
            res = res * modPow(primes[i], k, mod) % mod;
        }
        return int(res);
    }

    static int factorial(const vector<int>& primes, int n, int mod) {
        return factorial(primes.data(), int(primes.size()), n, mod);
    }

    //---

    // https://en.wikipedia.org/wiki/Legendre%27s_formula
    // returns largest power of a prime p that divides n!, O(log_p(n))
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
