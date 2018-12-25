#pragma once

// 0 < x < 2^62 (~ 4.6 * 10^18)
template <typename T>
struct FastPrimalityTest {
    static bool isPrimeNumber(T x) {
        if (x <= 1)
            return false;
        if (x == 2)
            return true;
        if ((x & 1) == 0)
            return false;

        T d = x - 1;
        while ((d & 1) == 0)
            d /= 2;

        // "Fast Primality Testing for Integers That Fit into a Machine Word"
        int alist[]{ 2, 325, 9375, 28178, 450775, 9780504, 1795265022 };
        for (auto a : alist) {
            if (x <= a)
                break;

            T t = d;
            T y = power(static_cast<T>(a), t, x);
            while (t != x - 1 && y != 1 && y != x - 1) {
                y = mulMod(y, y, x);
                t <<= 1;
            }
            if (y != x - 1 && (t & 1) == 0) {
                return false;
            }
        }
        return true;
    }

private:
    static T mulMod(T a, T b, T M) {
#ifndef __GNUC__
        T x = 0, y = a % M;
        while (b > 0) {
            if (b & 1)
                x = (x + y) % M;

            y = (y << 1) % M;
            b >>= 1;
        }
        return x % M;
#else
        return static_cast<T>(__int128_t(a) * b % M);
#endif
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
};
