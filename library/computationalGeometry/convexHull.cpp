#include <cmath>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#include "vec2D.h"
#include "line.h"
#include "polygon.h"
#include "convexHull.h"

#if 0
    //### find farest points from a line of convex hull's boundary
    // input : gP -> points, (u, v) -> two points of a line

    vector<Vec2D<int>> newP(gP);

    vector<int> boundary = doGrahamScan(newP);
    map<int, int> revI;
    for (int i = 0; i < (int)boundary.size(); i++) {
        revI[boundary[i]] = i;
    }

    int boundaryN = (int)boundary.size();

    // ... (u, v) ...

    int uI = revI[u];
    int vI = revI[v];
    if ((uI + 1) % boundaryN != vI)
        swap(uI, vI);

    Vec2D<int>& p0 = gP[boundary[uI]];
    Vec2D<int>& p1 = gP[boundary[vI]];

    ll cr = 0;
    int lo = vI, hi = uI - 1, mid;

    if (lo > hi)
        hi += boundaryN;

    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;

        cr = cross(p0, p1, gP[boundary[mid % boundaryN]], gP[boundary[(mid + 1) % boundaryN]]);
        if (cr == 0) {
            lo = mid;
            break;
        } else if (cr < 0)
            hi = mid - 1;
        else
            lo = mid + 1;
    }

    if (cr == 0) {
        // P0 -> gP[boundary[lo % boundaryN]];
        // P1 -> gP[boundary[(lo + 1) % boundaryN]];
    } else {
        // P0 -> gP[boundary[lo % boundaryN]];
    }
#endif

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void makeData(vector<Vec2D<int>>& points, int size) {
    if ((int)points.size() < size)
        points.assign(vector<Vec2D<int>>::size_type(size) - points.size(), Vec2D<int>());

    for (int i = 0; i < size; i++) {
        points[i].x = rand();
        points[i].y = rand();
    }
    random_shuffle(points.begin(), points.end());
}

bool compare(vector<Vec2D<int>>& A, vector<Vec2D<int>>& B) {
    if (A.size() != B.size())
        return false;

    for (int i = 0; i < (int)A.size(); i++) {
        if (!(A[i] == B[i]))
            return false;
    }
    return true;
}

void dump(vector<Vec2D<int>>& in) {
    for (auto it : in) {
        cout << " (" << it.x << ", " << it.y << ")";
    }
    cout << endl;
}

bool testConvexHull(vector<Vec2D<int>>& in) {
    vector<Vec2D<int>> out1 = doJarvis(vector<Vec2D<int>>(in));
    vector<Vec2D<int>> out2 = doGrahamScan(vector<Vec2D<int>>(in));
    vector<Vec2D<int>> out3 = doGrahamScanNoRemove(vector<Vec2D<int>>(in));

    sort(out1.begin(), out1.end());
    sort(out2.begin(), out2.end());
    sort(out3.begin(), out3.end());
    if (out1 != out2 || out1 != out3) {
        cout << "---Jarvis----------" << endl;
        dump(out1);
        cout << "---Graham Scan----------" << endl;
        dump(out2);
        cout << "---Graham Scan without Removing ----------" << endl;
        dump(out3);
        return false;
    }
    return true;
}

void testConvexHull() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Convex Hull ------------------------" << endl;

    {
        vector<Vec2D<int>> P(16);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                P[i * 4 + j].y = i;
                P[i * 4 + j].x = j;
            }
        }
        vector<Vec2D<int>> out1 = doGrahamScanNoRemove(vector<Vec2D<int>>(P), true);
        dump(out1);
        vector<Vec2D<int>> out2 = doGrahamScanNoRemove(vector<Vec2D<int>>(P), false);
        dump(out2);

        vector<Vec2D<int>> P2 = vector<Vec2D<int>>{
                                    { 0,  2 },
                        { -1,  1 }, { 0,  1 }, { 1,  1 },
             { -2, 0 }, { -1,  0 }, { 0,  0 }, { 1,  0 }, { 2, 0 },
                        { -1, -1 }, { 0, -1 }, { 1, -1 },
                                    { 0, -2 },
        };
        vector<Vec2D<int>> out3 = doGrahamScanNoRemove(vector<Vec2D<int>>(P2), true);
        dump(out3);
        vector<Vec2D<int>> out4 = doGrahamScanNoRemove(vector<Vec2D<int>>(P2), false);
        dump(out4);
    }

    assert(testConvexHull(vector<Vec2D<int>>{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 },
                                              { 0, 1 }, { 1, 1 }, { 2, 1 },
                                              { 0, 2 }, { 1, 2 },
                                              { 0, 3 } }) == true);
    assert(testConvexHull(vector<Vec2D<int>>{ { 0, 3 }, { 1, 1 }, { 2, 2 }, { 4, 4 },
                                              { 0, 0 }, { 1, 2 }, { 3, 1 }, { 3, 3 } }) == true);

    cout << "random test ..." << endl;

    bool success = true;
    for (int i = 0; i < 1000; i++) {
        cout << "Case #" << i << "\r";
        int N = rand() % 1000 + 1;
        vector<Vec2D<int>> points(N);
        makeData(points, N);
        if (!testConvexHull(points)) {
            success = false;
            break;
        }
    }
    assert(success);

    cout << "speed test (x 1000) ..." << endl;
    {
        int N = rand() % 1000 + 1;
        vector<Vec2D<int>> points(N);
        makeData(points, N);

        PROFILE_START(0);
        for (int i = 0; i < 1000; i++) {
            vector<Vec2D<int>> out1 = doJarvis(vector<Vec2D<int>>(points));
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < 1000; i++) {
            vector<Vec2D<int>> out2 = doGrahamScan(vector<Vec2D<int>>(points));
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        for (int i = 0; i < 1000; i++) {
            vector<Vec2D<int>> out3 = doGrahamScanNoRemove(vector<Vec2D<int>>(points));
        }
        PROFILE_STOP(2);
    }

    cout << "OK!" << endl;
}
