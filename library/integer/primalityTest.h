#pragma once

template <typename T>
struct PrimalityTest {
    static const T RAND_SEED = 3;

    static bool isPrimeNumber(T x) {
        return doMiller(x, 3) && doFermat(x, 3) && doPollardRho(x, 3).first;
    }

    //--- Pollard Rho
    static pair<bool,T> doPollardRho(T x, int k) {
        if (x <= 1)
            return make_pair(false, x);
        if (x <= 3)
            return make_pair(true, x);
        if (x == 4)
            return make_pair(false, 2);

        T rndX = RAND_SEED; // seed
        T rndY = RAND_SEED; // seed

        int t = min(int(sizeof(x)) * 8, int(sqrt(x))) * k;

        int i = 0, j = 2;
        while (j <= t) {
            i++;
            rndX = nextRand(rndX, x);
            T d = gcd(abs(rndY - rndX), x);
            if (d != 1 && d != x)
                return make_pair(false, d);

            if (i == j) {
                rndY = rndX;
                j <<= 1;
            }
        }

        return make_pair(true, x);
    }

    //--- Fermat's little theorem
    // If n is prime, then always returns true,
    // If n is composite then returns false with high probability
    // k=try count, O(KlogX)
    static bool doFermat(T x, int k) {
        if (x <= 1 || x == 4)
            return false;
        if (x <= 3)
            return true;

        T rnd = RAND_SEED;  // seed

        // try k times
        while (k-- > 0) {
            T a = 2 + T((rnd = nextRand(rnd, x)) % (x - 4));

            // Fermat's little theorem
            if (power(a, x - 1, x) != 1)
                return false;
        }

        return true;
    }

    //--- Miller-Rabin method
    // It returns false if n is composite and returns true if n is probably prime.
    static bool doMiller(T x, int k) {
        if (x <= 1 || x == 4)
            return false;
        if (x <= 3)
            return true;

        // find r such that n = 2^d * r + 1 for some r >= 1
        T d = x - 1;
        while ((d & 1) == 0)
            d >>= 1;

        T rnd = RAND_SEED;  // seed

        while (k-- > 0) {
            if (millerTest(d, x, (rnd = nextRand(rnd, x))) == false)
                return false;
        }

        return true;
    }

private:
    static T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }

    static T mulMod(T a, T b, T M) {
        T x = 0, y = a % M;
        while (b > 0) {
            if (b & 1)
                x = (x + y) % M;

            y = (y << 1) % M;
            b >>= 1;
        }
        return x % M;
    }

    static T nextRand(T x, T M) {
        return (mulMod(x, x, M) + M - 1) % M;
    }

    static T power(T a, T x, T p) {
        T res = 1;

        a = a % p;
        while (x > 0) {
            if (x & 1)
                res = T(mulMod(res, a, p));

            x >>= 1;
            a = T(mulMod(a, a, p));
        }

        return res;
    }

    // It returns false if n is composite and returns false if n is probably prime.
    // d is an odd number such that  d*2^r = n-1 for some r >= 1
    static bool millerTest(T d, T x, T rnd) {
        T a = 2 + T(rnd % (x - 4));

        // compute a^d % x
        T t = power(a, d, x);

        if (t == 1 || t == x - 1)
            return true;

        while (d != x - 1) {
            t = mulMod(t, t, x);
            d <<= 1;

            if (t == 1)
                return false;
            if (t == x - 1)
                return true;
        }

        return false;
    }
};
