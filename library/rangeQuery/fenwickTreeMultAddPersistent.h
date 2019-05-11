#pragma once


#include <vector>
#include "fenwickTreePersistent.h"

//--------- Persistent Fenwick Tree Mult & Add (Incremental Sum) --------------

// - for sum from 0 to pos
// - f(x) = mul * x + add
template <typename T>
struct PersistentFenwickTreeMultAdd {
    PartiallyPersistentFenwickTree<T> mulT;
    PartiallyPersistentFenwickTree<T> addT;

    PersistentFenwickTreeMultAdd() {
    }

    explicit PersistentFenwickTreeMultAdd(int n) : mulT(n), addT(n) {
    }

    void init(int n) {
        mulT.init(n);
        addT.init(n);
    }

    int add(int x, T d) {
        mulT.add(x, d);
        return addT.add(x, d * (1 - x));
    }

    // (inclusive, inclusive)
    int addRange(int left, int right, T d) {
        add(left, d);
        return add(right + 1, -d);
    }

    T sum(int x) const {
        return addT.sum(x) + mulT.sum(x) * x;
    }

    T sum(int time, int x) const {
        return addT.sum(time, x) + mulT.sum(time, x) * x;
    }

    // (inclusive, inclusive)
    T sumRange(int time, int left, int right) const {
        return sum(time, right) - sum(time, left - 1);
    }
};

// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[0,i] >= sum)
template <typename T>
inline int findFirst(const PersistentFenwickTreeMultAdd<T>& ft, int left, int right, T sum) {
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

template <typename T>
inline int findFirst(const PersistentFenwickTreeMultAdd<T>& ft, int time, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(time, left, mid) < sum)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return lo;
}

// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[i,N-1] < sum)
template <typename T>
inline int findLast(const PersistentFenwickTreeMultAdd<T>& ft, int left, int right, T sum) {
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

template <typename T>
inline int findLast(const PersistentFenwickTreeMultAdd<T>& ft, int time, int left, int right, T sum) {
    int lo = left, hi = right;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (ft.sumRange(time, mid, right) < sum)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    return lo;
}
