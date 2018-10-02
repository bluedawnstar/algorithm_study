#include <vector>
#include <algorithm>

using namespace std;

#include "maxSubarraySum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#pragma warning(disable: 4334)

void testMaxSubarraySum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Subarray Sum -----------------------------------" << endl;
    {
        vector<int> a{ -2, -3, 4, -1, -2, 1, 5, -3 };

        assert(MaxSubarraySum::kadane(a) == 7);

        auto r = MaxSubarraySum::kadaneEx(a);
        assert(r == make_pair(7, make_pair(2, 6)));

        auto tblForward = MaxSubarraySum::buildTableForward(a);
        auto tblBackward = MaxSubarraySum::buildTableBackward(a);

        vector<pair<int, int>> gtTblForward{ {-2, 0}, {-3, 1}, {4, 2}, {3, 2}, {1, 2}, {2, 2}, {7, 2}, {4, 2} };
        vector<pair<int, int>> gtTblBackward{ {2, 6}, {4, 6}, {7, 6}, {3, 6}, {4, 6}, {6, 6}, {5, 6}, {-3, 7} };
        assert(tblForward == gtTblForward);
        assert(tblBackward == gtTblBackward);
    }

    cout << "OK!" << endl;
}
