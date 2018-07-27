#include <cmath>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#include "distanceSumChebyshev.h"
#include "distanceSumManhattan.h"

// <Related Problems>
// https://www.hackerrank.com/contests/hourrank-27/challenges/moving-the-kings

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static long long calcManhattanDistance(const vector<pair<int, int>>& points, int x, int y) {
    long long res = 0;
    for (auto& it : points) {
        res += abs((long long)it.first - x) + abs((long long)it.second - y);
    }
    return res;
}

static long long calcChebyshevDistance(const vector<pair<int, int>>& points, int x, int y) {
    long long res = 0;
    for (auto& it : points) {
        res += max(abs((long long)it.first - x), abs((long long)it.second - y));
    }
    return res;
}

void testDistanceSum() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Distance Sum ---------------------------------" << endl;
    {
        int MaxV = 2000000000;
        int N = 10000;
        int T = 1000;

        vector<pair<int, int>> points(N);
        for (int i = 0; i < N; i++) {
            points[i].first = RandInt32::get() % MaxV - MaxV / 2;
            points[i].second = RandInt32::get() % MaxV - MaxV / 2;
        }

        ChebyshevDistanceSum<> chebyshev;
        ManhattanDistanceSum<> manhattan;

        chebyshev.build(points);
        manhattan.build(points);
        for (int i = 0; i < T; i++) {
            int x = RandInt32::get() % MaxV - MaxV / 2;
            int y = RandInt32::get() % MaxV - MaxV / 2;
            assert(chebyshev.query(x, y) == calcChebyshevDistance(points, x, y));
            assert(manhattan.query(x, y) == calcManhattanDistance(points, x, y));
        }
    }

    cout << "OK!" << endl;
}
