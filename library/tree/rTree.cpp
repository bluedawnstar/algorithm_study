#include <cassert>
#include <limits>
#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "rTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static double pointToRectL2(int x, int y, int x1, int y1, int x2, int y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double px = x - x1;
    double py = y - y1;

    double squaredLength = dx * dx + dy * dy;
    double dotProduct = dx * px + dy * py;
    if (dotProduct <= 0 || squaredLength == 0)
        return px * px + py * py;

    if (dotProduct >= squaredLength)
        return (px - dx) * (px - dx) + (py - dy) * (py - dy);

    double q = (double)dotProduct / squaredLength;
    return (px - q * dx) * (px - q * dx) + (py - q * dy) * (py - q * dy);
}

void testRTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- R-Tree ----------------------------------" << endl;
    {
        int T = 1000;
        for (int i = 0; i < T; i++) {
            int qx = RandInt32::get() % 1000 - 500;
            int qy = RandInt32::get() % 1000 - 500;
            int n = RandInt32::get() % 100 + 1;

            vector<RTree<int>::Rect> R(n);
            double minDist = DBL_MAX;
            for (int i = 0; i < n; i++) {
                int x1 = RandInt32::get() % 1000 - 500;
                int y1 = RandInt32::get() % 1000 - 500;
                int x2 = x1 + RandInt32::get() % 10;
                int y2 = y1 + RandInt32::get() % 10;
                R[i] = RTree<int>::Rect{ x1, y1, x2, y2 };
                minDist = min(minDist, pointToRectL2(qx, qy, x1, y1, x2, y2));
            }

            RTree<int> tr(R);

            auto ans = tr.searchNearest(qx, qy);
            RTree<int>::Rect& rr = tr.rects[ans.first];
            assert(minDist == ans.second && abs(pointToRectL2(qx, qy, rr.x1, rr.y1, rr.x2, rr.y2) - minDist) <= 1e-9);
        }
    }

    cout << "OK!" << endl;
}
