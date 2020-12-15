#include <cmath>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "discreteSqrt.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

const int MOD = 1000000007;

void testDiscreteSqrt() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Discrete Sqrt -----------------------" << endl;
    {
        auto x = DiscreteSqrt32bit::solve(1, 3);
        assert(x * x % 3 == 1);

        auto x2 = DiscreteSqrt32bit::solve(2, 3);
        assert(x2 == -1);
    }
    {
        const int T = 100;
        for (int tt = 0; tt < T; tt++) {
            int x = RandInt32::get() % MOD;
            auto xx = int(1ll * x * x % MOD);

            int sqrtXX = DiscreteSqrt32bit::solve(xx, MOD);
            assert(sqrtXX == x || (MOD - sqrtXX) == x);
        }
    }
    cout << "OK!" << endl;
}
