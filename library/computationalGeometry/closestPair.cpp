#include <cmath>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#include "vec2D.h"
#include "closestPair.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static long long distL2(const pair<long long, long long>& L, const pair<long long, long long>& R) {
    return (R.first - L.first) * (R.first - L.first) + (R.second - L.second) * (R.second - L.second);
}

static vector<pair<long long, long long>> findClosestPairSlow(const vector<pair<long long, long long>>& points) {
    long long minDist = 0x3f3f3f3f3f3f3f3fll;

    vector<pair<long long, long long>> res(2);
    for (int i = 0; i < int(points.size()); i++) {
        for (int j = i + 1; j < int(points.size()); j++) {
            auto d = distL2(points[i], points[j]);
            if (minDist > d) {
                minDist = d;
                res[0] = points[i];
                res[1] = points[j];
            }
        }
    }

    return res;
}

void testClosestPair() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Closest Pair ---------------------------------" << endl;

    int N = 1000;

    vector<pair<long long, long long>> points;
    set<pair<long long, long long>> S;
    for (int i = 0; i < N; i++) {
        auto p = make_pair(RandInt64::get() % 65536, RandInt64::get() % 65536);
        if (S.find(p) != S.end())
            continue;

        S.insert(p);
        points.push_back(p);
    }

    auto ans = ClosestPair<long long>::findClosestPair(vector<pair<long long, long long>>(points));
    auto gt = findClosestPairSlow(vector<pair<long long, long long>>(points));

    auto ansDist = distL2(ans[0], ans[1]);
    auto gtDist = distL2(gt[0], gt[1]);

    cout << ans << " (" << ansDist << "), " << gt << " (" << gtDist << ")" << endl;
    assert(ansDist == gtDist);

    cout << "OK!" << endl;
}
