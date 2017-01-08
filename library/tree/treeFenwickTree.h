#pragma once

#include <vector>

//--------- Fenwick Tree ------------------------------------------------------

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

    void init(vector<T>& v, int n) {
        for (int i = 0; i < n; i++)
            add(i, v[i]);
    }

    // sum from 0 to pos
    T sum(int pos) {
        pos++;

        T res = 0;
        while (pos > 0) {
            res += tree[pos];
            pos &= pos - 1;         // clear lowest bit
        }

        return res;
    }

    // inclusive
    T sumRange(int left, int right) {
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
};

//--------- Fenwick Tree 2D ---------------------------------------------------

template <typename T>
struct FenwickTree2D {
    vector<vector<T>> tree;

    FenwickTree2D(int rowN, int colN) : tree(rowN + 1, vector<T>(colN + 1)) {
        // no action
    }

    void clear() {
        for (int i = 0; i < (int)tree.size(); i++)
            fill(tree[i].begin(), tree[i].end(), 0);
    }

    // to initialize from (0, 0)
    void init(int row, int col, T val) {
        T v = sum(row, col);
        if (row > 0)
            v -= sum(row - 1, col);
        if (col > 0)
            v -= sum(row, col - 1);
        if (row > 0 && col > 0)
            v += sum(row - 1, col - 1);
        add(row, col, val - v);
    }

    // to initialize from (N - 1, M - 1)
    void initRev(int row, int col, T val) {
        add(row, col, val);
    }


    T sum(int row, int col) {
        row++;
        col++;

        T res = 0;
        for (int r = row; r > 0; r &= r - 1) {
            for (int c = col; c > 0; c &= c - 1) {
                res += tree[r][c];
            }
        }

        return res;
    }

    T sumRange(int row1, int col1, int row2, int col2) {
        T res = sum(row2, col2);
        if (row1 > 0)
            res -= sum(row1 - 1, col2);
        if (col1 > 0)
            res -= sum(row2, col1 - 1);
        if (row1 > 0 && col1 > 0)
            res += sum(row1 - 1, col1 - 1);
        return res;
    }

    void add(int row, int col, T val) {
        row++;
        col++;

        for (int r = row; r < (int)tree.size(); r += r & -r) {
            for (int c = col; c < (int)tree[row].size(); c += c & -c) {
                tree[r][c] += val;
            }
        }
    }
};

//--------- Fenwick Tree Mult & Add (Incremental Sum) -------------------------

// - for sum from 0 to pos
// - f(x) = mul * x + add
template <typename T>
struct FenwickTreeMultAdd {
    FenwickTree<T> mulT;
    FenwickTree<T> addT;

    FenwickTreeMultAdd(int n)
        : mulT(n), addT(n) {
    }

    void add(int x, T d) {
        mulT.add(x, d);
        addT.add(x, d * (1 - x));
    }

    // (inclusive, exclusive)
    void addRange(int left, int right, T d) {
        add(left, d);
        add(right, -d);
    }

    T sum(int x) {
        return addT.sum(x) + mulT.sum(x) * x;
    }

    // (inclusive, exclusive)
    T sumRange(int left, int right) {
        return sum(right - 1) - sum(left - 1);
    }
};
