#include <cmath>
#include <tuple>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "convexFunctions.h"
#include "../math/ternarySearch3D.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testConvexFunctions() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Convex Functions (polygon, ellipse, ...) ---------------------" << endl;
    {
        const double EPSILON = 1e-4;

        vector<vector<pair<int, int>>> polygons{
            { { 0, 0 }, { 2, 0 }, { 2, 1 }, { 0, 1 } },
            { { -1, -1 }, { 5, 1 }, { 0, 5 } }
        };
        vector<tuple<pair<int, int>, pair<int, int>, int>> ellipses{
            { { 1, 2 }, { 1, 4 }, 2}
        };

        auto ans = ternarySearchMax3D(-10, -10, 10, 10, [&](double x, double y) {
            return min(ConvexFunctions<int, double>::evalPointToPolygonsMax(polygons, x, y),
                       ConvexFunctions<int, double>::evalPointToEllipsesMax(ellipses, x, y));
        }, 1e-9);

        if (abs(ans.first.first - 1.0) > EPSILON || abs(ans.first.second - 1.0) > EPSILON)
            cout << "Mismatched : ans = " << ans.first << ", gt = (1, 1)" << endl;
        assert(abs(ans.first.first - 1.0) <= EPSILON && abs(ans.first.second - 1.0) <= EPSILON);
    }
    cout << "OK!" << endl;
}
