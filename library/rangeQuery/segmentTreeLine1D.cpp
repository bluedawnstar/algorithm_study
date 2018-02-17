#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreeLine1D.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSegmentTreeLine1D() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Segment Tree for 1D Line -----------------------------" << endl;

    int N = 10000;
    int D = 100;
    int X = N - D * 2;
    int T = 1000;
    SegmentTreeLine1D tree(N);

    tree.build(N);

    int minX = N, maxX = -1;
    for (int i = 0; i < T; i++) {
        int a = rand() % X + D;
        int b = rand() % X + D;
        if (a > b)
            swap(a, b);
        tree.add(a, b, 1);
        minX = min(minX, a);
        maxX = max(maxX, b);
    }

    auto ans = tree.queryNotCovered();
    cout << ans << endl;
    assert(ans == N - (maxX - minX + 1));

    int lt = 7;
    int rt = N - 6;
    ans = tree.query(lt, rt, 0);
    cout << "(" << minX << ", " << maxX << ") = " << ans << endl;
    assert(ans == (rt - lt + 1) - (min(maxX, rt) - max(minX, lt) + 1));

    cout << "OK!" << endl;
}
