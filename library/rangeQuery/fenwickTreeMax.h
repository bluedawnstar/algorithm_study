#pragma once

//--------- Fenwick Tree for Max ----------------------------------------------

template <typename T>
struct FenwickTreeMax {
    vector<T>   tree;
    T           defaultValue;

    explicit FenwickTreeMax(T dflt) {
    }

    FenwickTreeMax(int n, T dflt) : tree(n + 1, dflt) {
    }

    FenwickTreeMax(T value, int n, T dflt) : defaultValue(dflt) {
        build(value, n);
    }

    FenwickTreeMax(const T arr[], int n, T dflt) : defaultValue(dflt) {
        build(arr, n);
    }

    explicit FenwickTreeMax(const vector<T>& v, T dflt) : defaultValue(dflt) {
        build(v);
    }


    void init(int n) {
        tree.assign(n + 1, defaultValue);
    }

    void build(T value, int n) {
        init(n);
        for (int i = 0; i < n; i++)
            set(i, value);
    }

    void build(const T arr[], int n) {
        init(n);
        for (int i = 0; i < n; i++)
            set(i, arr[i]);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }


    // max[0..pos]
    T get(int pos) const {
        pos++;

        T res = numeric_limits<T>::min();
        while (pos > 0) {
            res = max(res, tree[pos]);
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

private:
    void set(int pos, T val) {
        pos++;

        while (pos < (int)tree.size()) {
            tree[pos] = max(tree[pos], val);
            pos += pos & -pos;      // add lowest bit
        }
    }
};
