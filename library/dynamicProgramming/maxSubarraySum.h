#pragma once

// Maximum Subarray Sum

struct MaxSubarraySum {
    template <typename T>
    static T kadane(const T v[], int n) {
        if (n <= 0)
            return 0;

        T sum = v[0];
        T maxSum = sum;
        for (int i = 1; i < n; i++) {
            sum = max(sum + v[i], v[i]);
            maxSum = max(maxSum, sum);
        }

        return maxSum;
    }

    template <typename T>
    static T kadane(const vector<T>& v) {
        return kadane(v.data(), int(v.size()));
    }

    //---

    // return (max sum, (range start, range end))
    template <typename T>
    static pair<T, pair<int, int>> kadaneEx(const T v[], int n) {
        pair<T, pair<int, int>> res(0, make_pair(-1, -1));
        if (n > 0) {
            res.first = v[0];
            res.second = make_pair(0, 0);

            T sum = v[0];
            int start = 0;

            for (int i = 1; i < n; i++) {
                if (v[i] <= sum + v[i]) {
                    sum += v[i];
                } else {
                    sum = v[i];
                    start = i;
                }
                if (res.first < sum) {
                    res.first = sum;
                    res.second = make_pair(start, i);
                }
            }
        }
        return res;
    }

    template <typename T>
    static pair<T, pair<int, int>> kadaneEx(const vector<T>& v) {
        return kadaneEx(v.data(), int(v.size()));
    }

    //---

    // return res[i] = (max sum in [0, i], start of max sum range)
    template <typename T>
    static vector<pair<T, int>> buildTableForward(const T v[], int n) {
        vector<pair<T, int>> res(n);
        if (n > 0) {
            T sum = v[0];
            int start = 0;

            res[0] = make_pair(sum, start);
            for (int i = 1; i < n; i++) {
                if (v[i] <= sum + v[i]) {
                    sum += v[i];
                } else {
                    sum = v[i];
                    start = i;
                }
                res[i] = make_pair(sum, start);
            }
        }
        return res;
    }

    template <typename T>
    static vector<pair<T, int>> buildTableForward(const vector<T>& v) {
        return buildTableForward(v.data(), int(v.size()));
    }

    //---

    // return res[i] = (max sum in [i, N - 1], end of max sum range)
    template <typename T>
    static vector<pair<T, int>> buildTableBackward(const T v[], int n) {
        vector<pair<T, int>> res(n);
        if (n > 0) {
            T sum = v[n - 1];
            int end = n - 1;

            res[n - 1] = make_pair(sum, end);
            for (int i = n - 2; i >= 0; i--) {
                if (v[i] <= sum + v[i]) {
                    sum += v[i];
                } else {
                    sum = v[i];
                    end = i;
                }
                res[i] = make_pair(sum, end);
            }
        }
        return res;
    }

    template <typename T>
    static vector<pair<T, int>> buildTableBackward(const vector<T>& v) {
        return buildTableBackward(v.data(), int(v.size()));
    }
};
