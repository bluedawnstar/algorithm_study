using namespace std;

#include "treeFenwickTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testFenwickTree2D() {
    return; //TODO: if you want to test a split function, make this line to a comment.

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
            assert(fenwick.sumRange(i, j, i, j) == mat[i][j]);
        }
    }

    int ans = fenwick.sumRange(1, 1, 2, 3);
    cout << "fenwick.sumRange(1, 1, 2, 3) = " << ans << endl;
    assert(ans == 30);

    ans = fenwick.sumRange(3, 2, 3, 3);
    cout << "fenwick.sumRange(3, 2, 3, 3) = " << ans << endl;
    assert(ans == 17);

    ans = fenwick.sumRange(1, 1, 1, 1);
    cout << "fenwick.sumRange(1, 1, 1, 1) = " << ans << endl;
    assert(ans == 3);

    fenwick.add(1, 1, 3);
    cout << "after fenwick.add(1, 1, 3)" << endl;

    ans = fenwick.sumRange(1, 1, 2, 3);
    cout << "fenwick.sumRange(1, 1, 2, 3) = " << ans << endl;
    assert(ans == 33);

    ans = fenwick.sumRange(3, 2, 3, 3);
    cout << "fenwick.sumRange(3, 2, 3, 3) = " << ans << endl;
    assert(ans == 17);

    ans = fenwick.sumRange(1, 1, 1, 1);
    cout << "fenwick.sumRange(1, 1, 1, 1) = " << ans << endl;
    assert(ans == 6);

    ans = fenwick.sumRange(0, 0, 0, 0);
    cout << "fenwick.sumRange(0, 0, 0, 0) = " << ans << endl;
    assert(ans == 1);

    ans = fenwick.sumRange(3, 3, 3, 3);
    cout << "fenwick.sumRange(3, 3, 3, 3) = " << ans << endl;
    assert(ans == 9);

    cout << "OK!" << endl;
}
