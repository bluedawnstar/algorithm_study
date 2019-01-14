#include <limits>
#include <functional>
#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;

#include "mergeSortTreeConvexHull.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

void testMergeSortTreeConvexHull() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Merge Sort Tree for Convex Hull -----------------------------------" << endl;
    // https://www.codechef.com/JAN19A/problems/MXDIST/
    {
        vector<pair<int, int>> points{
            { 1, 2 }, { 2, 2 }, { 3, 0 }, { 3, 1 }, { 3, 3 },
            { 3, 4 }, { 3, 1 }, { 3, 0 }, { 3, 3 }, { 4, 2 }, { 5, 2 },
        };

        MergeSortTreeConvexHull tree;
        tree.build(points);

        vector<long long> gt{ 16, 5, 16, 9, 2 };

        vector<tuple<int, int, int>> qry{
            { 4, 10, 0 }, { 8, 10, 1 }, { 2, 7, 2 }, { 3, 6, 3 }, { 8, 9, 4 },
        };
        vector<long long> ans(qry.size());

        solveRangeMaxDist(tree, ans, qry);
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
