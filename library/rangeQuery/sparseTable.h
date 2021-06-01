#pragma once

#include <vector>
#include <functional>

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename MergeOp = function<T(T,T)>>
struct SparseTable {
    int                 N;
    vector<vector<T>>   value;
    vector<int>         H;
    MergeOp             mergeOp;
    T                   defaultValue;

    explicit SparseTable(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    SparseTable(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    SparseTable(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    SparseTable(SparseTable&& rhs)
        : N(rhs.N), value(std::move(rhs.value)), H(std::move(rhs.H)),
          mergeOp(std::move(rhs.mergeOp)), defaultValue(rhs.defaultValue) {
    }


    void build(const T a[], int n) {
        this->N = n;

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<T>(n, defaultValue));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1, step = 1; i < int(value.size()); i++, step <<= 1) {
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

        int level = H[right - left];
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
            int level = H[lastBit];
            res = mergeOp(res, value[level][left]);

            left += lastBit;
            rangeSize -= lastBit;
        }

        return res;
    }
};

template <typename T, typename MergeOp>
inline SparseTable<T, MergeOp> makeSparseTable(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return SparseTable<T, MergeOp>(arr, op, dfltValue);
}

template <typename T, typename MergeOp>
inline SparseTable<T, MergeOp> makeSparseTable(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return SparseTable<T, MergeOp>(arr, size, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeSparseTable<int>(v, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, right);

    2) Max Sparse Table
        auto sparseTable = makeSparseTable<int>(v, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, right);

    3) GCD Sparse Table
        auto sparseTable = makeSparseTable<int>(v, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, right);

    4) Sum Sparse Table
        auto sparseTable = makeSparseTable<int>(v, [](int a, int b) { return a + b; });
        ...
        sparseTable.queryNoOverlap(left, right);
*/
