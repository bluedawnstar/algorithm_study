#pragma once

template <typename T>
struct MergeSortTree {
    int               N;        // the size of array
    vector<vector<T>> tree;     // 

    MergeSortTree() : N(0) {
    }

    MergeSortTree(const T arr[], int n) {
        build(arr, n);
    }

    explicit MergeSortTree(const vector<T>& v) {
        build(v);
    }


    // O(NlogN)
    void build(const T arr[], int n) {
        N = n;
        tree.resize(n * 4);
        buildSub(arr, 0, n - 1, 0);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }


    // O((logN)^2), inclusive (0 <= left <= right < N)
    int countLessThanOrEqual(int left, int right, T val) const {
        return countLessThanOrEqualSub(left, right, val, 0, 0, N - 1);
    }

    int countLessThanOrEqual(T val) const {
        return countLessThanOrEqualSub(0, N - 1, val, 0, 0, N - 1);
    }

    // count a value k, O((logN)^2), inclusive (0 <= left <= right < N)
    int count(int left, int right, T val) const {
        return countLessThanOrEqual(left, right, val) - countLessThanOrEqual(left, right, val - 1);
    }

    int count(T val) const {
        return countLessThanOrEqual(val) - countLessThanOrEqual(val - 1);
    }

    // count a value k, O((logN)^2), inclusive (0 <= left <= right < N)
    int count(int left, int right, T valLow, T valHigh) const {
        return countLessThanOrEqual(left, right, valHigh) - countLessThanOrEqual(left, right, valLow - 1);
    }

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

    // O((logN)^2 * logA), inclusive (0 <= k < N)
    T kth(int k) const {
        return tree[0][k];
    }


private:
    // inclusive
    void buildSub(const T arr[], int left, int right, int node) {
        if (left == right) {
            tree[node].push_back(arr[left]);
            return;
        }

        int mid = left + (right - left) / 2;
        buildSub(arr, left, mid, node * 2 + 1);
        buildSub(arr, mid + 1, right, node * 2 + 2);
        merge(tree[2 * node + 1].begin(), tree[2 * node + 1].end(),
              tree[2 * node + 2].begin(), tree[2 * node + 2].end(),
              back_inserter(tree[node]));
    }

    int countLessThanOrEqualSub(int left, int right, T k, int node, int nodeLeft, int nodeRight) const {
        if (right < nodeLeft || nodeRight < left)
            return 0;

        if (left <= nodeLeft && nodeRight <= right)
            return int(upper_bound(tree[node].begin(), tree[node].end(), k) - tree[node].begin());

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        return countLessThanOrEqualSub(left, right, k, 2 * node + 1, nodeLeft, mid)
             + countLessThanOrEqualSub(left, right, k, 2 * node + 2, mid + 1, nodeRight);
    }
};
