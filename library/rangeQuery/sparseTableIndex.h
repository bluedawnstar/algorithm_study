#pragma once

#include <vector>
#include <functional>

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct SparseTableIndex {
    int                 N;
    vector<vector<int>> value;          // index to 'in'
    vector<int>         H;
    MergeOp             mergeOp;
    T                   defaultValue;

    vector<T>           in;

    explicit SparseTableIndex(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    SparseTableIndex(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    SparseTableIndex(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    SparseTableIndex(SparseTableIndex&& rhs)
        : N(rhs.N), value(std::move(rhs.value)), H(std::move(rhs.H)),
        mergeOp(std::move(rhs.mergeOp)), defaultValue(rhs.defaultValue), in(std::move(rhs.in)) {
    }


    void build(const T a[], int n) {
        N = n;

        in.resize(n);
        for (int i = 0; i < n; i++)
            in[i] = a[i];

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<T>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < int(value.size()); i++) {
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

        int k = H[right - left];

        int a = value[k][left];
        int b = value[k][right - (1 << k)];
        return (mergeOp(in[a], in[b]) == in[a]) ? a : b;
    }

    // O(log(right - left + 1)), inclusive
    int queryNoOverlap(int left, int right) const {
        int rangeSize = right - left + 1;
        if (rangeSize <= 0)
            return -1;

        T val = defaultValue;
        int res = -1;
        while (rangeSize) {
#ifndef __GNUC__
            int i = int(_tzcnt_u32(rangeSize));
#else
            int i = __builtin_ctz(rangeSize);
#endif
            //int i = H[rangeSize & -rangeSize];

            int idx = value[i][left];
            val = mergeOp(val, in[idx]);
            if (val == in[idx])
                res = idx;

            left += rangeSize & -rangeSize;
            rangeSize &= rangeSize - 1;
        }

        return res;
    }
};

template <typename T, typename MergeOp>
inline SparseTableIndex<T, MergeOp> makeSparseTableIndex(const vector<T>& arr, int size, MergeOp op, T dfltValue = T()) {
    return SparseTableIndex<T, MergeOp>(arr, size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline SparseTableIndex<T, MergeOp> makeSparseTableIndex(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return SparseTableIndex<T, MergeOp>(arr, size, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeSparseTableIndex<int>(v, N, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, right);

    2) Max Sparse Table
        auto sparseTable = makeSparseTableIndex<int>(v, N, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, right);

    3) GCD Sparse Table
        auto sparseTable = makeSparseTableIndex<int>(v, N, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, right);

    4) Sum Sparse Table
        auto sparseTable = makeSparseTableIndex<int>(v, N, [](int a, int b) { return a + b; });
        ...
        sparseTable.queryNoOverlap(left, right);
*/
