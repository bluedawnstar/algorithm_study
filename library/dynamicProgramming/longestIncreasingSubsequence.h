#pragma once

struct LIS {
    // O(NlogN)
    template <typename T>
    static int findLength(const T v[], int n) {
        if (n <= 0)
            return 0;

        vector<T> res;
        res.push_back(v[0]);
        for (int i = 1; i < n; i++) {
            if (res.back() < v[i])
                res.push_back(v[i]);
            else
                *lower_bound(res.begin(), res.end(), v[i]) = v[i];
        }
        return int(res.size());
    }

    template <typename T>
    static int findLength(const vector<T>& v) {
        return findLength(v.data(), int(v.size()));
    }

    // forward, O(NlogN)
    template <typename T>
    static long long countLIS(const vector<T>& A) {
        int N = int(A.size());

        vector<int> dp;
        vector<vector<pair<T, long long>>> dp2; // dp2 = { (value, count), ... }

        auto compValue = [](const T& a, const pair<T, long long>& b) {
            return a > b.first;
        };

        for (int i = 0, j = 0; i < N; i++) {
            if (dp.empty() || dp.back() < A[i]) {
                j = int(dp.size());
                dp.push_back(A[i]);
                dp2.emplace_back(vector<pair<T, long long>>{ { numeric_limits<T>::max(), 0ll } });
            } else {
                j = int(lower_bound(dp.begin(), dp.end(), A[i]) - dp.begin());
                dp[j] = A[i];
            }

            long long cnt = 1;
            if (j > 0) {
                int first = int(upper_bound(dp2[j - 1].begin(), dp2[j - 1].end(), A[i], compValue) - dp2[j - 1].begin());
                cnt = dp2[j - 1].back().second - dp2[j - 1][first - 1].second;
            }
            dp2[j].emplace_back(A[i], dp2[j].back().second + cnt);
        }

        return dp2.back().back().second;
    }

    //---

    // lexicographically first LIS, O(NlogN)
    template <typename T>
    static vector<T> findFirst(const T v[], int n) {
        if (n <= 0)
            return {};

        vector<pair<T, int>> dp;        // dp[i] = (value, index)
        vector<int> prev(n);            // 

        prev[0] = -1;
        dp.emplace_back(v[0], 0);
        for (int i = 1; i < n; i++) {
            if (dp.back().first < v[i]) {
                prev[i] = dp.back().second;
                dp.emplace_back(v[i], i);
            } else {
                int j = int(lower_bound(dp.begin(), dp.end(), make_pair(v[i], -1)) - dp.begin());
                if (j == 0)
                    prev[i] = -1;
                else
                    prev[i] = dp[j - 1].second;
                dp[j] = make_pair(v[i], i);
            }
        }

        vector<T> res;
        for (int i = dp.back().second; i >= 0; i = prev[i])
            res.push_back(v[i]);
        reverse(res.begin(), res.end());

        return res;
    }

    template <typename T>
    static vector<T> findFirst(const vector<T>& v) {
        return findFirst(v.data(), int(v.size()));
    }

    //---

    // lexicographically last LIS, O(NlogN)
    template <typename T>
    static vector<T> findLast(const T v[], int n) {
        if (n <= 0)
            return {};

        vector<T> dp;                           // 
        vector<int> prev(n);                    // 
        vector<vector<pair<T, int>>> dp2;       // dp2[i] = { (-value, index), ... }

        prev[0] = -1;
        dp.push_back(v[0]);
        dp2.push_back({ { -v[0], 0 } });
        for (int i = 1; i < n; i++) {
            int j = -1;
            if (dp.back() < v[i]) {
                j = int(dp.size());
                dp.push_back(v[i]);
                dp2.push_back({});
            } else {
                j = int(lower_bound(dp.begin(), dp.end(), v[i]) - dp.begin());
                dp[j] = v[i];
            }

            if (j == 0) {
                prev[i] = -1;
            } else {
                int k = int(upper_bound(dp2[j - 1].begin(), dp2[j - 1].end(), make_pair(-v[i], n)) - dp2[j - 1].begin());
                prev[i] = dp2[j - 1][k].second;
            }
            dp2[j].emplace_back(-v[i], i);
        }

        vector<T> res;
        for (int i = dp2.back().front().second; i >= 0; i = prev[i])
            res.push_back(v[i]);
        reverse(res.begin(), res.end());

        return res;
    }

    template <typename T>
    static vector<T> findLast(const vector<T>& v) {
        return findLast(v.data(), int(v.size()));
    }
};
