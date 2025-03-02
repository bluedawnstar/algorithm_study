#include <math.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "comb.h"
#include "combMod.h"
#include "combModGeneral.h"
#include "combModCRT.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

// related problems
// - https://leetcode.com/problems/check-if-digits-are-equal-in-string-after-operations-ii

void testCombModCRT() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Binomial Coefficients - with Chinese Remainder Theorem ----" << endl;
    {
        int N = 1000;
        long long M = 3 * 5 * 7 * 31 * 41;

        vector<int> P{ 3, 5, 7, 31, 41 };

        auto combGeneral = CombModGeneral(N, M);
        auto combCRT = CombModCRT(N, M);

        for (int n = 1; n <= N; n++) {
            auto tblC = combGeneral.buildCombTable(n);
            auto tblC2 = CombModGeneral::buildCombTable(n, M);
            for (int r = 0; r <= n; r++) {
                auto ans = combGeneral.comb(n, r);
                auto ans2 = combCRT.combLucas(n, r);
                auto ans3 = CombModCRT::combLucas(n, r, P);
                if (ans != ans2 || ans != ans3 || ans != tblC[r] || ans != tblC2[r]) {
                    cout << "Mismatched at " << __LINE__ << " : "
                        << "C(" << n << ", " << r << "): ans = " << ans
                                                   << ", ans2 = " << ans2
                                                   << ", ans3 = " << ans3
                                                   << ", tblC = " << tblC[r]
                                                   << ", tblC2 = " << tblC2[r] << endl;
                }
                assert(ans == ans2);
                assert(ans == ans3);
                assert(ans == tblC[r]);
                assert(ans == tblC2[r]);
            }
        }
    }

    cout << "OK!" << endl;
}
