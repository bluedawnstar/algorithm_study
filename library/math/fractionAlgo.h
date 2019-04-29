#pragma once

//--------- General Functions -------------------------------------------------

//CAUTION: use right type according to required precision (ex: long long, __int128_t)
// sum(floor(a/b * i)), i=0..n
inline long long sumFloor(long long n, long long a, long long b) {
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
    res += n * m + (n / b) - sumFloor(m, b, a);

    return res;
}

// sum(floor(a/b * i)), i=left..right
inline long long sumFloorRange(long long left, long long right, long long num, long long denom) {
    return sumFloor(right, num, denom) - (left > 1 ? sumFloor(left - 1, num, denom) : 0);
}


//--------- Continued Fraction ------------------------------------------------

template <typename T, typename R = T>
vector<pair<R, R>> continuedFraction(const vector<T>& a) {
    int N = int(a.size());
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
