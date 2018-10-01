#pragma once

struct LIS {
    // O(NlogN)
    template <typename T>
    static int findLength(const T v[], int n) {
        vector<T> res;
        if (n > 0) {
            res.reserve(n);
            res.push_back(v[0]);
            for (int i = 1; i < n; i++) {
                if (res.back() < v[i])
                    res.push_back(v[i]);
                else
                    *lower_bound(res.begin(), res.end(), v[i]) = v[i];
            }
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
        vector<T> res;

        vector<T> dp;
        if (n > 0) {
            dp.reserve(n);

            dp.push_back(v[0]);
            res = dp;
            for (int i = 1; i < n; i++) {
                int j = -1;
                if (dp.back() < v[i]) {
                    j = int(dp.size());
                    dp.push_back(v[i]);
                } else {
                    j = int(lower_bound(dp.begin(), dp.end(), v[i]) - dp.begin());
                    dp[j] = v[i];
                }
                if (j + 1 == int(dp.size()))
                    res = dp;
            }
        }
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
        vector<T> res;

        vector<T> dp;
        if (n > 0) {
            dp.reserve(n);

            dp.push_back(v[0]);
            res = dp;
            for (int i = 1; i < n; i++) {
                if (dp.back() < v[i]) {
                    dp.push_back(v[i]);
                    res.push_back(v[i]);
                } else {
                    *lower_bound(dp.begin(), dp.end(), v[i]) = v[i];
                }
            }
        }
        return res;
    }

    template <typename T>
    static vector<T> findLast(const vector<T>& v) {
        return findLast(v.data(), int(v.size()));
    }
};
