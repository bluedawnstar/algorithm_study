#pragma once

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
