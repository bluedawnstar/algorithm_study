#include <vector>
#include <algorithm>

using namespace std;

#include "permutationCountByDivisibleAdjacentPairCount.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testPermutationCounterByDivisibleAdjacentPairCount() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Permutation Counter By Divisible Adjacent Pair Count -------" << endl;
    {
        int N = 5;
        vector<bool> possibleScale{
            false,
            true,   // 1
            false,  // 2
            false,  // 3
            false,  // 4
            true    // 5
        };

        vector<int> gt{ 96, 24, 0, 0, 0 };

        auto ans = PermutationCounterByDivisibleAdjacentPairCount<>::solve(possibleScale, N);
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
