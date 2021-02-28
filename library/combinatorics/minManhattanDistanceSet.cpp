#include <vector>
#include <algorithm>

using namespace std;

#include "minManhattanDistanceSet.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testMinManhattanDistanceSet() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Set with Min Manhattan Distance among All Pairs ----------------" << endl;
    {
        vector<pair<int, int>> initCells{
            { 3, 3 },
            { 4, 3 },
            { 1, 1 },
            { 5, 4 },
            { 5, 2 },
            { 2, 2 }
        };
        MinManhattanDistanceSet<> solver(10, 10);
        auto ans = solver.calcMinSizeAndCount(initCells);
        assert(ans.first == 10 && ans.second == 2);
    }
    cout << "OK!" << endl;
}
