#include <vector>
#include <algorithm>

using namespace std;

#include "gridConvexHull.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGridConvexHull() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Grid Convex Hull ------------------------" << endl;
    {
        vector<pair<int, int>> points{
            { -1, 2 }, { 0, 0 }, { 0, 2 }, { 0, 3 }, { 1, 3 }, { -1, 3 }, { -1, 1 }, { 1, 0 }, { 0, 1 }, { -2, 2 },
        };

        GridConvexHull<int> cvh(points);
        assert(cvh.countInnerPoints() == 12);
    }
    cout << "OK!" << endl;
}
