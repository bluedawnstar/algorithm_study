#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include <queue>
#include <algorithm>
#include "fraction.h"

using namespace std;

// 
// <Related Links>
// http://www.sciencedirect.com/science/article/pii/S0166218X09000894
// https://en.wikipedia.org/wiki/Continued_fraction
// https://en.wikipedia.org/wiki/Farey_sequence
// https://en.wikipedia.org/wiki/Mediant_(mathematics)
// https://en.wikipedia.org/wiki/Stern%E2%80%93Brocot_tree
// 
// <HackerRank's Question & Solutions>
// https://www.hackerrank.com/contests/w29/challenges/minimal-distance-to-pi/submissions/code/1300697342
// https://www.hackerrank.com/contests/w29/challenges/minimal-distance-to-pi/submissions/code/1300700017
// https://codepair.hackerrank.com/paper/OqDsIuXQ?b=eyJyb2xlIjoiY2FuZGlkYXRlIiwibmFtZSI6ImJsdWVkYXduc3RhciIsImVtYWlsIjoieW91bmdtYW4ucm9AZ21haWwuY29tIn0%3D
// https://codepair.hackerrank.com/paper/PLC7kLzG?b=eyJyb2xlIjoiY2FuZGlkYXRlIiwibmFtZSI6ImJsdWVkYXduc3RhciIsImVtYWlsIjoieW91bmdtYW4ucm9AZ21haWwuY29tIn0%3D
// 
// <PI>
// http://qin.laya.com/tech_projects_approxpi.html
// https://oeis.org/A001203
// 

//--------- General Functions -------------------------------------------------

//TODO: fix return type 'R' to right type according to required precision of a/b (ex: __int128)

// sum(floor(a/b * i)), i=0..n
template <typename T, typename U = T, typename R = T>
R sumFloor(T n, U a, U b) {
    //assert(gcd(a, b) == 1);

    R res = 0;
    if (a >= b) {
        res += R(a / b) * n * (n + 1) / 2;
        a %= b;
    }
    if (a == 0 || n == 0) {
        return res;
    }

    T m = T(R(a) * n / b);
    res += R(n) * m + (n / b) - sumFloor<T, U, R>(m, b, a);

    return res;
}

// sum(floor(a/b * i)), i=left..right
template <typename T, typename U = T, typename R = T>
R sumFloorRange(T left, T right, U num, U denom) {
    return sumFloor<T, U, R>(right, num, denom) - (left > 1 ? sumFloor<T, U, R>(left - 1, num, denom) : 0);
}


//--------- Continued Fraction ------------------------------------------------

template <typename T, typename R = T>
vector<pair<R, R>> continuedFraction(const vector<T>& a) {
    int N = (int)a.size();
    vector<pair<R, R>> res;
    res.reserve(N);

    res.emplace_back(R(a[0]), R(1));
    if (N > 1)
        res.emplace_back(R(a[0]) * a[1] + 1, R(a[1]));

    for (int i = 2; i < N; i++) {
        res.emplace_back(res[i - 2].first + res[i - 1].first * a[i],
                         res[i - 2].second + res[i - 1].second * a[i]);
    }

    return res;
}

//--------- Fraction Finding --------------------------------------------------
// using Stern-Brocot tree, Mediant, ...

template <typename T, typename RealT>
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

    Fract mAns;
    RealT mDiff;

    T mDenomRangeMin;
    T mDenomRangeMax;

    FractionFinder() {
        setDenomRange(1, numeric_limits<T>::max());
        init();
    }

    FractionFinder(T denomRangeMin, T denomRangeMax) {
        setDenomRange(denomRangeMin, denomRangeMax);
        init();
    }

    void init() {
        mAns.num = mAns.denom = numeric_limits<T>::max();
        // TODO: if RealT's numeric_limits<> is not supported, fix this value with real maximum value
        mDiff = numeric_limits<RealT>::max();
    }

    void setDenomRange(T denomRangeMin, T denomRangeMax) {
        mDenomRangeMin = denomRangeMin;
        mDenomRangeMax = denomRangeMax;
    }

    Fract solve(RealT target) {
        init();

        Fract lo{ 0, 1 };
        Fract hi{ 1, 0 };

        updateAns(lo.num, lo.denom, target);
        updateAns(hi.num, hi.denom, target);

        priority_queue<FractRange> Q;
        Q.push(FractRange{ (RealT)0, lo, hi });

        while (!Q.empty()) {
            auto u = Q.top();
            Q.pop();
            if (abs(u.err) > mDiff)
                break;

            Fract f{ u.lo.num + u.hi.num, u.lo.denom + u.hi.denom };

            if (f.denom > mDenomRangeMax)
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

        return mAns;
    }

    bool updateAns(T num, T denom, RealT target) {
        reduce(num, denom);

        if (denom > mDenomRangeMax)
            return false;

        RealT diff = abs((RealT)num / denom - target);
        if (diff < mDiff) {
            if (denom < mDenomRangeMin) {
                T scale = ((mDenomRangeMin + denom - 1) / denom);
                num *= scale;
                denom *= scale;
            }

            if (mDenomRangeMin <= denom && denom <= mDenomRangeMax) {
                mAns.num = num;
                mAns.denom = denom;
                mDiff = diff;
            }

            return true;
        }

        return false;
    }

private:
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


//--------- Stern-Brocot Tree -------------------------------------------------

// inclusive range
template <typename T, typename U = T>
pair<T,T> findKthSternBrocot(T denomRangeMin, T denomRangeMax, pair<T,T> left, pair<T,T> right, T rateL, T rateR, U cnt) {
    pair<T,T> mid = { rateL * left.first + rateR * right.first, rateL * left.second + rateR * right.second };
    U count = sumFloorRange<T,T,U>(denomRangeMin, denomRangeMax, mid.first, mid.second);
    T cntEq = denomRangeMax / mid.second - (denomRangeMin - 1) / mid.second;

    if (count - cntEq < cnt && cnt <= count)
        return mid;

    if (count >= cnt) {
        return (rateR == 1) ? findKthSternBrocot(denomRangeMin, denomRangeMax, left, mid, 2 * rateL, T(1), cnt)
                            : findKthSternBrocot(denomRangeMin, denomRangeMax, left, right, T(1), rateR / 2, cnt);
    } else {
        return (rateL == 1) ? findKthSternBrocot(denomRangeMin, denomRangeMax, mid, right, T(1), 2 * rateR, cnt)
                            : findKthSternBrocot(denomRangeMin, denomRangeMax, left, right, rateL / 2, T(1), cnt);
    }
}

// inclusive range
template <typename T, typename U = T>
pair<T, T> findKthSternBrocot(T denomRangeMin, T denomRangeMax, U cnt) {
    return findKthSternBrocot(denomRangeMin, denomRangeMax, make_pair<T,T>(0, 1), make_pair<T,T>(1, 0), T(1), T(1), cnt);
}

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

template <typename T>
ostream& operator <<(ostream& os, const Fraction<T>& f) {
    os << "(" << f.num << " / " << f.denom << ")";
    return os;
}

void testFraction() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--------- testFraction() -----------------" << endl;

    Fraction<int> f1(6, 21);
    assert(f1.copyReduce() == Fraction<int>(2, 7));

    Fraction<int> f2(9, 12);

    assert(f1 + f2 == Fraction<int>(87, 84));
    assert(f1 - f2 == Fraction<int>(-39, 84));
    assert(f1 * f2 == Fraction<int>(54, 252));
    assert(f1 / f2 == Fraction<int>(72, 189));
    assert(-f1 == Fraction<int>(-6, 21));
    assert(+f1 == f1);

    Fraction<int> f3(2, 7);
    assert(f1 == f3);
    Fraction<int> f4(3, 7);
    assert(f1 < f4);
    assert(f4 > f1);

    //--- test continued fraction ---------------------------------------------

    cout << "*** PI's fractions" << endl;
    vector<long long> piA = vector<long long>{ 3, 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14, 2, 1, 1, 2, 2, 2, 2, 1, 84, 2, };
    auto v = continuedFraction(piA);
    for (auto it : v) {
        cout << it.first << "/" << it.second << endl;
    }

    //--- test FractionFinder class -------------------------------------------

    cout << "*** Fraction finding " << endl;

    FractionFinder<long long, double> ff;

    static struct TestData {
        long long from, to;
        long long num, denom;
    } sIn[] = {
        { 1, 10, 22, 7 },
        { 482480, 1196809, 3126535, 995207 },
        { 359407, 835582, 1146408, 364913 },
        { 622459, 1367716, 4272943, 1360120 },
        { 646699, 1108000, 3126535, 995207 },
        { 819345, 1327749, 3126535, 995207 },
        { 450214, 458009, 1438862, 458004 },
        { 476934, 927416, 2292816, 729826 },
        { 724574, 1347684, 3126535, 995207 },
        { 766766, 1488720, 4272943, 1360120 },
        { 16706, 61598, 104348, 33215 }
    };

    for (int i = 0; i < sizeof(sIn) / sizeof(sIn[0]); i++) {
        ff.setDenomRange(sIn[i].from, sIn[i].to);
        auto f = ff.solve(M_PI);
        assert(f.num == sIn[i].num && f.denom == sIn[i].denom);
    }

    cout << "OK!" << endl;

    //--- test Stern-Brocot tree ----------------------------------------------

    cout << "*** Stern-Brocot tree" << endl;

    for (int i = 0; i < sizeof(sIn) / sizeof(sIn[0]); i++) {
        long long lo = sIn[i].from, hi = sIn[i].to;

        auto cnt = sumFloorRange(lo, hi, 899125804609ll, 286200632530ll);
        auto c1 = findKthSternBrocot(lo, hi, cnt);
        auto c2 = findKthSternBrocot(lo, hi, cnt + 1);

        long long scale1 = (lo + c1.second - 1) / c1.second;
        c1.first *= scale1;
        c1.second *= scale1;

        long long scale2 = (lo + c2.second - 1) / c2.second;
        c2.first *= scale2;
        c2.second *= scale2;

        assert((c1.first == sIn[i].num || c2.first == sIn[i].num) &&
               (c1.second == sIn[i].denom || c2.second == sIn[i].denom));
    }
    cout << "OK!" << endl;
}
