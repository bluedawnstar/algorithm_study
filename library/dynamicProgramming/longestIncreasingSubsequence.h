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
    
    //---

    // lexicographically first LIS, O(NlogN)
    template <typename T>
    static vector<T> findFirst(const T v[], int n) {
        if (n <= 0)
            return {};

        vector<int> prev(n);            // 
        vector<pair<T, int>> lis;       // lis[i] = (value, index)

        prev[0] = -1;
        lis.emplace_back(v[0], 0);
        for (int i = 1; i < n; i++) {
            if (lis.back().first < v[i]) {
                prev[i] = lis.back().second;
                lis.emplace_back(v[i], i);
            } else {
                int j = int(lower_bound(lis.begin(), lis.end(), make_pair(v[i], -1)) - lis.begin());
                if (j == 0)
                    prev[i] = -1;
                else
                    prev[i] = lis[j - 1].second;
                lis[j] = make_pair(v[i], i);
            }
        }

        vector<T> res;
        for (int i = lis.back().second; i >= 0; i = prev[i])
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

        vector<int> prev(n);                    // 
        vector<T> lis;                          // 
        vector<vector<pair<T, int>>> lis2;      // lis2[i] = { (value, index), ... }

        prev[0] = -1;
        lis.push_back(v[0]);
        lis2.push_back({ { -v[0], 0 } });
        for (int i = 1; i < n; i++) {
            int j = -1;
            if (lis.back() < v[i]) {
                j = int(lis.size());
                lis.push_back(v[i]);
                lis2.push_back({});
            } else {
                j = int(lower_bound(lis.begin(), lis.end(), v[i]) - lis.begin());
                lis[j] = v[i];
            }

            if (j == 0) {
                prev[i] = -1;
            } else {
                int k = int(upper_bound(lis2[j - 1].begin(), lis2[j - 1].end(), make_pair(-v[i], -1)) - lis2[j - 1].begin());
                prev[i] = lis2[j - 1][k].second;
            }
            lis2[j].emplace_back(-v[i], i);
        }

        vector<T> res;
        for (int i = lis2.back().front().second; i >= 0; i = prev[i])
            res.push_back(v[i]);
        reverse(res.begin(), res.end());

        return res;
    }

    template <typename T>
    static vector<T> findLast(const vector<T>& v) {
        return findLast(v.data(), int(v.size()));
    }
};
