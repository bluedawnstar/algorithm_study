#pragma once

#include <vector>

//--------- Fenwick Tree (Binary Indexed Tree) --------------------------------

// for sum from 0 to pos
template <typename T>
struct FenwickTree {
    vector<T> tree;

    FenwickTree(int n) : tree(n + 1) {
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

    // sum from 0 to pos
    T sum(int pos) const {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += tree[pos];
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive
    T sumRange(int left, int right) const {
        T res = sum(right);
        if (left > 0)
            res -= sum(left - 1);
        return res;
    }

    void add(int pos, T val) {
        pos++;

        while (pos < (int)tree.size()) {
            tree[pos] += val;
            pos += pos & -pos;      // add lowest bit
        }
    }

    // inclusive
    void addRange(int left, int right, T val) {
        add(left, val);
        if (right + 1 < (int)tree.size() - 1)
            add(right + 1, -val);
    }


    T get(int pos) const {
        T res = tree[pos + 1];
        if (pos > 0) {
            int lca = pos & (pos + 1);
            for (pos; pos != lca; pos &= pos - 1)
                res -= tree[pos];
        }

        return res;
    }

    void set(int pos, T val) {
        add(pos, val - get(pos));
    }

    // returns min(i | sum[0,i] >= sum)
    int lowerBound(T sum) const {
        --sum;

        int N = (int)tree.size() - 1;
        int pos = 0;

        int blockSize = N;
        while (blockSize & (blockSize - 1))
            blockSize &= blockSize - 1;

        for (; blockSize > 0; blockSize >>= 1) {
            int nextPos = pos + blockSize;
            if (nextPos < N && sum >= tree[nextPos]) {
                sum -= tree[nextPos];
                pos = nextPos;
            }
        }

        return pos;
    }
};
