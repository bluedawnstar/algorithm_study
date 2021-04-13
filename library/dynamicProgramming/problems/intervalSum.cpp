#include <functional>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "intervalSum.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"

void testIntervalSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Interval Sum ------------------------" << endl;
    {
        const int N = 4, K = 3;
        const vector<tuple<int, int, int>> values{
            { 0, 0, -4 },
            { 0, 2,  0 },
            { 0, 3, -3 },
            { 1, 1, -2 },
            { 2, 2,  5 },
            { 3, 3,  2 }
        };
        const vector<long long> gt{ 7, 5, 5 };

        auto ans1 = IntervalSum::solveDP(N, K, values);
        auto ans2 = IntervalSum::solveDP(N, K, values);
        assert(ans1 == gt);
        assert(ans2 == gt);
    }

    cout << "OK!" << endl;
}
