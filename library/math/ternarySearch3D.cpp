#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "ternarySearch3D.h"
#include "../computationalGeometry/convexFunctions.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

// <Examples>
// https://www.hackerrank.com/contests/w25/challenges/good-point/problem

// https://leetcode.com/problems/best-position-for-a-service-centre/
static double distance(double cx, double cy, int x, int y) {
    return sqrt((cx - x) * (cx - x) + (cy - y) * (cy - y));
}

static double distanceSum(const vector<pair<int,int>>& positions, double x, double y) {
    double res = 0;
    for (auto& it : positions) {
        res += distance(x, y, it.first, it.second);
    }
    return res;
}

static pair<pair<double,double>,double> findPositionOfMinDistanceSum(const vector<pair<int,int>>& positions) {
    auto ans = ternarySearchMax3D(0, 0, 100, 100, [&](double x, double y) {
        return -distanceSum(positions, x, y);
    });
    return make_pair(ans.first, -ans.second);
}

void testTernarySearch3D() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Ternary Search for 3D -----------------------" << endl;
    // https://www.hackerrank.com/contests/w25/challenges/good-point/problem
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
    // https://leetcode.com/problems/best-position-for-a-service-centre/
    {
        const double EPSILON = 1e-4;

        vector<pair<int, int>> positions{
            { 0, 1 }, { 1, 0 }, { 1, 2 }, { 2, 1 }
        };

        auto ans = findPositionOfMinDistanceSum(positions);

        if (abs(ans.first.first - 1.0) > EPSILON || abs(ans.first.second - 1.0) > EPSILON || abs(ans.second - 4.0) > EPSILON)
            cout << "Mismatched : ans = " << ans << ", gt = ((1, 1), 4)" << endl;
        assert(abs(ans.first.first - 1.0) <= EPSILON && abs(ans.first.second - 1.0) <= EPSILON
               && abs(ans.second - 4.0) <= EPSILON);
    }

    cout << "OK!" << endl;
}
