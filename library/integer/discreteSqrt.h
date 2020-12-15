#pragma once

/*
  <Tonelli-Shanks algorithm>
  - algorithm to find a square root of n modulo MOD

    x^2 = n (mod MOD)


  https://en.wikipedia.org/wiki/Tonelli%E2%80%93Shanks_algorithm
  https://www.geeksforgeeks.org/find-square-root-modulo-p-set-2-shanks-tonelli-algorithm/

  from https://github.com/zimpha/algorithmic-library/blob/master/cpp/mathematics/mod_sqrt.cc
*/

struct DiscreteSqrt32bit;
struct DiscreteSqrt64bit;

struct DiscreteSqrt32bit {
    // Tonelli-Shanks algorithm, O(logM)
    static int solve(int n, int mod) {
        if (n == 0)
            return 0;
        if (mod == 2)
            return (n & 1) ? 1 : -1;
        if (modPow(n, mod >> 1, mod) != 1)
            return -1;
        if (mod & 2)
            return modPow(n, (mod + 1) >> 2, mod);
        int s = ctz(mod ^ 1);
        int q = mod >> s, z = 2;
        for (; modPow(z, mod >> 1, mod) == 1; ++z)
            ;
        int c = modPow(z, q, mod);
        int r = modPow(n, (q + 1) >> 1, mod);
        int t = modPow(n, q, mod);
        for (int m = s; t != 1;) {
            int i, tmp;
            for (i = 0, tmp = t; tmp != 1; i++) {
                tmp = int(1ll * tmp * tmp % mod);
            }
            while (i < --m)
                c = int(1ll * c * c % mod);
            r = int(1ll * r * c % mod);
            c = int(1ll * c * c % mod);
            t = int(1ll * t * c % mod);
        }
        return min(r, mod - r);
    }

private:
    // counting trailing zeros
    static int ctz(unsigned x) {
        if (!x)
            return 32;
#ifndef __GNUC__
        return int(_tzcnt_u32(x));
#else
        return __builtin_ctz(x);
#endif
    }

    static int ctz(int x) {
        return ctz(static_cast<unsigned int>(x));
    }

    static int modPow(int x, int n, int mod) {
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

    static int modPow(int x, long long n, int mod) {
        if (x == 0 && n > 0)
            return 0;
        return modPow(x, int(n % (mod - 1)), mod);
    }
};

struct DiscreteSqrt64bit {
    // Tonelli-Shanks algorithm, O(logM)
    static long long solve(long long n, long long mod) {
        if (n == 0)
            return 0;
        if (mod == 2)
            return (n & 1) ? 1 : -1;
        if (modPow(n, mod >> 1, mod) != 1)
            return -1;
        if (mod & 2)
            return modPow(n, (mod + 1) >> 2, mod);
        int s = ctz(mod ^ 1);
        long long q = mod >> s, z = 2;
        for (; modPow(z, mod >> 1, mod) == 1; ++z)
            ;
        long long c = modPow(z, q, mod);
        long long r = modPow(n, (q + 1) >> 1, mod);
        long long t = modPow(n, q, mod);
        for (int m = s; t != 1;) {
            int i;
            long long tmp;
            for (i = 0, tmp = t; tmp != 1; ++i) {
                tmp = modMul(tmp, tmp, mod);
            }
            for (; i < --m;)
                c = modMul(c, c, mod);
            r = modMul(r, c, mod);
            c = modMul(c, c, mod);
            t = modMul(t, c, mod);
        }
        return min(r, mod - r);
    }

private:
    static int ctz(unsigned long long x) {
        if (!x)
            return 64;
#if defined(_M_X64)
        return int(_tzcnt_u64(x));
#elif defined(__GNUC__)
        return __builtin_ctzll(x);
#else
        if ((x >> 32) != 0)
            return int(_tzcnt_u32(unsigned(x >> 32)));
        else
            return 32 + int(_tzcnt_u32(unsigned(x)));
#endif
    }

    static int ctz(long long x) {
        return ctz(static_cast<unsigned long long>(x));
    }

    static long long modMul(long long a, long long b, long long mod) {
#ifdef __GNUC__
        return (long long)((__int128_t)(a % mod) * (b % mod) % mod);
#else
        int base = int(1e9);
        long long aLow = a % base, aHigh = a / base;
        long long bLow = b % base, bHigh = b / base;

        long long result = (aHigh * bHigh) % mod;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % mod;

        result = (result + aLow * bHigh % mod + bLow * aHigh % mod) % mod;
        for (int i = 0; i < 9; i++)
            result = (result * 10) % mod;

        result = (result + aLow * bLow % mod) % mod;

        return result;
#endif
    }

    static long long modPow(long long x, long long n, long long mod) {
        if (n == 0)
            return 1ll;

        long long res = 1ll;
#ifdef __GNUC__
        long long t = x % mod;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = (long long)((__int128_t)t * res % mod);
            t = (long long)((__int128_t)t * t % mod);
        }
#else
        long long t = x % mod;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = modMul(res, t, mod);
            t = modMul(t, t, mod);
        }
#endif
        return res;
    }
};
