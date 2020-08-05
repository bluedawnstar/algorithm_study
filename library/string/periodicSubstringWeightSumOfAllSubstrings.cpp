#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "periodicSubstringWeightSumOfAllSubstrings.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testPeriodicSubstringWeightSumOfAllSubstrings() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Periodic Substring Weight-Sum Of All Substrings ----------------" << endl;
    {
        const string s = "abab";
        const vector<int> W{ 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        const int gt = 39;
        int ans1 = PeriodicSubstringWeightSumOfAllSubstrings<>::solve(s, W);
        int ans2 = PeriodicSubstringWeightSumOfAllSubstrings<>::solveWithSuffixAutomaton(s, W);
        int ans3 = PeriodicSubstringWeightSumOfAllSubstrings<>::solveNaive(s, W);
        if (ans1 != gt || ans2 != gt || ans3 != gt)
            cout << "Mismatch : " << ans1 << ", " << ans2 << ", " << ans3 << ", " << gt << endl;
        assert(ans1 == gt);
        assert(ans2 == gt);
        assert(ans3 == gt);
    }
    cout << "OK!" << endl;
}
