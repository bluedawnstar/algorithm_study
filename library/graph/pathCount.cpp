#include <climits>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "pathCount.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static const int INF = 0x3f3f3f3f;

void testPathCount() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Path Count ---------------" << endl;
    {
        vector<vector<int>> edges = {
            { 3 },
            { 0, 4, 5 },
            { 1 },
            { 1 },
            { },
            { 2, 4 }
        };
        vector<vector<int>> gt4 = {
            { 0, 0, 1, 1, 1, 0 },
            { 2, 0, 0, 0, 2, 2 },
            { 0, 2, 0, 0, 0, 0 },
            { 0, 2, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 1, 1, 1, 0 }
        };

        for (int i = 0; i < 6; i++)
            assert(UniquePathCounter<int>::countNaive(edges, i, 4) == gt4[i]);
        assert(UniquePathCounter<int>::countAllNaive(edges, 4) == gt4);
        assert(UniquePathCounter<int>::countAllFastFromEdges(edges, 4).val == gt4);
        assert(UniquePathCounter<int>::countAllFastFromAdjMatrix(UniquePathCounter<int>::countAllNaive(edges, 1), 4).val == gt4);
    }

    cout << "OK!" << endl;
}
