#include <vector>
#include <algorithm>

using namespace std;

#include "longestNonPeakSubsequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testLongestNonPeakSubsequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Non-Peak Subsequence ------------------------" << endl;
    {
        vector<vector<int>> qry{
            { 1, 2, 3 },
            { 5, 4, 5, 4, 3 },
            { 4, 1, 3, 2, 1, 3, 3 }
        };
        vector<int> gt{
            3, 4, 5
        };

        for (int i = 0; i < int(qry.size()); i++) {
            int ans = LongestNonPeakSubsequence::calcMaxLength(qry[i]);
            if (ans != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt[i] << endl;
            assert(ans == gt[i]);
        }
    }

    cout << "OK!" << endl;
}
