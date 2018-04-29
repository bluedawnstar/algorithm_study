#pragma once

template <typename T>
struct MergeSortTreeIndex {
    int                 N;        // the size of array
    vector<T>           values;   // 
    vector<vector<T>>   tree;     // 

    MergeSortTreeIndex() : N(0) {
    }

    MergeSortTreeIndex(const T arr[], int n) {
        build(arr, n);
    }

    explicit MergeSortTreeIndex(const vector<T>& v) {
        build(v);
    }


    // O(NlogN)
    void build(const T arr[], int n) {
        N = n;
        tree.resize(n * 4);

        values.assign(arr, arr + n);

        vector<pair<T, int>> v;
        v.reserve(n);
        for (int i = 0; i < n; i++)
            v.emplace_back(arr[i], i);
        sort(v.begin(), v.end());

        buildSub(v, 0, n - 1, 0);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }


    // O((logN)^3), inclusive (0 <= left <= right < N)
    int countLessThanOrEqual(int left, int right, T val) const {
        int lo = 0, hi = right - left;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (kth(left, right, mid) > val)
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        return lo;   // upper bound
    }

    int countLessThanOrEqual(T val) const {
        return countLessThanOrEqual(0, N - 1, val);
    }

    // count a value k, O((logN)^3), inclusive (0 <= left <= right < N)
    int count(int left, int right, T val) const {
        return countLessThanOrEqual(left, right, val) - countLessThanOrEqual(left, right, val - 1);
    }

    int count(T val) const {
        return countLessThanOrEqual(0, N - 1, val) - countLessThanOrEqual(0, N - 1, val - 1);
    }

    // count a value k, O((logN)^3), inclusive (0 <= left <= right < N)
    int count(int left, int right, T valLow, T valHigh) const {
        return countLessThanOrEqual(left, right, valHigh) - countLessThanOrEqual(left, right, valLow - 1);
    }

    int count(T valLow, T valHigh) const {
        return countLessThanOrEqual(0, N - 1, valHigh) - countLessThanOrEqual(0, N - 1, valLow - 1);
    }


    // O((logN)^2), inclusive (0 <= left <= right < N, 0 <= k <= right - left)
    int kthIndex(int left, int right, int k) const {
        return kthSub(left, right, k, 0, 0, N - 1);
    }

    // O((logN)^2), inclusive (0 <= k < N)
    int kthIndex(int k) const {
        return kthSub(0, N - 1, k, 0, 0, N - 1);
    }

    // O((logN)^2), inclusive (0 <= left <= right < N, 0 <= k <= right - left)
    T kth(int left, int right, int k) const {
        return values[kthSub(left, right, k, 0, 0, N - 1)];
    }

    // O((logN)^2), inclusive (0 <= k < N)
    T kth(int k) const {
        return values[kthSub(0, N - 1, k, 0, 0, N - 1)];
    }


private:
    // inclusive
    void buildSub(const vector<pair<T,int>>& v, int left, int right, int node) {
        if (left == right) {
            tree[node].push_back(v[left].second);
            return;
        }

        int mid = left + (right - left) / 2;
        buildSub(v, left, mid, node * 2 + 1);
        buildSub(v, mid + 1, right, node * 2 + 2);
        merge(tree[2 * node + 1].begin(), tree[2 * node + 1].end(),
              tree[2 * node + 2].begin(), tree[2 * node + 2].end(),
              back_inserter(tree[node]));
    }

    int kthSub(int left, int right, int k, int node, int nodeLeft, int nodeRight) const {
        if (nodeLeft == nodeRight)
            return tree[node][0];

        int cnt = upper_bound(tree[2 * node + 1].begin(), tree[2 * node + 1].end(), right)
                - lower_bound(tree[2 * node + 1].begin(), tree[2 * node + 1].end(), left);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (cnt > k)
            return kthSub(left, right, k, 2 * node + 1, nodeLeft, mid);
        else
            return kthSub(left, right, k - cnt, 2 * node + 2, mid + 1, nodeRight);
    }
};
