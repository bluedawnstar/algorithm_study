#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include <vector>
#include <functional>

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct FastReverseSparseTable {
    int               N;
    int               H;
    vector<vector<T>> value;
    MergeOp           mergeOp;
    T                 defaultValue;

    explicit FastReverseSparseTable(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    FastReverseSparseTable(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    FastReverseSparseTable(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    FastReverseSparseTable(FastReverseSparseTable&& rhs)
        : N(rhs.N), value(std::move(rhs.value)), H(std::move(rhs.H)),
        mergeOp(std::move(rhs.mergeOp)), defaultValue(rhs.defaultValue) {
    }


    void build(const T a[], int n) {
        this->N = n;

#ifndef __GNUC__
        H = 32 - _lzcnt_u32((N << 1) - 1);
#else
        H = 32 - __builtin_clz((N << 1) - 1);
#endif

        value.assign(H, vector<T>(n, defaultValue));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < n; i++) {
#ifndef __GNUC__
            int level = 31 - _lzcnt_u32(static_cast<unsigned int>(i + 1));
#else
            int level = 31 - __builtin_clz(static_cast<unsigned int>(i + 1));
#endif
            for (int j = 0; j < level; j++)
                value[j + 1][i] = mergeOp(value[j][i - (1 << j)], value[j][i]);
        }
    }

    void build(const vector<T>& a) {
        build(&a[0], int(a.size()));
    }


    // O(1), inclusive
    T query(int left, int right) const {
        left--;
        if (right <= left)
            return defaultValue;

#ifndef __GNUC__
        int level = 31 - _lzcnt_u32(static_cast<unsigned int>(right - left));
#else
        int level = 31 - __builtin_clz(static_cast<unsigned int>(right - left));
#endif
        return mergeOp(value[level][left + (1 << level)], value[level][right]);
    }

    // O(log(right - left + 1)), inclusive
    T queryNoOverlap(int left, int right) const {
        int rangeSize = right - --left;
        if (rangeSize <= 0)
            return defaultValue;

        T res = defaultValue;
        while (rangeSize > 0) {
            int lastBit = rangeSize & -rangeSize;
#ifndef __GNUC__
            int level = int(_tzcnt_u32(lastBit));
#else
            int level = __builtin_ctz(lastBit);
#endif

            left += lastBit;
            res = mergeOp(res, value[level][left]);

            rangeSize -= lastBit;
        }

        return res;
    }
};

template <typename T, typename MergeOp>
inline FastReverseSparseTable<T, MergeOp> makeFastReverseSparseTable(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return FastReverseSparseTable<T, MergeOp>(arr, op, dfltValue);
}

template <typename T, typename MergeOp>
inline FastReverseSparseTable<T, MergeOp> makeFastReverseSparseTable(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return FastReverseSparseTable<T, MergeOp>(arr, size, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeFastReverseSparseTable<int>(v, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, right);

    2) Max Sparse Table
        auto sparseTable = makeFastReverseSparseTable<int>(v, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, right);

    3) GCD Sparse Table
        auto sparseTable = makeFastReverseSparseTable<int>(v, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, right);

    4) Sum Sparse Table
        auto sparseTable = makeFastReverseSparseTable<int>(v, [](int a, int b) { return a + b; });
        ...
        sparseTable.queryNoOverlap(left, right);
*/
