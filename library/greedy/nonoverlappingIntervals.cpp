#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

#include "nonoverlappingIntervals.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

// https://leetcode.com/problems/non-overlapping-intervals/

void testNonoverlappingIntervals() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Non-overlapping Intervals ------------------------------" << endl;
    {
        vector<pair<int, int>> in{ { 1, 3 }, { 2, 5 }, { 4, 7 }, { 6, 9 }, { 8, 10 } };
        auto ans = countNonoverlapIntervals(in);
        if (ans != 3)
            cout << "invalid answer : " << 3 << ", " << ans << endl;
        assert(ans == 3);
    }

    cout << "OK!" << endl;
}
