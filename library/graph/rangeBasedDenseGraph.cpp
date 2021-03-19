#include <vector>
#include <deque>
#include <queue>
#include <algorithm>

using namespace std;

#include "rangeBasedDenseGraph.h"
#include "rangeBasedDenseGraph2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testRangeBasedDenseGraph() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Range-based Dense Graph ---------------" << endl;
    {
        vector<pair<int, int>> from{
            { 1, 2 },
            { 5, 5 }
        };
        vector<pair<int, int>> to{
            { 4, 5 },
            { 2, 3 }
        };

        RangeBasedDenseGraph solver;
        solver.build(6, from, to);

        int ans = solver.shortestPath(1, 2);
        int gt = 2;
        assert(ans == gt);
    }
    {
        vector<pair<int, int>> from{
            { 1, 6 }
        };
        vector<pair<int, int>> to{
            { 3, 3 }
        };

        RangeBasedDenseGraph solver;
        solver.build(7, from, to);

        int ans = solver.shortestPath(3, 4);
        int gt = -1;
        assert(ans == gt);
    }
    {
        vector<pair<int, int>> from{
            { 1, 1 }
        };
        vector<pair<int, int>> to{
            { 1, 1 }
        };

        RangeBasedDenseGraph solver;
        solver.build(3, from, to);

        int ans = solver.shortestPath(2, 2);
        int gt = 0;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
