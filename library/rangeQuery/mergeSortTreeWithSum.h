#pragma once

// space : O(NlogN)
template <typename T, typename SumT = T>
struct MergeSortTreeWithSum {
    int                 N;      // the size of array
    vector<vector<T>>   tree;   // 
    vector<vector<SumT>> sum;   // 

    MergeSortTreeWithSum() : N(0) {
    }

    MergeSortTreeWithSum(const T arr[], int n) {
        build(arr, n);
    }

    explicit MergeSortTreeWithSum(const vector<T>& v) {
        build(v);
    }


    // O(NlogN)
    void build(const T arr[], int n) {
        N = n;
        tree.resize(n * 4);
        sum.resize(n * 4);
        buildSub(arr, 0, n - 1, 0);
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    // O((logN)^2), inclusive (0 <= left <= right < N)
    pair<int, SumT> countLessThanOrEqual(int left, int right, T val) const {
        return countLessThanOrEqualSub(left, right, val, 0, 0, N - 1);
    }

    // O(logN)
    pair<int, SumT> countLessThanOrEqual(T val) const {
        int cnt = countLessThanOrEqualSub(tree[0], val);
        return make_pair(cnt, sum[0][cnt]);
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

    // O((logN)^2 * logA), inclusive (0 <= left <= right < N, 0 <= k <= valLow - valHigh)
    T kth(int left, int right, int k, T valLow, T valHigh) const {
        T lo = valLow, hi = valHigh;
        while (lo <= hi) {
            T mid = lo + (hi - lo) / 2;
            if (countLessThanOrEqual(left, right, mid).first >= k + 1)
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        return lo;
    }

    // O(1), inclusive (0 <= k < N)
    T kth(int k) const {
        return tree[0][k];
    }


private:
    // inclusive
    void buildSub(const T arr[], int left, int right, int node) {
        if (left == right) {
            tree[node].push_back(arr[left]);
            sum[node].resize(2);
            sum[node][1] = arr[left];
            return;
        }

        int sz = right - left + 1;

        int mid = left + (right - left) / 2;
        buildSub(arr, left, mid, node * 2 + 1);
        buildSub(arr, mid + 1, right, node * 2 + 2);

        tree[node].resize(sz);
        merge(tree[2 * node + 1].begin(), tree[2 * node + 1].end(),
            tree[2 * node + 2].begin(), tree[2 * node + 2].end(),
            tree[node].begin());

        sum[node].resize(sz + 1);
        for (int i = 1; i <= sz; i++)
            sum[node][i] = sum[node][i - 1] + tree[node][i - 1];
    }

    pair<int, SumT> countLessThanOrEqualSub(int left, int right, T k, int node, int nodeLeft, int nodeRight) const {
        if (right < nodeLeft || nodeRight < left)
            return make_pair(0, SumT(0));

        if (left <= nodeLeft && nodeRight <= right) {
            int cnt = int(upper_bound(tree[node].begin(), tree[node].end(), k) - tree[node].begin());
            return make_pair(cnt, sum[node][cnt]);
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        auto l = countLessThanOrEqualSub(left, right, k, 2 * node + 1, nodeLeft, mid);
        auto r = countLessThanOrEqualSub(left, right, k, 2 * node + 2, mid + 1, nodeRight);
        return make_pair(l.first + r.first, l.second + r.second);
    }

    int countLessThanOrEqualSub(const vector<int>& v, T k) const {
        return int(upper_bound(v.begin(), v.end(), k) - v.begin());
    }
};
