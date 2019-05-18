#pragma once

#include "fraction.h"

// using Stern-Brocot tree, Mediant, ...
template <typename T = int, typename RealT = double>
struct FractionFinder {
    struct Fract {
        T num;
        T denom;
    };
    struct FractRange {
        RealT err;
        Fract lo;
        Fract hi;

        bool operator <(const FractRange& rhs) const {
            return err < rhs.err;
        }
    };

    Fract ans;
    RealT ansDiff;

    T denomRangeMin;
    T denomRangeMax;

    FractionFinder() {
        setDenomRange(1, numeric_limits<T>::max());
        init();
    }

    FractionFinder(T denomRangeMin, T denomRangeMax) {
        setDenomRange(denomRangeMin, denomRangeMax);
        init();
    }

    void init() {
        ans.num = ans.denom = numeric_limits<T>::max();
        ansDiff = numeric_limits<RealT>::max();
    }

    void setDenomRange(T denomRangeMin, T denomRangeMax) {
        this->denomRangeMin = denomRangeMin;
        this->denomRangeMax = denomRangeMax;
    }

    Fract solve(RealT target) {
        init();

        Fract lo{ 0, 1 };
        Fract hi{ 1, 0 };

        updateAns(lo.num, lo.denom, target);
        updateAns(hi.num, hi.denom, target);

        priority_queue<FractRange> Q;
        Q.push(FractRange{ RealT(0), lo, hi });

        while (!Q.empty()) {
            auto u = Q.top();
            Q.pop();
            if (abs(u.err) > ansDiff)
                break;

            Fract f{ u.lo.num + u.hi.num, u.lo.denom + u.hi.denom };

            if (f.denom > denomRangeMax)
                continue;

            RealT err = (RealT)f.num / f.denom - target;
            if (err < 0) {
                Q.push(FractRange{ err, u.lo, f });
                Q.push(FractRange{ u.err, f, u.hi });
            } else {
                Q.push(FractRange{ -err, f, u.hi });
                Q.push({ u.err, u.lo, f });
            }
            updateAns(f.num, f.denom, target);
        }

        return ans;
    }

private:
    bool updateAns(T num, T denom, RealT target) {
        reduce(num, denom);

        if (denom > denomRangeMax)
            return false;

        RealT diff = abs((RealT)num / denom - target);
        if (diff < ansDiff) {
            if (denom < denomRangeMin) {
                T scale = ((denomRangeMin + denom - 1) / denom);
                num *= scale;
                denom *= scale;
            }

            if (denomRangeMin <= denom && denom <= denomRangeMax) {
                ans.num = num;
                ans.denom = denom;
                ansDiff = diff;

                return true;
            }
        }

        return false;
    }

    static T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }

    static void reduce(T& a, T& b) {
        T g = gcd(a, b);
        a /= g;
        b /= g;
    }

    static RealT abs(RealT x) {
        return x < 0 ? -x : x;
    }
};
