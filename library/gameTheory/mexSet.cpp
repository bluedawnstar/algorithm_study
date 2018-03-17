#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "mexSet.h"
#include "mexBitSet.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static int getMex(const vector<int>& in) {
    MexSet mex;
    for (auto x : in)
        mex.insert(x);
    int ans1 = mex.get();

    MexBitSet mexBS;
    for (auto x : in)
        mexBS.insert(x);
    int ans2 = mexBS.get();

    assert(ans1 == ans2);

    return ans1;
}

void testMexTrie() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Mex Set --------------------------" << endl;
    {
        assert(getMex(vector<int>{}) == 0);
        assert(getMex(vector<int>{ 1, 2, 3 }) == 0);
        assert(getMex(vector<int>{ 0, 2, 4, 6 }) == 1);
        assert(getMex(vector<int>{ 0, 1, 4, 7, 12 }) == 2);
    }

    cout << "OK!" << endl;
}
