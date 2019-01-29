#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "bipartiteGraphCheck.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testBipartiteGraphCheck() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Bipartite Graph Check -------------" << endl;
    {
        vector<vector<int>> edges{
            { 1, 2 },
            { 0, 3 },
            { 0, 3 },
            { 1, 2 }
        };
        assert(BipartiteGraphCheck::isBipartiteGraph(edges, 0) == true);
    }
    {
        vector<vector<int>> edges{
            { 1, 2 },
            { 0, 2, 3 },
            { 0, 1, 3 },
            { 1, 2 }
        };
        assert(BipartiteGraphCheck::isBipartiteGraph(edges, 0) == false);
    }

    cout << "OK" << endl;
}
