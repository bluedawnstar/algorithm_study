#pragma once

template <typename T = long long>
struct FractionFinder2 {
    // inclusive
    static pair<pair<T, T>, pair<T, T>> solve(T denomRangeMin, T denomRangeMax, T targetNum, T targetDenom) {
        auto cnt = sumFloorRange(denomRangeMin, denomRangeMax, targetNum, targetDenom);
        auto L = findKthSternBrocot(denomRangeMin, denomRangeMax, cnt);
        auto R = findKthSternBrocot(denomRangeMin, denomRangeMax, cnt + 1);

        T scale1 = (denomRangeMin + L.second - 1) / L.second;
        L.first *= scale1;
        L.second *= scale1;

        T scale2 = (denomRangeMin + R.second - 1) / R.second;
        R.first *= scale2;
        R.second *= scale2;

        return make_pair(L, R);
    }

private:
    //CAUTION: use right type according to required precision (ex: long long, __int128_t)
    // sum(floor(a/b * i)), i=0..n
    template <typename T>
    static T sumFloor(T n, T a, T b) {
        //assert(gcd(a, b) == 1);

        T res = 0;
        if (a >= b) {
            res += (a / b) * n * (n + 1) / 2;
            a %= b;
        }
        if (a == 0 || n == 0) {
            return res;
        }

        T m = a * n / b;
        res += n * m + (n / b) - sumFloor(m, b, a);

        return res;
    }

    // sum(floor(a/b * i)), i=left..right
    template <typename T>
    static T sumFloorRange(T left, T right, T num, T denom) {
        return sumFloor(right, num, denom) - (left > 1 ? sumFloor(left - 1, num, denom) : 0);
    }

    // inclusive range
    static pair<T, T> findKthSternBrocot(T denomRangeMin, T denomRangeMax, pair<T, T> left, pair<T, T> right, T rateL, T rateR, T cnt) {
        pair<T, T> mid = { rateL * left.first + rateR * right.first, rateL * left.second + rateR * right.second };
        auto count = sumFloorRange(denomRangeMin, denomRangeMax, mid.first, mid.second);
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
    static pair<T, T> findKthSternBrocot(T denomRangeMin, T denomRangeMax, T cnt) {
        return findKthSternBrocot(denomRangeMin, denomRangeMax, make_pair<T, T>(0, 1), make_pair<T, T>(1, 0), T(1), T(1), cnt);
    }
};
