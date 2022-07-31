#pragma once

#include <vector>

//--------- Fenwick Tree (Binary Indexed Tree) --------------------------------

/*
  1. use case #1 : point update, point/range query
    1) point update
       add(pos1, x1);
       add(pos2, x2);
       ...

    2) point query
       get(pos) -> return the value of index pos

    3) range query
       sum(pos)              -> return sum of [0, pos]
       sumRange(left, right) -> return sum of [left, right]

  2. use case #2 : range update, point query
    1) range update
       addRange(left1, right1, x1);
       addRange(left2, right2, x2);
       ...

    2) point query
       sum(pos) -> return the value of index pos (not range)

    3) range query
       Not working!!!
 */

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
        tree = vector<T>(n + 1);
    }


    // O(N)
    void build(T value, int n) {
        tree = vector<T>(n + 1, value);
        tree[0] = T(0);
        for (int step = 2; step <= n; step <<= 1) {
            for (int i = step >> 1, j = step; j <= n; i += step, j += step)
                tree[j] += tree[i];
        }
    }

    // O(N)
    void build(const T arr[], int n) {
        tree.clear();
        tree.push_back(T(0));
        tree.insert(tree.end(), arr, arr + n);
        for (int step = 2; step <= n; step <<= 1) {
            for (int i = step >> 1, j = step; j <= n; i += step, j += step)
                tree[j] += tree[i];
        }
    }

    // O(N)
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

        while (pos < int(tree.size())) {
            tree[pos] += val;
            pos += pos & -pos;      // add lowest bit
        }
    }

    // inclusive, O(logN)
    // [CAUTION] This is not a general range update.
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
            for (; pos != lca; pos &= pos - 1)
                res -= tree[pos];
        }

        return res;
    }

    // O(logN)
    void set(int pos, T val) {
        add(pos, val - get(pos));
    }

    //--- lower bound

    // PRECONDITION: tree's values are monotonically increasing and positive (tree[i] >= 0)
    // returns min(i | sum[0, i] >= sum)
    // O(logN)
    int lowerBound(T sum) {
        int N = int(tree.size()) - 1;

        int blockSize = N;
        while (blockSize & (blockSize - 1))
            blockSize &= blockSize - 1;

        int lo = 0;
        for (; blockSize > 0; blockSize >>= 1) {
            int next = lo + blockSize;
            if (next <= N && sum >= tree[next]) {
                sum -= tree[next];
                lo = next;
            }
        }

        return lo - (sum == 0);
    }
};


// PRECONDITION: tree's values are monotonically increasing (ex: positive values)
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

// PRECONDITION: tree's values are monotonically increasing (ex: positive values)
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
