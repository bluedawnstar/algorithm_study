#include <vector>
#include <algorithm>

using namespace std;

#include "longestRepeatedSubsequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testLongestRepeatedSubsequence() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Repeated Subsequence ------------------------" << endl;
    {
        string s = "aabebcdd";
        assert(LongestRepeatedSubsequence::findLength(s) == 3);
        assert(LongestRepeatedSubsequence::find(s) == "abd");
    }

    cout << "OK!" << endl;
}
