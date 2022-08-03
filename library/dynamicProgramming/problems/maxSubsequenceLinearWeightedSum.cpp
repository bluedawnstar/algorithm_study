#include <algorithm>

using namespace std;

#include "maxSubsequenceLinearWeightedSum.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../../common/profile.h"

void testMaxSubsequenceLinearWeightedSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Linear Weighted Sum of Subsequences ----------------" << endl;
    {
        MaxSubsequenceLinearWeightedSum<long long> solver;

        vector<vector<int>> testCases{
            { -29, -34, 97, 37, 57 },
            { 60, 5, 8, 45, 89, -88, 75, 25, 98, -15, 12, -3, -16, -42 },
            { -19, 76, 98, 48, 19, 80, -96, 81, 0, 30 },
            { 22, -75, -8, -25, 55, 32, 66, -42, 6 },
            { 41, 28, -23, -50, 43, 11, 72, 65, -13, -40, 53, 24, 13, 57, 64 },
            { 29, 45 },
            { 25, 95, -77, 68, -52, 90, -71 },
            { 49, 31, 10, -75, 34, 25, 13, 38, 19, 45, 88, 7, 3, 59 },
            { 22, -23, -43, 5, 31, 36, 86, 32, -30 },
            { 87, -43, 20, -76, -49 }
        };
        vector<int> gt{ 627, 2236, 2031, 749, 3603, 119, 779, 3094, 1096, 127 };

        for (int i = 0; i < int(testCases.size()); i++) {
            auto ans = MaxSubsequenceLinearWeightedSum<int>::solve(testCases[i]);
            if (ans != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt[i] << endl;
            assert(ans == gt[i]);
        }
    }

    cout << "OK!" << endl;
}
