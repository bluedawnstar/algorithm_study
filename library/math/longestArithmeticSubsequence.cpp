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
        cout << buildLongestArithmeticSubsequenceK(5, 2) << endl;
        cout << buildLongestArithmeticSubsequenceK(5, 3) << endl;
        cout << buildLongestArithmeticSubsequenceK(8, 2) << endl;
    }

    cout << "OK!" << endl;
}
