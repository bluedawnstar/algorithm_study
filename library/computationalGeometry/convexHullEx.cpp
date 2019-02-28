#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "vec2D.h"
#include "line.h"
#include "polygon.h"
#include "convexHull.h"
#include "convexHullEx.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static void makeData(vector<pair<int, int>>& points, int size) {
    if (int(points.size()) < size)
        points.assign(vector<pair<int, int>>::size_type(size) - points.size(), pair<int, int>());

    for (int i = 0; i < size; i++) {
        points[i].first = RandInt32::get() % 32767;
        points[i].second = RandInt32::get() % 32767;
    }
    random_shuffle(points.begin(), points.end());
}

static void makeData(vector<pair<int, int>>& points, int size, int R) {
    if (int(points.size()) < size)
        points.assign(vector<pair<int, int>>::size_type(size) - points.size(), pair<int, int>());

    for (int i = 0; i < size; i++) {
        points[i].first = RandInt32::get() % (2 * R + 1) % R;
        points[i].second = ((RandInt32::get() & 1) ? 1 : -1) * int(sqrt(1.0 * R * R - 1.0 * points[i].first * points[i].first));
    }
    random_shuffle(points.begin(), points.end());
}

static void dump(vector<pair<int, int>>& in) {
    for (auto it : in) {
        cout << " (" << it.first << ", " << it.second << ")";
    }
    cout << endl;
}

static bool testConvexHull(vector<pair<int,int>>& in) {
    vector<pair<int, int>> lower, upper;
    GrahamAndrew::convexHull(vector<pair<int,int>>(in), lower, upper);

    ConvexHull hull;
    hull.build(vector<pair<int, int>>(in));

    return lower == hull.lower && upper == hull.upper;
}

void testConvexHullEx() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Convex Hull Ex ---------------------" << endl;
    {
        assert(testConvexHull(vector<pair<int, int>>{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 },
        { 0, 1 }, { 1, 1 }, { 2, 1 },
        { 0, 2 }, { 1, 2 },
        { 0, 3 } }) == true);
        assert(testConvexHull(vector<pair<int, int>>{ { 0, 3 }, { 1, 1 }, { 2, 2 }, { 4, 4 },
        { 0, 0 }, { 1, 2 }, { 3, 1 }, { 3, 3 } }) == true);
    }
    cout << "... random test" << endl;
    {
        int T = 1000;
#ifdef _DEBUG
        T = 100;
#endif
        for (int i = 0; i < T; i++) {
            cout << "Case #" << i << "\r";
            int N = RandInt32::get() % 1000 + 1;
            vector<pair<int,int>> points(N);
            makeData(points, N);

            auto result = testConvexHull(points);
            if (!result)
                cerr << "ERROR! : " << i << endl;
            assert(result);
        }
    }
    cout << "OK!       " << endl;
}
