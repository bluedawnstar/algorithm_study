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

    //---

    // returns min(i | sum[0,i] >= sum)
    int findFirst(int left, int right, T sum) const {
        int lo = left, hi = right;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (sumRange(left, mid) < sum)
                lo = mid + 1;
            else
                hi = mid - 1;
        }

        return lo;
    }

    // returns min(i | sum[i,N-1] < sum)
    int findLast(int left, int right, T sum) const {
        int lo = left, hi = right;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (sumRange(mid, right) < sum)
                hi = mid - 1;
            else
                lo = mid + 1;
        }

        return lo;
    }
};
