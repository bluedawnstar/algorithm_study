#pragma once

// space : O(NlogN)
template <typename T>
struct CompactMergeSortTree {
    int               N;        // the size of array
    vector<vector<T>> tree;     // 

    CompactMergeSortTree() : N(0) {
    }

    CompactMergeSortTree(const T v[], int n) {
        build(v, n);
    }

    explicit CompactMergeSortTree(const vector<T>& v) {
        build(v);
    }


    // O(NlogN)
    void build(const T v[], int n) {
        N = n;
        tree.resize(n * 2);

        for (int i = 0; i < n; i++) {
            tree[n + i].resize(1);
            tree[n + i][0] = v[i];
        }

        for (int i = n - 1; i >= 1; i--) {
            auto& L = tree[i * 2];
            auto& R = tree[i * 2 + 1];
            tree[i].resize(L.size() + R.size());
            merge(L.begin(), L.end(), R.begin(), R.end(), tree[i].begin());
        }
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }

    // (min value, max value)
    pair<T, T> getValueRange(int left, int right) const {
        pair<T, T> res(numeric_limits<T>::max(), numeric_limits<T>::min());

        for (int L = left + N, R = right + N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1) {
                res.first = min(res.first, tree[L].front());
                res.second = max(res.second, tree[L].back());
                L++;
            }
            if ((R & 1) == 0) {
                res.first = min(res.first, tree[R].front());
                res.second = max(res.second, tree[R].back());
                R--;
            }
        }

        return res;
    }

    // O((logN)^2), inclusive (0 <= left <= right < N)
    int countLessThanOrEqual(int left, int right, T val) const {
        int res = 0;

        for (int L = left + N, R = right + N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1)
                res += countLessThanOrEqual(tree[L++], val);
            if ((R & 1) == 0)
                res += countLessThanOrEqual(tree[R--], val);
        }

        return res;
    }

    // O(logN)
    int countLessThanOrEqual(T val) const {
        return countLessThanOrEqual(tree[1], val);
    }

    // count a value k, O((logN)^2), inclusive (0 <= left <= right < N)
    int count(int left, int right, T val) const {
        return countLessThanOrEqual(left, right, val) - countLessThanOrEqual(left, right, val - 1);
    }

    // O(logN)
    int count(T val) const {
        return countLessThanOrEqual(val) - countLessThanOrEqual(val - 1);
    }

    // count a value k, O((logN)^2), inclusive (0 <= left <= right < N)
    int count(int left, int right, T valLow, T valHigh) const {
        return countLessThanOrEqual(left, right, valHigh) - countLessThanOrEqual(left, right, valLow - 1);
    }

    // O(logN)
    int count(T valLow, T valHigh) const {
        return countLessThanOrEqual(valHigh) - countLessThanOrEqual(valLow - 1);
    }

    // O((logN)^2 * logA), inclusive (0 <= left <= right < N, 0 <= k <= right - left)
    // If not found, return valHigh
    T kth(int left, int right, int k, T valLow, T valHigh) const {
        T lo = valLow, hi = valHigh;
        while (lo <= hi) {
            T mid = lo + (hi - lo) / 2;
            if (countLessThanOrEqual(left, right, mid) >= k + 1)
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        return lo; // lower bound
    }

    // O(1), inclusive (0 <= k < N)
    T kth(int k) const {
        return tree[1][k];
    }

private:
    int countLessThanOrEqual(const vector<T>& v, int x) const {
        return int(upper_bound(v.begin(), v.end(), x) - v.begin());
    }
};
