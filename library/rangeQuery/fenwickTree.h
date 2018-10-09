#pragma once

#include <vector>

//--------- Fenwick Tree (Binary Indexed Tree) --------------------------------

// for sum from 0 to pos
template <typename T>
struct FenwickTree {
    vector<T> tree;

    FenwickTree() {
    }

    explicit FenwickTree(int n) : tree(n + 1) {
    }

    FenwickTree(const T value, int n) {
        build(value, n);
    }

    FenwickTree(const T arr[], int n) {
        build(arr, n);
    }

    FenwickTree(const vector<T>& v) {
        build(v);
    }


    void init(int n) {
        tree = vector<int>(n + 1);
    }

    // O(NlogN)
    void build(T value, int n) {
        init(n);
        for (int i = 0; i < n; i++)
            add(i, value);
    }

    // O(NlogN)
    void build(const T arr[], int n) {
        init(n);
        for (int i = 0; i < n; i++)
            add(i, arr[i]);
    }

    // O(NlogN)
    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    // sum from 0 to pos
    // O(logN)
    T sum(int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += tree[pos];
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive, O(logN)
    T sumRange(int left, int right) const {
        T res = sum(right);
        if (left > 0)
            res -= sum(left - 1);
        return res;
    }

    // O(logN)
    void add(int pos, T val) {
        pos++;

        while (pos <= int(tree.size())) {
            tree[pos] += val;
            pos += pos & -pos;      // add lowest bit
        }
    }

    // inclusive, O(logN)
    void addRange(int left, int right, T val) {
        add(left, val);
        if (right + 1 < int(tree.size()) - 1)
            add(right + 1, -val);
    }

    // O(logN)
    T get(int pos) const {
        T res = tree[pos + 1];
        if (pos > 0) {
            int lca = pos & (pos + 1);
            for (pos; pos != lca; pos &= pos - 1)
                res -= tree[pos];
        }

        return res;
    }

    // O(logN)
    void set(int pos, T val) {
        add(pos, val - get(pos));
    }
};

// PRECONDITION: tree's values are monotonically increasing
// returns min(i | sum[0, i] >= sum)
// O(logN)
template <typename T>
inline int lowerBound(const FenwickTree<T>& ft, T sum) {
    --sum;

    int N = int(ft.tree.size()) - 1;
    int pos = 0;

    int blockSize = N;
    while (blockSize & (blockSize - 1))
        blockSize &= blockSize - 1;

    for (; blockSize > 0; blockSize >>= 1) {
        int nextPos = pos + blockSize;
        if (nextPos < N && sum >= ft.tree[nextPos]) {
            sum -= ft.tree[nextPos];
            pos = nextPos;
        }
    }

    return pos;
}

// PRECONDITION: tree's values are monotonically increasing
// returns min(i | sum[left, i] >= sum)
// O((logN)^2)
template <typename T>
inline int findFirst(const FenwickTree<T>& ft, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(left, mid) < sum)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return lo;
}

// PRECONDITION: tree's values are monotonically increasing
// returns min(i | sum[i, right] < sum)
// O((logN)^2)
template <typename T>
inline int findLast(const FenwickTree<T>& ft, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(mid, right) < sum)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}
