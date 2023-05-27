#include <vector>
#include <algorithm>

using namespace std;

#include "subsetAnd.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testSubsetAND() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Subset AND -----------------------------" << endl;
    {
        vector<int> A{ 1, 2, 3 };
        int ans = SubsetAnd::longestSubsequenceGreaterThanOrEqual(A, 2, 30);
        int gt = 2;
        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<int> A{ 6, 1, 7, 4, 3 };
        int ans = SubsetAnd::longestSubsequenceGreaterThanOrEqual(A, 3, 30);
        int gt = 3;
        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<int> A{ 1, 3, 4 };
        int ans = SubsetAnd::longestSubsequenceGreaterThanOrEqual(A, 5, 30);
        int gt = 0;
        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
