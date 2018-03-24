#pragma once

#include <vector>
#include "fenwickTree.h"

//--------- Fenwick Tree Mult & Add (Incremental Sum) -------------------------

// - for sum from 0 to pos
// - f(x) = mul * x + add
template <typename T>
struct FenwickTreeMultAdd {
    FenwickTree<T> mulT;
    FenwickTree<T> addT;

    FenwickTreeMultAdd() {
    }

    FenwickTreeMultAdd(int n) : mulT(n), addT(n) {
    }


    void init(int n) {
        mulT.build(n);
        addT.build(n);
    }


    void add(int x, T d) {
        mulT.add(x, d);
        addT.add(x, d * (1 - x));
    }

    // (inclusive, inclusive)
    void addRange(int left, int right, T d) {
        add(left, d);
        add(right + 1, -d);
    }

    T sum(int x) const {
        return addT.sum(x) + mulT.sum(x) * x;
    }

    // (inclusive, inclusive)
    T sumRange(int left, int right) const {
        return sum(right) - sum(left - 1);
    }
};

// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[0,i] >= sum)
template <typename T>
inline int findFirst(const FenwickTreeMultAdd<T>& ft, int left, int right, T sum) {
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

// PRECONDITION: tree's values are monotonically increasing or decreasing (positive / negative sum, min, max, gcd, lcm, ...)
// returns min(i | sum[i,N-1] < sum)
template <typename T>
inline int findLast(const FenwickTreeMultAdd<T>& ft, int left, int right, T sum) {
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
