#include <cassert>
#include <cmath>
#include <limits>
#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "metricTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static long long distL2(int x1, int y1, int x2, int y2) {
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    return dx * dx + dy * dy;
}

void testMetricTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Metric-Tree ----------------------------------" << endl;
    {
        int T = 10000;
        for (int i = 0; i < T; i++) {
            int qx = RandInt32::get() % 100 - 50;
            int qy = RandInt32::get() % 100 - 50;
            int n = RandInt32::get() % 100 + 1;

            vector<pair<int,int>> P(n);
            long long minDist = numeric_limits<long long>::max();
            for (int i = 0; i < n; i++) {
                P[i].first = RandInt32::get() % 100 - 50;
                P[i].second = RandInt32::get() % 100 - 50;
                minDist = min(minDist, distL2(qx, qy, P[i].first, P[i].second));
            }

            MetricTree<> tr(P);

            auto ans = tr.searchNearest(qx, qy);
            auto& pp = tr.points[ans.first];
            assert(minDist == ans.second && distL2(qx, qy, pp.first, pp.second) == minDist);
        }
    }

    cout << "OK!" << endl;
}
