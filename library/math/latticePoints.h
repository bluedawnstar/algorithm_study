#pragma once

#include "fraction.h"

struct LatticePoints {
    /*
                             n
        sumFloor(a, b, n) = SUM floor(a / b * x) ,   x = 0, 1, 2, ..., n
                            x=0
    */
    // O(logN)
    static long long count(long long a, long long b, long long n) {
        //assert(gcd(a, b) == 1);
        long long res = 0;
        if (a >= b) {
            res += (a / b) * n * (n + 1) / 2;
            a %= b;
        }
        if (a == 0 || n == 0) {
            return res;
        }

        long long m = a * n / b;
        res += n * m + (n / b) - count(b, a, m);

        return res;
    }

    static long long countRange(long long a, long long b, long long left, long long right) {
        return count(a, b, right) - (left > 1 ? count(a, b, left - 1) : 0ll);
    }

    //---

    /*
                                n
        sumFloor(a, b, c, n) = SUM floor(a / b * x + c) ,   x = 0, 1, 2, ..., n
                               x=0
    */
    static long long count(long long a, long long b, long long c, long long n) {
        //assert(gcd(a, b) == 1);
        return (n + 1) * c + count(a, b, n);
    }

    static long long countRange(long long a, long long b, long long c, long long left, long long right) {
        return count(a, b, c, right) - (left > 0 ? count(a, b, c, left - 1) : 0ll);
    }

    //---

    /*
                             n
        sumFloor(a, b, n) = SUM floor((a.num / a.denom) * x + (b.num / b.denom)) ,   x = 0, 1, 2, ..., n
                            x=0
    */
    // O(logN)
    static long long count(Fraction<long long> a, Fraction<long long> b, long long n) {
        long long res = 0;

        auto fa = a.floor();
        auto fb = b.floor();
        if (a.num >= a.denom || b.num >= b.denom) {
            res += (fa * n + 2 * fb) * (n + 1) / 2;
            a -= fa;
            b -= fb;
        }

        auto t = a * (n + 1) + b;
        t.reduce();

        auto ft = t.floor();
        if (ft >= 1) {
            res += count(a.inverse(), (t - ft) * a.inverse(), ft - 1);
        }

        return res;
    }

    static long long countRange(Fraction<long long> a, Fraction<long long> b, long long left, long long right) {
        return count(a, b, right) - (left > 0 ? count(a, b, left - 1) : 0ll);
    }
};
