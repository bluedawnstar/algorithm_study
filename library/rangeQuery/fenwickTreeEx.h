#pragma once

//--------- Extended Fenwick Tree ----------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct FenwickTreeEx {
    vector<T> tree;

    T         defaultValue;
    BinOp     mergeOp;

    explicit FenwickTreeEx(BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
    }

    FenwickTreeEx(int n, BinOp op, T dflt = T())
        : tree(n + 1, dflt), mergeOp(op), defaultValue(dflt) {
    }

    FenwickTreeEx(T value, int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    FenwickTreeEx(const T arr[], int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    FenwickTreeEx(const vector<T>& v, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    void init(int n) {
        tree.assign(n + 1, defaultValue);
    }

    void build(T value, int n) {
        init(n);
        for (int i = 0; i < n; i++)
            update(i, value);
    }

    void build(const T arr[], int n) {
        init(n);
        for (int i = 0; i < n; i++)
            update(i, arr[i]);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }


    void update(int pos, T val) {
        pos++;

        while (pos < (int)tree.size()) {
            tree[pos] = mergeOp(tree[pos], val);
            pos += pos & -pos;      // add lowest bit
        }
    }

    // [0..right], inclusive (0 <= right < N)
    T query(int right) const {
        right++;

        T res = defaultValue;
        while (right > 0) {
            res = mergeOp(res, tree[right]);
            right &= right - 1;
        }

        return res;
    }
};

template <typename T, typename BinOp>
FenwickTreeEx<T, BinOp> makeFenwickTreeEx(int size, BinOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, BinOp>(size, op, dfltValue);
}

template <typename T, typename BinOp>
FenwickTreeEx<T, BinOp> makeFenwickTreeEx(T value, int size, BinOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, BinOp>(value, size, op, dfltValue);
}

template <typename T, typename BinOp>
FenwickTreeEx<T, BinOp> makeFenwickTreeEx(const T arr[], int size, BinOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, BinOp>(arr, size, op, dfltValue);
}

template <typename T, typename BinOp>
FenwickTreeEx<T, BinOp> makeFenwickTreeEx(const vector<T>& v, BinOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, BinOp>(v, op, dfltValue);
}
