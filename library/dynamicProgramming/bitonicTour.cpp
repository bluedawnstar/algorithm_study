#include <cmath>
#include <limits>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "bitonicTour.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static double calcDist(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void testBitonicTour() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Bitonic Tour -----------------------------" << endl;
    {
        vector<pair<int, int>> in{ { 0, 1 }, { 1, 0 }, { 2, 2 }, { 3, 1 } };
        auto ans = bitonicTourCost<int,double>(in, [](const pair<int, int>& a, const pair<int, int>& b) {
            return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
        });
        auto gt = calcDist(0, 1, 2, 2) + calcDist(2, 2, 3, 1) + calcDist(3, 1, 1, 0) + calcDist(1, 0, 0, 1);
        assert(fabs(gt - ans) < 1e-9);
    }
    {
        vector<pair<int, int>> in{ { 13, 8 }, { 3, 2 }, { 12, 3 }, { 2, 9 }, { 15, 4 }, { 10, 7 }, { 4, 6 } };
        auto t = bitonicTourCost<int, double>(in, [](const pair<int, int>& a, const pair<int, int>& b) {
            return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
        });
        auto ans = int(t + 0.5);
        auto gt = 37;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
