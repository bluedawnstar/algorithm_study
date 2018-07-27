#pragma once

#include <vector>

//--------- Fenwick Tree 2D ---------------------------------------------------

template <typename T>
struct FenwickTree2D {
    vector<vector<T>> tree;

    FenwickTree2D(int rowN, int colN) : tree(rowN + 1, vector<T>(colN + 1)) {
    }

    //--- for initialization

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
    void initReverse(int row, int col, T val) {
        add(row, col, val);
    }

    //---

    T sum(int row, int col) const {
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

    T sumRange(int row1, int col1, int row2, int col2) const {
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

        for (int r = row; r < int(tree.size()); r += r & -r) {
            for (int c = col; c < int(tree[row].size()); c += c & -c) {
                tree[r][c] += val;
            }
        }
    }


    T get(int row, int col) const {
        return sumRange(row, col, row, col);
    }

    void set(int row, int col, T val) {
        add(row, col, val - get(row, col));
    }
};
