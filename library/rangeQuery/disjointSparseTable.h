#pragma once

#include <vector>
#include <functional>

//--------- Disjoint Sparse Table ---------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct DisjointSparseTable {
    int                 RealN;
    int                 N;
    vector<vector<T>>   value;
    vector<int>         H;
    T                   defaultValue;
    BinOp               mergeOp;

    explicit DisjointSparseTable(BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    DisjointSparseTable(const T a[], int n, BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    DisjointSparseTable(const vector<T>& a, BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    DisjointSparseTable(DisjointSparseTable&& rhs)
        : ReadN(rhs.RealN), N(rhs.N), value(std::move(rhs.value)), H(std::move(rhs.H)),
        mergeOp(std::move(rhs.mergeOp)), defaultValue(rhs.defaultValue) {
    }


    void build(const T a[], int n) {
        RealN = n;
        N = 1;
        while (N < n)
            N <<= 1;

        H.resize(N);
        H[1] = 0;
        for (int i = 2; i < N; i++)
            H[i] = H[i >> 1] + 1;

        value.resize(H.back() + 1, vector<T>(N, defaultValue));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        vector<T>& A = value[0];    // range is 2
        for (int h = 1, range = 4; range <= N; h++, range <<= 1) {
            int half = range >> 1;

            vector<T>& curr = value[h];
            for (int i = half; i < N; i += range) {
                curr[i - 1] = A[i - 1];
                for (int j = i - 2; j >= i - half; j--)
                    curr[j] = mergeOp(A[j], curr[j + 1]);

                curr[i] = A[i];
                for (int j = i + 1; j < i + half; j++)
                    curr[j] = mergeOp(curr[j - 1], A[j]);
            }
        }
    }

    void build(const vector<T>& a) {
        build(&a[0], (int)a.size());
    }


    // O(1), inclusive
    T query(int left, int right) const {
        if (left == right)
            return value[0][left];

        int h = H[left ^ right];
        return mergeOp(value[h][left], value[h][right]);
    }
};

template <typename T, typename BinOp>
DisjointSparseTable<T, BinOp> makeDisjointSparseTable(BinOp op, T dfltValue = T()) {
    return DisjointSparseTable<T, BinOp>(op, dfltValue);
}

template <typename T, typename BinOp>
DisjointSparseTable<T, BinOp> makeDisjointSparseTable(const T arr[], int size, BinOp op, T dfltValue = T()) {
    return DisjointSparseTable<T, BinOp>(arr, size, op, dfltValue);
}

template <typename T, typename BinOp>
DisjointSparseTable<T, BinOp> makeDisjointSparseTable(const vector<T>& arr, BinOp op, T dfltValue = T()) {
    return DisjointSparseTable<T, BinOp>(arr, op, dfltValue);
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
