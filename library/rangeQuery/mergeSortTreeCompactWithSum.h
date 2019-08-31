#pragma once

// space : O(NlogN)
template <typename T, typename SumT = T>
struct CompactMergeSortTreeWithSum {
    int                 N;      // the size of array
    vector<vector<T>>   tree;   // 
    vector<vector<SumT>> sum;   // 

    CompactMergeSortTreeWithSum() : N(0) {
    }

    CompactMergeSortTreeWithSum(const T v[], int n) {
        build(v, n);
    }

    explicit CompactMergeSortTreeWithSum(const vector<T>& v) {
        build(v);
    }


    // O(NlogN)
    void build(const T v[], int n) {
        N = n;
        tree.resize(n * 2);
        sum.resize(n * 2);

        for (int i = 0; i < n; i++) {
            tree[n + i].resize(1);
            tree[n + i][0] = v[i];

            sum[n + i].resize(2);
            sum[n + i][1] = v[i];
        }

        for (int i = n - 1; i >= 1; i--) {
            auto& L = tree[i * 2];
            auto& R = tree[i * 2 + 1];

            int sz = int(L.size() + R.size());
            tree[i].resize(sz);
            merge(L.begin(), L.end(), R.begin(), R.end(), tree[i].begin());

            sum[i].resize(sz + 1);
            for (int j = 1; j <= sz; j++)
                sum[i][j] = sum[i][j - 1] + tree[i][j - 1];
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
    pair<int, SumT> countLessThanOrEqual(int left, int right, T val) const {
        pair<int, SumT> res;

        for (int L = left + N, R = right + N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1) {
                int cnt = countLessThanOrEqual(tree[L], val);
                res.first += cnt;
                res.second += sum[L][cnt];
                L++;
            }
            if ((R & 1) == 0) {
                int cnt = countLessThanOrEqual(tree[R], val);
                res.first += cnt;
                res.second += sum[R][cnt];
                R--;
            }
        }

        return res;
    }

    // O(logN)
    pair<int, SumT> countLessThanOrEqual(T val) const {
        int cnt = countLessThanOrEqual(tree[1], val);
        return make_pair(cnt, sum[1][cnt]);
    }

    // count a value k, O((logN)^2), inclusive (0 <= left <= right < N)
    pair<int, SumT> count(int left, int right, T val) const {
        auto r = countLessThanOrEqual(left, right, val);
        auto l = countLessThanOrEqual(left, right, val - 1);
        return make_pair(r.first - l.first, r.second - l.second);
    }

    // O(logN)
    pair<int, SumT> count(T val) const {
        auto r = countLessThanOrEqual(val);
        auto l = countLessThanOrEqual(val - 1);
        return make_pair(r.first - l.first, r.second - l.second);
    }

    // count a value k, O((logN)^2), inclusive (0 <= left <= right < N)
    pair<int, SumT> count(int left, int right, T valLow, T valHigh) const {
        auto r = countLessThanOrEqual(left, right, valHigh);
        auto l = countLessThanOrEqual(left, right, valLow - 1);
        return make_pair(r.first - l.first, r.second - l.second);
    }

    // O(logN)
    pair<int, SumT> count(T valLow, T valHigh) const {
        auto r = countLessThanOrEqual(valHigh);
        auto l = countLessThanOrEqual(valLow - 1);
        return make_pair(r.first - l.first, r.second - l.second);
    }

    // O((logN)^2 * logA), inclusive (0 <= left <= right < N, 0 <= k <= valHigh - valLow)
    // If not found, return valHigh
    T kth(int left, int right, int k, T valLow, T valHigh) const {
        T lo = valLow, hi = valHigh;
        while (lo <= hi) {
            T mid = lo + (hi - lo) / 2;
            if (countLessThanOrEqual(left, right, mid).first >= k + 1)
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
    int countLessThanOrEqual(const vector<T>& v, T x) const {
        return int(upper_bound(v.begin(), v.end(), x) - v.begin());
    }
};
