#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "powerSeries.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

//<Related problems>
// https://www.codechef.com/problems/PPARTS

void testPowerSeries() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Power Series -----------------------" << endl;
    {
        vector<pair<int, int>> p{
            { 1, 3 },
            { 2, 2 },
            { 3, 2 },
            { 3, 1 },
            { 5, 1 },
            { 6, 1 }
        };
        PowerSeriesMod<998'244'353, 3> solver;
        vector<int> ans = solver.calculateAllCombinationWithNTT(p, 7);
        vector<int> gt{ 1, 1, 2, 4, 4, 7, 9, 10 };
        if (ans != gt)
            cout << "Mismatched : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
