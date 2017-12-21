#pragma once

//--------- Fenwick Tree for Max ----------------------------------------------

// for sum from 0 to pos
template <typename T>
struct FenwickTreeMax {
    vector<T> tree;

    FenwickTreeMax(int n, T initValue = T()) : tree(n + 1, initValue) {
        // no action
    }

    void clear(T initValue = T()) {
        fill(tree.begin(), tree.end(), initValue);
    }

    void init(T arr[], int n) {
        for (int i = 0; i < n; i++)
            set(i, arr[i]);
    }

    void init(vector<T>& v) {
        for (int i = 0; i < (int)v.size(); i++)
            set(i, v[i]);
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

    void set(int pos, T val) {
        pos++;

        while (pos < (int)tree.size()) {
            tree[pos] = max(tree[pos], val);
            pos += pos & -pos;      // add lowest bit
        }
    }
};
