#pragma once

#include <vector>

//--------- XOR Fenwick Tree (Binary Indexed Tree) ----------------------------

// for xor from 0 to pos
template <typename T>
struct FenwickTreeXor {
    vector<T> tree;

    FenwickTreeXor(int n) : tree(n + 1) {
        // no action
    }

    void clear() {
        fill(tree.begin(), tree.end(), 0);
    }

    void init(T arr[], int n) {
        for (int i = 0; i < n; i++)
            add(i, arr[i]);
    }

    void init(vector<T>& v) {
        for (int i = 0; i < (int)v.size(); i++)
            add(i, v[i]);
    }

    void add(int pos, T val) {
        pos++;

        while (pos < (int)tree.size()) {
            tree[pos] ^= val;
            pos += pos & -pos;      // add lowest bit
        }
    }

    // inclusive
    void addRange(int left, int right, T val) {
        add(left, val);
        if (right + 1 < (int)tree.size() - 1)
            add(right + 1, ~val);
    }

    // xor from 0 to pos
    T query(int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res ^= tree[pos];
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive
    T queryRange(int left, int right) const {
        T res = query(right);
        if (left > 0)
            res ^= query(left - 1);
        return res;
    }


    T get(int pos) const {
        T res = tree[pos + 1];
        if (pos > 0) {
            int lca = pos & (pos + 1);
            for (pos; pos != lca; pos &= pos - 1)
                res ^= tree[pos];
        }

        return res;
    }

    void set(int pos, T val) {
        add(pos, val ^ get(pos));
    }
};
