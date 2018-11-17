#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "mergeSortTree2DPoint.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int countSlow(const vector<pair<int, int>>& v, int x1, int y1, int x2, int y2) {
    int res = 0;
    for (auto it : v) {
        if (x1 <= it.first && it.first <= x2 && y1 <= it.second && it.second <= y2)
            res++;
    }
    return res;
}

void testMergeSortTree2DPoint() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Merge Sort Tree of 2D Point ---------------------------------------" << endl;
    {
        int W = 1000000;
        int H = 1000000;
        int N = 10000;
        int T = 1000;

        vector<pair<int, int>> points;
        points.reserve(N);
        for (int i = 0; i < N; i++)
            points.emplace_back(RandInt32::get() % W, RandInt32::get() % H);

        MergeSortTree2DPoint<int> tree(points);

        for (int i = 0; i < T; i++) {
            int x1 = RandInt32::get() % W;
            int x2 = RandInt32::get() % W;
            int y1 = RandInt32::get() % H;
            int y2 = RandInt32::get() % H;
            if (x1 > x2)
                swap(x1, x2);
            if (y1 > y2)
                swap(y1, y2);

            int gt = countSlow(points, x1, y1, x2, y2);
            int ans = tree.count(x1, y1, x2, y2);
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
