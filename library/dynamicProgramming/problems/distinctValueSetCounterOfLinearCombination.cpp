#include <functional>
#include <numeric>
#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "distinctValueSetCounterOfLinearCombination.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"

void testDistinctValueSetCounterOfLinearCombination() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Distinct Value Set Counter of Linear Combination -----" << endl;
    {
        int ans = DistinctValueSetCounterOfLinearCombination<>::solve(vector<int>{ 1, 1, 1 });
        assert(ans == 56406);
    }
    cout << "OK!" << endl;
}
