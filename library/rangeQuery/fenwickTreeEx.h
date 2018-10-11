#pragma once

//--------- Extended Fenwick Tree ----------------------------------------------

template <typename T, typename MergeOp = function<T(T, T)>>
struct FenwickTreeEx {
    vector<T> tree;

    MergeOp   mergeOp;
    T         defaultValue;

    explicit FenwickTreeEx(MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
    }

    FenwickTreeEx(int n, MergeOp op, T dflt = T())
        : tree(n + 1, dflt), mergeOp(op), defaultValue(dflt) {
    }

    FenwickTreeEx(T value, int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    FenwickTreeEx(const T arr[], int n, MergeOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    FenwickTreeEx(const vector<T>& v, MergeOp op, T dflt = T())
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
        build(&v[0], int(v.size()));
    }


    void update(int pos, T val) {
        pos++;

        while (pos <= int(tree.size())) {
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

template <typename T, typename MergeOp>
FenwickTreeEx<T, MergeOp> makeFenwickTreeEx(int size, MergeOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, MergeOp>(size, op, dfltValue);
}

template <typename T, typename MergeOp>
FenwickTreeEx<T, MergeOp> makeFenwickTreeEx(T value, int size, MergeOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, MergeOp>(value, size, op, dfltValue);
}

template <typename T, typename MergeOp>
FenwickTreeEx<T, MergeOp> makeFenwickTreeEx(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, MergeOp>(arr, size, op, dfltValue);
}

template <typename T, typename MergeOp>
FenwickTreeEx<T, MergeOp> makeFenwickTreeEx(const vector<T>& v, MergeOp op, T dfltValue = T()) {
    return FenwickTreeEx<T, MergeOp>(v, op, dfltValue);
}
