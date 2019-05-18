#pragma once

//--- Fraction Rank

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


//--- Continued Fraction

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
