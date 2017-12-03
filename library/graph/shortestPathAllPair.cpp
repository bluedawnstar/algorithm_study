#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "shortestPathAllPair.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static const int INF = ShortestAllPairs<int>::INF;

void testShortestPathAllPairs() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Shortest Path - All Pairs ---------" << endl;
    {
        vector<vector<int>> D{
            { 0, 3, 2 },
            { 0, 0, 1 },
            { INF, 0, 0 }
        };
        vector<vector<int>> parent(3, vector<int>(3));
        ShortestAllPairs<int>::doFloydWarshal(D, parent, 3);
        assert(D[0][0] == 0);
        assert(D[0][1] == 2);
        assert(D[0][2] == 2);
        assert(parent[0][0] == -1);
        assert(parent[0][1] == 2);
        assert(parent[0][2] == 0);
        
        vector<int> pathGT{ 0, 2, 1 };
        auto pathAns = ShortestAllPairs<int>::getPath(parent, 3, 0, 1);
        assert(pathGT == pathAns);
    }
    cout << "OK" << endl;
}
