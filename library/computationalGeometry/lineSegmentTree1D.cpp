#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "lineSegmentTree1D.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testLineSegmentTree1D() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Segment Tree for 1D Line -----------------------------" << endl;
    {
        int N = 10000;
        int D = 100;
        int X = N - D * 2;
        int T = 1000;
        LineSegmentTree1D tree(N);

        tree.build(N);

        int minX = N, maxX = -1;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % X + D;
            int b = RandInt32::get() % X + D;
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
    }
    {
        int N = 10;
        LineSegmentTree1D tree(N);
        tree.build(N);

        tree.add(3, 6, 1);

        vector<bool> gt(N);
        fill(gt.begin() + 3, gt.begin() + 7, true);

        vector<bool> ans(N);
        tree.forEachCovered(0, N - 1, [&ans](int idx) {
            ans[idx] = true;
        });

        assert(gt == ans);
    }
    {
        int N = 100;
        LineSegmentTree1D tree(N);
        tree.build(N);

        tree.add(10, 20, 1);
        tree.add(15, 30, 1);
        tree.add(60, 90, 1);
        tree.add(70, 80, -1);

        vector<bool> gt(N);
        fill(gt.begin() + 10, gt.begin() + 31, true);
        fill(gt.begin() + 60, gt.begin() + 70, true);
        fill(gt.begin() + 81, gt.begin() + 91, true);

        vector<bool> ans(N);
        tree.forEachCovered(0, N - 1, [&ans](int idx) {
            ans[idx] = true;
        });

        assert(gt == ans);
    }
    cout << "OK!" << endl;
}
