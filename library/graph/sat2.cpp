#include <climits>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "sat2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testSAT2() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- 2-SAT -----------------" << endl;

    // (a + b) * (b + -c)
    auto ans = SAT2::solve2Sat(3, vector<int>{ SAT2::nodeIndex(0, false), SAT2::nodeIndex(1, false) },
                                  vector<int>{ SAT2::nodeIndex(1, false), SAT2::nodeIndex(2, true) });
    cout << ans << endl;
    assert(!ans.empty());

    ans = SAT2::solve2Sat(2, vector<int>{ SAT2::nodeIndex(0, false), SAT2::nodeIndex(0, true), SAT2::nodeIndex(0, false), SAT2::nodeIndex(0, true) },
                             vector<int>{ SAT2::nodeIndex(1, false), SAT2::nodeIndex(1, false), SAT2::nodeIndex(1, true), SAT2::nodeIndex(1, true) });
    assert(ans.empty());

    cout << "OK" << endl;
}
