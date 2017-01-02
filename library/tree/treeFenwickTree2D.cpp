#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

template <typename T>
struct FenwickTree2D {
    vector<vector<T>> mTree;

    FenwickTree2D(int rowN, int colN) : mTree(rowN + 1, vector<T>(colN + 1)) {
        // no action
    }

    void clear() {
        for (int i = 0; i < (int)mTree.size(); i++)
            fill(mTree[i].begin(), mTree[i].end(), 0);
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
                res += mTree[r][c];
            }
        }

        return res;
    }

    T rangeSum(int row1, int col1, int row2, int col2) {
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

        for (int r = row; r < (int)mTree.size(); r += r & -r) {
            for (int c = col; c < (int)mTree[row].size(); c += c & -c) {
                mTree[r][c] += val;
            }
        }
    }
};

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testFenwickTree2D() {
    //return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "-- FenwickTree2D ---------------------------------------" << endl;

    const int N = 4;
    int mat[N][N] = { { 1, 2, 3, 4 },
                      { 5, 3, 8, 1 },
                      { 4, 6, 7, 5 },
                      { 2, 4, 8, 9 } };

    FenwickTree2D<int> fenwick(N, N);

    // step1: setup data
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fenwick.init(i, j, mat[i][j]);
            assert(fenwick.rangeSum(i, j, i, j) == mat[i][j]);
        }
    }

    int ans = fenwick.rangeSum(1, 1, 2, 3);
    cout << "fenwick.rangeSum(1, 1, 2, 3) = " << ans << endl;
    assert(ans == 30);

    ans = fenwick.rangeSum(3, 2, 3, 3);
    cout << "fenwick.rangeSum(3, 2, 3, 3) = " << ans << endl;
    assert(ans == 17);

    ans = fenwick.rangeSum(1, 1, 1, 1);
    cout << "fenwick.rangeSum(1, 1, 1, 1) = " << ans << endl;
    assert(ans == 3);

    fenwick.add(1, 1, 3);
    cout << "after fenwick.add(1, 1, 3)" << endl;

    ans = fenwick.rangeSum(1, 1, 2, 3);
    cout << "fenwick.rangeSum(1, 1, 2, 3) = " << ans << endl;
    assert(ans == 33);

    ans = fenwick.rangeSum(3, 2, 3, 3);
    cout << "fenwick.rangeSum(3, 2, 3, 3) = " << ans << endl;
    assert(ans == 17);

    ans = fenwick.rangeSum(1, 1, 1, 1);
    cout << "fenwick.rangeSum(1, 1, 1, 1) = " << ans << endl;
    assert(ans == 6);

    ans = fenwick.rangeSum(0, 0, 0, 0);
    cout << "fenwick.rangeSum(0, 0, 0, 0) = " << ans << endl;
    assert(ans == 1);

    ans = fenwick.rangeSum(3, 3, 3, 3);
    cout << "fenwick.rangeSum(3, 3, 3, 3) = " << ans << endl;
    assert(ans == 9);

    cout << "OK!" << endl;
}
