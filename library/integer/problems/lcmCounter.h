#pragma once

// https://www.hackerearth.com/problem/algorithm/lcm-range-queries-a890aff3/
// 
// the number of pair(A, B) such that lcm(A, A + 1, A + 2, ..., B) == X and 1 <= A < B <= R      (X, R <= maxx)
struct LcmCounter {
    vector<pair<long long, long long>> dp;    // { (lcm, R), ... }

    LcmCounter() {
    }

    explicit LcmCounter(long long maxx) {
        prepare(maxx);
    }

    // O(maxx^(1/3)*log(maxx))
    void prepare(long long maxx = 10'000'000'000'000'000ll) {
        dp.clear();

        for (int R = 3; R <= maxx; R++) {
            if (1ll * R * (R - 1) / 2 * (R - 2) > maxx)
                break;

            long long lc = lcm(1ll * R * (R - 1), static_cast<long long>(R - 2), maxx);
            if (lc > 0 && lc <= maxx)
                dp.emplace_back(lc, R);
            for (int i = R - 3; i >= 1; i--) {
                lc = lcm(lc, static_cast<long long>(i), maxx);
                if (lc <= 0 || lc > maxx)
                    break;
                dp.emplace_back(lc, R);
            }
        }

        sort(dp.begin(), dp.end());
    }

    // O(logR)
    long long countLcm(long long X, long long R) {
        auto itL = lower_bound(dp.begin(), dp.end(), make_pair(X, 2ll));
        auto itR = upper_bound(dp.begin(), dp.end(), make_pair(X, R));

        return exist2(X, R) + static_cast<long long>(itR - itL);
    }

private:
    // recursive
    template <typename T>
    T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }

    template <typename T>
    T lcm(T p, T q) {
        return (p / gcd(p, q)) * q;
    }

    template <typename T>
    T lcm(T p, T q, T maxx) {
        T g = gcd(p, q);
        if (p / g > maxx / q)
            return 0;
        return (p / g) * q;
    }

    int exist2(long long X, long long R) {
        int x2 = int(sqrt(X));
        if (1ll * x2 * (x2 + 1) == X && x2 + 1 <= R)
            return 1;
        return 0;
    }
};
