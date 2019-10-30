#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

#include "overlappingIntervals.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testOverlappingIntervals() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Overlapping Intervals ------------------------------" << endl;
    {
        vector<pair<int, int>> in{ { 1, 3 }, { 2, 5 }, { 4, 7 }, { 6, 9 }, { 8, 10 } };
        auto ans = countOverlapIntervals(in);
        if (ans != 3)
            cout << "invalid answer : " << 3 << ", " << ans << endl;
        assert(ans == 3);
    }

    cout << "OK!" << endl;
}
