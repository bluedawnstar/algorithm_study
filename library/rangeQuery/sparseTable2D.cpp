#include <memory.h>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "sparseTable2D.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static vector<vector<int>> makeRandomArray2D(int rows, int cols, int maxValue) {
    vector<vector<int>> res(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res[i][j] = RandInt32::get() % maxValue;
        }
    }
    return res;
}

static vector<tuple<int, int, int, int>> makeQuery2D(int rows, int cols, int N) {
    vector<tuple<int, int, int, int>> res;
    res.reserve(rows);
    for (int i = 0; i < N; i++) {
        int L = RandInt32::get() % cols;
        int R = RandInt32::get() % cols;
        int T = RandInt32::get() % rows;
        int B = RandInt32::get() % rows;
        if (L > R)
            swap(L, R);
        if (T > B)
            swap(T, B);
        res.emplace_back(L, T, R, B);
    }
    return res;
}

static int minSlow(const vector<vector<int>>& vec, int left, int top, int right, int bottom) {
    int res = numeric_limits<int>::max();
    for (int i = top; i <= bottom; i++) {
        for (int j = left; j <= right; j++)
            res = min(res, vec[i][j]);
    }

    return res;
}

static int maxSlow(const vector<vector<int>>& vec, int left, int top, int right, int bottom) {
    int res = numeric_limits<int>::min();
    for (int i = top; i <= bottom; i++) {
        for (int j = left; j <= right; j++)
            res = max(res, vec[i][j]);
    }

    return res;
}

static int sumSlow(const vector<vector<int>>& vec, int left, int top, int right, int bottom) {
    int res = 0;
    for (int i = top; i <= bottom; i++) {
        for (int j = left; j <= right; j++)
            res += vec[i][j];
    }

    return res;
}

void testSparseTable2D() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Sparse Table - 2D ----------------------------------------" << endl;
    {
        int RowN = 100;
        int ColN = 100;
        int T = 200;
        auto A = makeRandomArray2D(RowN, ColN, 1000000000);
        auto Q = makeQuery2D(RowN, ColN, T);

        auto spt = makeSparseTable2D(A, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        for (int i = 0; i < int(Q.size()); i++) {
            auto& q = Q[i];
            int gt = minSlow(A, get<0>(q), get<1>(q), get<2>(q), get<3>(q));
            int ans = spt.query(get<0>(q), get<1>(q), get<2>(q), get<3>(q));
            assert(ans == gt);
        }
    }
    {
        int RowN = 100;
        int ColN = 100;
        int T = 200;
        auto A = makeRandomArray2D(RowN, ColN, 1000000000);
        auto Q = makeQuery2D(RowN, ColN, T);

        auto spt = makeSparseTable2D(A, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());
        for (int i = 0; i < int(Q.size()); i++) {
            auto& q = Q[i];
            int gt = maxSlow(A, get<0>(q), get<1>(q), get<2>(q), get<3>(q));
            int ans = spt.query(get<0>(q), get<1>(q), get<2>(q), get<3>(q));
            assert(ans == gt);
        }
    }
    {
        int RowN = 100;
        int ColN = 100;
        int T = 200;
        auto A = makeRandomArray2D(RowN, ColN, 1000);
        auto Q = makeQuery2D(RowN, ColN, T);

        auto spt = makeSparseTable2D(A, [](int a, int b) { return a + b; });
        for (int i = 0; i < int(Q.size()); i++) {
            auto& q = Q[i];
            int gt = sumSlow(A, get<0>(q), get<1>(q), get<2>(q), get<3>(q));
            int ans = spt.queryNoOverlap(get<0>(q), get<1>(q), get<2>(q), get<3>(q));
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
