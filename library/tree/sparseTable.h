#pragma once

#include <vector>
#include <functional>

//--------- General Sparse Table ----------------------------------------------

template <typename T, typename BinOp = function<T(T,T)>>
struct SparseTable {
    int                 N;
    vector<vector<T>>   value;
    vector<T>           H;
    T                   defaultValue;
    BinOp               mergeOp;

    template <typename U>
    SparseTable(const U& a, int n, T dfltValue = T())
        : mergeOp(), defaultValue(dfltValue) {
        _init(a, n);
    }

    template <typename U>
    SparseTable(const U& a, int n, BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        _init(a, n);
    }

    SparseTable(SparseTable&& rhs)
        : N(rhs.N), value(std::move(rhs.value)), H(std::move(rhs.H)),
          mergeOp(std::move(rhs.mergeOp)), defaultValue(rhs.defaultValue) {
    }

    template <typename U>
    void _init(const U& a, int n) {
        this->N = n;

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < (int)H.size(); i++)
            H[i] = H[i >> 1] + 1;

        value.resize(H.back() + 1, vector<T>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < (int)value.size(); i++) {
            vector<T>& prev = value[i - 1];
            vector<T>& curr = value[i];
            for (int v = 0; v < n; v++) {
                if (v + (1 << (i - 1)) < n)
                    curr[v] = mergeOp(prev[v], prev[v + (1 << (i - 1))]);
                else
                    curr[v] = prev[v];
            }
        }
    }

    // inclusive
    T query(int left, int right) {
        right++;
        if (right <= left)
            return defaultValue;

        int k = H[right - left];
        vector<int>& mink = value[k];
        return mergeOp(mink[left], mink[right - (1 << k)]);
    }
};

template <typename T, typename BinOp>
SparseTable<T, BinOp> makeSparseTable(const vector<T>& arr, int size, BinOp op, T dfltValue = T()) {
    return std::move(SparseTable<T, BinOp>(arr, size, op, dfltValue));
}

template <typename T, typename BinOp>
SparseTable<T, BinOp> makeSparseTable(const T arr[], int size, BinOp op, T dfltValue = T()) {
    return std::move(SparseTable<T, BinOp>(arr, size, op, dfltValue));
}

/* example
    1) Min Sparse Table (RMQ)
        auto sparseTable = makeSparseTable<int>(v, N, [](int a, int b) { return min(a, b); }, INT_MAX);
    2) Max Sparse Table
        auto sparseTable = makeSparseTable<int>(v, N, [](int a, int b) { return max(a, b); });
    3) GCD Sparse Table
        auto sparseTable = makeSparseTable<int>(v, N, [](int a, int b) { return gcd(a, b); });
*/

//--------- RMQ (Range Minimum Query) - Min Sparse Table ----------------------

struct SparseTableMin {
    int N;
    vector<vector<int>> value;
    vector<int> H;

    template <typename U>
    SparseTableMin(const U& a, int n) {
        this->N = n;

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < (int)H.size(); i++)
            H[i] = H[i >> 1] + 1;

        value.resize(H.back() + 1, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1; i < (int)value.size(); i++) {
            vector<int>& prev = value[i - 1];
            vector<int>& curr = value[i];
            for (int v = 0; v < n; v++) {
                if (v + (1 << (i - 1)) < n)
                    curr[v] = min(prev[v], prev[v + (1 << (i - 1))]);
                else
                    curr[v] = prev[v];
            }
        }
    }

    int query(int left, int right) {
        right++;
        if (right <= left)
            return INT_MAX;

        int k = H[right - left];
        vector<int>& mink = value[k];
        return min(mink[left], mink[right - (1 << k)]);
    }
};
