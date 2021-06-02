#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include <vector>
#include <functional>

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct FastSparseTableIndex {
    int                 N;
    int                 H;
    vector<vector<int>> value;          // index to 'in'
    MergeOp             mergeOp;
    T                   defaultValue;

    vector<T>           in;

    explicit FastSparseTableIndex(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    FastSparseTableIndex(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    FastSparseTableIndex(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    FastSparseTableIndex(FastSparseTableIndex&& rhs)
        : N(rhs.N), value(std::move(rhs.value)), H(std::move(rhs.H)),
        mergeOp(std::move(rhs.mergeOp)), defaultValue(rhs.defaultValue), in(std::move(rhs.in)) {
    }


    void build(const T a[], int n) {
        N = n;

#ifndef __GNUC__
        H = 32 - _lzcnt_u32((N << 1) - 1);
#else
        H = 32 - __builtin_clz((N << 1) - 1);
#endif

        in.resize(n);
        for (int i = 0; i < n; i++)
            in[i] = a[i];

        value.assign(H, vector<T>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < H; i++) {
            vector<T>& prev = value[i - 1];
            vector<T>& curr = value[i];
            for (int j = 0; j < n; j++) {
                int j2 = j + (1 << (i - 1));
                if (j2 < n) {
                    T a = in[prev[j]];
                    T b = in[prev[j2]];
                    curr[j] = (mergeOp(a, b) == a) ? prev[j] : prev[j2];
                } else {
                    curr[j] = prev[j];
                }
            }
        }
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    // O(1), inclusive
    // return index
    int query(int left, int right) const {
        right++;
        if (right <= left)
            return -1;

#ifndef __GNUC__
        int level = 31 - _lzcnt_u32(static_cast<unsigned int>(right - left));
#else
        int level = 31 - __builtin_clz(static_cast<unsigned int>(right - left));
#endif

        int a = value[level][left];
        int b = value[level][right - (1 << level)];
        return (mergeOp(in[a], in[b]) == in[a]) ? a : b;
    }

    // O(log(right - left + 1)), inclusive
    int queryNoOverlap(int left, int right) const {
        int rangeSize = right - left + 1;
        if (rangeSize <= 0)
            return -1;

        T val = defaultValue;
        int res = -1;
        while (rangeSize > 0) {
            int lastBit = rangeSize & -rangeSize;
#ifndef __GNUC__
            int level = int(_tzcnt_u32(lastBit));
#else
            int level = __builtin_ctz(lastBit);
#endif

            int idx = value[level][left];
            val = mergeOp(val, in[idx]);
            if (val == in[idx])
                res = idx;

            left += lastBit;
            rangeSize -= lastBit;
        }

        return res;
    }
};

template <typename T, typename MergeOp>
inline FastSparseTableIndex<T, MergeOp> makeFastSparseTableIndex(const vector<T>& arr, int size, MergeOp op, T dfltValue = T()) {
    return FastSparseTableIndex<T, MergeOp>(arr, size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline FastSparseTableIndex<T, MergeOp> makeFastSparseTableIndex(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return FastSparseTableIndex<T, MergeOp>(arr, size, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeFastSparseTableIndex<int>(v, N, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, right);

    2) Max Sparse Table
        auto sparseTable = makeFastSparseTableIndex<int>(v, N, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, right);

    3) GCD Sparse Table
        auto sparseTable = makeFastSparseTableIndex<int>(v, N, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, right);

    4) Sum Sparse Table
        auto sparseTable = makeFastSparseTableIndex<int>(v, N, [](int a, int b) { return a + b; });
        ...
        sparseTable.queryNoOverlap(left, right);
*/
