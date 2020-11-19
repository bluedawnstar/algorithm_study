#include <assert.h>
#include <tuple>
#include <numeric>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "generalMaxMatchingWeighted.h"
#include "generalMaxMatchingWeighted2.h"

#include "generalMaxMatchingWeighted_VertexAndEdgeWeighted.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <map>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testGeneralMaxMatchingWeighted() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Blossom Algorithm for Weighted General Matching -------------" << endl;
    {
        // https://www.codechef.com/problems/SELEDGE
        int N = 5;
        int K = 2;
        vector<int> vertexWeight{
            1, 2, 4, 8, 16
        };
        vector<tuple<int, int, int>> edges{
            { 0, 1, 1 },
            { 0, 2, 2 },
            { 2, 3, 2 },
            { 3, 4, 2 },
            { 1, 4, 1 }
        };

        auto ans1 = GeneralMaxMatchingWithVertexAndEdgeWeight2<int>::calcMaxMatching(N, vertexWeight, edges);
        auto ans2 = GeneralMaxMatchingWithVertexAndEdgeWeight<int>::calcMaxMatching(N, vertexWeight, edges);
        auto ans2k = GeneralMaxMatchingWithVertexAndEdgeWeight<int>::calcMaxMatching(N, vertexWeight, edges, K);
        assert(ans1 == 27 && ans2 == 27 && ans2k == 27);
    }

    cout << "OK" << endl;
}
