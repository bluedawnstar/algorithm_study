#pragma once

// for sum from 0 to pos
template <typename T, int mod = 1'000'000'007>
struct FenwickTreeMod {
    vector<T> tree;

    FenwickTreeMod() {
    }

    explicit FenwickTreeMod(int n) : tree(n + 1) {
    }


    void init(int n) {
        tree = vector<T>(n + 1);
    }


    // sum from 0 to pos
    // O(logN)
    T sum(int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += tree[pos];
            if (res >= mod)
                res -= mod;
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive, O(logN)
    T sumRange(int left, int right) const {
        T res = sum(right);
        if (left > 0) {
            res -= sum(left - 1);
            if (res < 0)
                res += mod;
        }
        return res;
    }

    // O(logN)
    void add(int pos, T val) {
        pos++;

        while (pos < int(tree.size())) {
            tree[pos] += val;
            if (tree[pos] >= mod)
                tree[pos] -= mod;
            pos += pos & -pos;      // add lowest bit
        }
    }
};
