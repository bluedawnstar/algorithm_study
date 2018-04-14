#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "reachableAllPair.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testReachableAllPairs() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Reachable Path - All Pairs ---------" << endl;
    {
        vector<vector<bool>> D{
            { 0, true, 0 },
            { 0, 0, true },
            { 0, 0, 0 }
        };
        ReachableAllPair::doFloydWarshal(D, 3);
        assert(D[0][0]);
        assert(D[0][1]);
        assert(D[0][2]);
        assert(D[1][1]);
        assert(D[1][2]);
        assert(D[2][2]);
        assert(!D[1][0]);
        assert(!D[2][0]);
        assert(!D[2][1]);
    }
    cout << "OK" << endl;
}
