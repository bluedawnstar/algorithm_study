#include <vector>
#include <algorithm>

using namespace std;

#include "longestArithmeticSubsequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testLongestArithmeticSubsequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Arithmetic Subsequence -----------------------" << endl;
    {
        auto las1 = buildLongestArithmeticSubsequenceK(5, 2);
        auto las1Len = lenghtOfLongestArithmeticSubsequence(las1);
        cout << las1 << ", " << las1Len << endl;
        assert(las1Len == 2);

        auto las2 = buildLongestArithmeticSubsequenceK(5, 3);
        auto las2Len = lenghtOfLongestArithmeticSubsequence(las2);
        cout << las2 << ", " << las2Len << endl;
        assert(las2Len == 3);

        auto las3 = buildLongestArithmeticSubsequenceK(8, 2);
        auto las3Len = lenghtOfLongestArithmeticSubsequence(las3);
        cout << las3 << ", " << las3Len << endl;
        assert(las3Len == 2);
    }

    cout << "OK!" << endl;
}
