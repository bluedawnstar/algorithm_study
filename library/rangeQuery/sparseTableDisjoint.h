#pragma once

#include <vector>
#include <functional>

//--------- Disjoint Sparse Table ---------------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct DisjointSparseTable {
    int                 RealN;
    int                 N;
    vector<vector<T>>   value;
    vector<int>         H;
    MergeOp             mergeOp;
    T                   defaultValue;

    explicit DisjointSparseTable(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    DisjointSparseTable(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    DisjointSparseTable(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }


    // O(NlogN)
    void build(const T a[], int n) {
        RealN = n;
        N = 1;
        while (N < n)
            N <<= 1;

        H.resize(N);
        H[1] = 0;
        for (int i = 2; i < N; i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<T>(N, defaultValue));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int h = 1, range = 4; range <= N; h++, range <<= 1) {
            int half = range >> 1;
            for (int i = half; i < N; i += range) {
                value[h][i - 1] = value[0][i - 1];
                for (int j = i - 2; j >= i - half; j--)
                    value[h][j] = mergeOp(value[h][j + 1], value[0][j]);

                value[h][i] = value[0][i];
                for (int j = i + 1; j < i + half; j++)
                    value[h][j] = mergeOp(value[h][j - 1], value[0][j]);
            }
        }
    }

    void build(const vector<T>& a) {
        build(&a[0], int(a.size()));
    }


    // O(1), inclusive
    T query(int left, int right) const {
        if (left == right)
            return value[0][left];

        int h = H[left ^ right];
        return mergeOp(value[h][left], value[h][right]);
    }

    // O(N)
    void update(int index, T x) {
        value[0][index] = x;
        for (int h = 1, range = 4; range <= N; h++, range <<= 1) {
            int half = range >> 1;
            int start = index & ~(range - 1);
            if ((index & half) == 0) { // suffix
                value[h][index] = (index + 1 < (start | half)) ? mergeOp(value[h][index + 1], x) : x;
                for (int i = index - 1; i >= start; i--)
                    value[h][i] = mergeOp(value[h][i + 1], value[0][i]);
            } else { // prefix
                value[h][index] = (index > (start | half)) ? mergeOp(value[h][index - 1], x) : x;
                for (int i = index + 1, next = start + range; i < next; i++)
                    value[h][i] = mergeOp(value[h][i - 1], value[0][i]);
            }
        }
    }
};

template <typename T, typename MergeOp>
inline DisjointSparseTable<T, MergeOp> makeDisjointSparseTable(MergeOp op, T dfltValue = T()) {
    return DisjointSparseTable<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline DisjointSparseTable<T, MergeOp> makeDisjointSparseTable(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return DisjointSparseTable<T, MergeOp>(arr, size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline DisjointSparseTable<T, MergeOp> makeDisjointSparseTable(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return DisjointSparseTable<T, MergeOp>(arr, op, dfltValue);
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeDisjointSparseTable<int>(v, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sparseTable.query(left, right);

    2) Max Sparse Table
        auto sparseTable = makeDisjointSparseTable<int>(v, [](int a, int b) { return max(a, b); });
        ...
        sparseTable.query(left, right);

    3) GCD Sparse Table
        auto sparseTable = makeDisjointSparseTable<int>(v, [](int a, int b) { return gcd(a, b); });
        ...
        sparseTable.query(left, right);

    4) Sum Sparse Table
        auto sparseTable = makeDisjointSparseTable<int>(v, [](int a, int b) { return a + b; });
        ...
        sparseTable.query(left, right);
*/
