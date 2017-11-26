#pragma once


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
pair<T, T> findKthSternBrocot(T denomRangeMin, T denomRangeMax, pair<T, T> left, pair<T, T> right, T rateL, T rateR, U cnt) {
    pair<T, T> mid = { rateL * left.first + rateR * right.first, rateL * left.second + rateR * right.second };
    U count = sumFloorRange<T, T, U>(denomRangeMin, denomRangeMax, mid.first, mid.second);
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
    return findKthSternBrocot(denomRangeMin, denomRangeMax, make_pair<T, T>(0, 1), make_pair<T, T>(1, 0), T(1), T(1), cnt);
}


//----------- Fraction class --------------------------------

template <typename T>
struct Fraction {
    explicit Fraction(T x) : num(x), denom(1) {
    }

    Fraction(T _num, T _denom) : num(_num), denom(_denom) {
    }

    bool isNaN() const {
        return denom == 0;
    }

    bool isZero() const {
        return num == 0;
    }

    T gcd() const {
        return gcd(num, denom);
    }

    void reduce() {
        T g = gcd();
        num /= g;
        denom /= g;
    }

    Fraction copyReduce() const {
        T g = gcd();
        return Fraction(num / g, denom / g);
    }

    Fraction& operator +=(const Fraction& rhs) {
        T g = gcd(denom, rhs.denom);
        num = num * rhs.denom / g + rhs.num * denom / g;
        denom = denom / g * rhs.denom;
        return *this;
    }

    Fraction& operator -=(const Fraction& rhs) {
        T g = gcd(denom, rhs.denom);
        num = num * rhs.denom / g - rhs.num * denom / g;
        denom = denom / g * rhs.denom;
        return *this;
    }

    Fraction& operator *=(const Fraction& rhs) {
        num *= rhs.num;
        denom *= rhs.denom;
        return *this;
    }

    Fraction& operator /=(const Fraction& rhs) {
        num *= rhs.denom;
        denom *= rhs.num;
        return *this;
    }

    Fraction operator +(const Fraction& rhs) const {
        T g = gcd(denom, rhs.denom);
        return Fraction(num * rhs.denom / g + rhs.num * denom / g, denom / g * rhs.denom);
    }

    Fraction operator -(const Fraction& rhs) const {
        T g = gcd(denom, rhs.denom);
        return Fraction(num * rhs.denom / g - rhs.num * denom / g, denom / g * rhs.denom);
    }

    Fraction operator *(const Fraction& rhs) const {
        return Fraction(num * rhs.num, denom * rhs.denom);
    }

    Fraction operator /(const Fraction& rhs) const {
        return Fraction(num * rhs.denom, denom * rhs.num);
    }

    Fraction operator -() const {
        return Fraction(-num, denom);
    }

    Fraction operator +() const {
        return Fraction(num, denom);
    }

    bool operator ==(const Fraction& rhs) const {
        return num * rhs.denom == denom * rhs.num;
    }

    bool operator !=(const Fraction& rhs) const {
        return !operator ==(rhs);
    }

    bool operator <(const Fraction& rhs) const {
        return num * rhs.denom < denom * rhs.num;
    }

    bool operator <=(const Fraction& rhs) const {
        return num * rhs.denom <= denom * rhs.num;
    }

    bool operator >(const Fraction& rhs) const {
        return num * rhs.denom > denom * rhs.num;
    }

    bool operator >=(const Fraction& rhs) const {
        return num * rhs.denom >= denom * rhs.num;
    }

    static T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }

    T num;
    T denom;
};
