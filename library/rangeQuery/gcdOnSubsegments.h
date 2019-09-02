#pragma once

struct GcdOnSubsegments {
    // O(N*(logX)^2)
    // result[L] = { (R0 + 1, gcd(L, R0)), (R1 + 1, gcd(L, R1)), (R2 + 1, gcd(L, R2)), ... }
    template <typename T>
    static vector<vector<pair<int, T>>> buildLeftAligned(const vector<T>& values) {
        int N = int(values.size());
        vector<vector<pair<int, T>>> res(N);

        res[N - 1].emplace_back(N, values[N - 1]);
        for (int L = N - 2; L >= 0; L--) {
            T g = values[L];
            int R = L + 1;
            res[L].emplace_back(R, g);
            if (R >= N)
                continue;

            for (auto& it : res[R]) {
                T g2 = gcd(g, it.second);
                if (g2 == g)
                    res[L].back().first = it.first;
                else {
                    g = g2;
                    res[L].emplace_back(it.first, g);
                }
            }
        }

        return res;
    }

    // O(N*(logX)^2)
    // result[R] = { ..., (L2 - 1, gcd(L2, R)), (L1 - 1, gcd(L1, R)), (L0 - 1, gcd(L0, R)) }
    template <typename T>
    static vector<vector<pair<int, T>>> buildRightAligned(const vector<T>& values) {
        int N = int(values.size());
        vector<vector<pair<int, T>>> res(N);

        res[0].emplace_back(-1, values[0]);
        for (int R = 1; R < N; R++) {
            T g = values[R];
            int L = R - 1;
            res[R].emplace_back(L, g);
            if (L < 0)
                continue;

            for (auto& it : res[L]) {
                T g2 = gcd(g, it.second);
                if (g2 == g)
                    res[R].back().first = it.first;
                else {
                    g = g2;
                    res[R].emplace_back(it.first, g);
                }
            }
        }

        return res;
    }

//private:
    template <typename T>
    static T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }
};
