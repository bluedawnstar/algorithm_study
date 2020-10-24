#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "longestZigzagSubsequence.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testLongestZigzagSubsequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Zigzag Subsequence ----------------------" << endl;
    {
        int ans = LongestZigzagSubsequence::calcLength(vector<int>{ 5, 7, 11, 13, 19, 17, 8, 4, 10, 19 });
        assert(ans == 3);
    }

    cout << "OK!" << endl;
}
