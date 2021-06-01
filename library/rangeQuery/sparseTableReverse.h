#pragma once

#include <vector>
#include <functional>

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct ReverseSparseTable {
    int                 N;
    vector<vector<T>>   value;
    vector<int>         H;
    MergeOp             mergeOp;
    T                   defaultValue;

    explicit ReverseSparseTable(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    ReverseSparseTable(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    ReverseSparseTable(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    ReverseSparseTable(ReverseSparseTable&& rhs)
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

        for (int i = 1; i < n; i++) {
            int level = H[i + 1];
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

        int level = H[right - left];
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
            int level = H[lastBit];

            left += lastBit;
            res = mergeOp(res, value[level][left]);

            rangeSize -= lastBit;
        }

        return res;
    }
};

template <typename T, typename MergeOp>
inline ReverseSparseTable<T, MergeOp> makeReverseSparseTable(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return ReverseSparseTable<T, MergeOp>(arr, op, dfltValue);
}

template <typename T, typename MergeOp>
inline ReverseSparseTable<T, MergeOp> makeReverseSparseTable(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return ReverseSparseTable<T, MergeOp>(arr, size, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeReverseSparseTable<int>(v, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, right);

    2) Max Sparse Table
        auto sparseTable = makeReverseSparseTable<int>(v, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, right);

    3) GCD Sparse Table
        auto sparseTable = makeReverseSparseTable<int>(v, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, right);

    4) Sum Sparse Table
        auto sparseTable = makeReverseSparseTable<int>(v, [](int a, int b) { return a + b; });
        ...
        sparseTable.queryNoOverlap(left, right);
*/
