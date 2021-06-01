#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include <vector>
#include <functional>

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct FastSparseTable {
    int                 N;
    int                 H;
    vector<vector<T>>   value;
    MergeOp             mergeOp;
    T                   defaultValue;

    explicit FastSparseTable(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    FastSparseTable(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    FastSparseTable(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    FastSparseTable(FastSparseTable&& rhs)
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

        for (int i = 1, step = 1; i < H; i++, step <<= 1) {
            for (int j = 0; j < n; j++) {
                if (j + step < n)
                    value[i][j] = mergeOp(value[i - 1][j], value[i - 1][j + step]);
                else
                    value[i][j] = value[i - 1][j];
            }
        }
    }

    void build(const vector<T>& a) {
        build(&a[0], int(a.size()));
    }


    // O(1), inclusive
    T query(int left, int right) const {
        right++;
        if (left >= right)
            return defaultValue;

#ifndef __GNUC__
        int level = 31 - _lzcnt_u32(static_cast<unsigned int>(right - left));
#else
        int level = 31 - __builtin_clz(static_cast<unsigned int>(right - left));
#endif
        return mergeOp(value[level][left], value[level][right - (1 << level)]);
    }

    // O(log(right - left + 1)), inclusive
    T queryNoOverlap(int left, int right) const {
        int rangeSize = right - left + 1;
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
            res = mergeOp(res, value[level][left]);

            left += lastBit;
            rangeSize -= lastBit;
        }

        return res;
    }
};

template <typename T, typename MergeOp>
inline FastSparseTable<T, MergeOp> makeFastSparseTable(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return FastSparseTable<T, MergeOp>(arr, op, dfltValue);
}

template <typename T, typename MergeOp>
inline FastSparseTable<T, MergeOp> makeFastSparseTable(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return FastSparseTable<T, MergeOp>(arr, size, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeFastSparseTable<int>(v, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, right);

    2) Max Sparse Table
        auto sparseTable = makeFastSparseTable<int>(v, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, right);

    3) GCD Sparse Table
        auto sparseTable = makeFastSparseTable<int>(v, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, right);

    4) Sum Sparse Table
        auto sparseTable = makeFastSparseTable<int>(v, [](int a, int b) { return a + b; });
        ...
        sparseTable.queryNoOverlap(left, right);
*/
