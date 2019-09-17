#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "powerSumOfAllCombinations_NTT.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPowerSumOfAllCombinations() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Power Sum of All Combinations ------------------------" << endl;
    {
        PowerSumOfAllCombinationsWithNTT<> solver;

        vector<pair<int, int>> costAndValue{
            { 1, 2 },
            { 2, 3 },
            { 1, 4 }
        };
        int S = 2;
        int K = 2;

        solver.init(K * 2 + 1);
        int ans = solver.solve(costAndValue, S, K);
        if (ans != 65)
            cout << "ERROR: invalid answer!" << endl;
        assert(ans == 65);
    }

    cout << "OK!" << endl;
}
