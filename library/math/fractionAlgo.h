#pragma once

/*
   Continued Fraction : https://en.wikipedia.org/wiki/Continued_fraction

                          1   
    x = a(0) + ------------------------
                             1
               a(1) + -----------------
                                1
                    a(2) + ------------
                                    1
                           a(3) + -----
                                   ...

           ***

        h(0)   a(0)
    x = ---- = ----
        k(0)     1

        h(1)   a(1)a(0) + 1
    x = ---- = ------------
        k(1)       a(1)

        ...

        h(n)   a(n)h(n-1) + h(n-2)
    x = ---- = -------------------
        k(n)   a(n)k(n-1) + k(n-2)
*/

// return { (h(0), k(0)), (h(1), k(1)), ..., (h(i), k(i)), ... }
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


// coefficients or terms of the continued fraction
template <typename R = long double, typename T = long long>
vector<T> getCoefficientsOfContinuedFraction(R x, int maxCnt) {
    if (x == 0)
        return { 0 };

    vector<T> res;
    for (int i = 0; i < maxCnt && x != 0; i++) {
        T c = T(x);
        res.push_back(c);
        x = 1 / (x - c);
    }
    return res;
}

// x = num / denom
template <typename T = long long>
vector<T> getCoefficientsOfContinuedFraction(T num, T denom, int maxCnt) {
    if (num == 0)
        return { 0 };

    vector<T> res;
    for (int i = 0; i < maxCnt && num != 0; i++) {
        T c = num / denom;
        res.push_back(c);

        T t = num - denom * c;
        num = denom;
        denom = t;
    }
    return res;
}
