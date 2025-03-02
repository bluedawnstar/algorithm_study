#include <math.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "comb.h"
#include "combMod.h"
#include "combModGeneral.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

// related problems
// - https://leetcode.com/problems/check-if-digits-are-equal-in-string-after-operations-ii

void testCombModGeneral() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Binomial Coefficients - General --------------------------------" << endl;
    {
        int N = 1000;
        long long M = 31 * 31 * 31;

        auto combGeneral = CombModGeneral(N, M);

        for (int n = 1; n <= N; n++) {
            auto tblC = combGeneral.buildCombTable(n);
            auto tblC2 = CombModGeneral::buildCombTable(n, M);
            for (int r = 0; r <= n; r++) {
                auto ans = combGeneral.comb(n, r);
                if (ans != tblC[r] || ans != tblC2[r]) {
                    cout << "Mismatched at " << __LINE__ << " : "
                         << "C(" << n << ", " << r << "): ans = " << ans
                                                    << ", ans2 = " << tblC[r]
                                                    << ", ans3 = " << tblC2[r] << endl;
                }
                assert(ans == tblC[r]);
                assert(ans == tblC2[r]);
            }
        }
    }
    {
        int N = 1000;
        long long M = 5 * 5 * 7 * 7 * 31 * 31 * 31;

        auto combGeneral = CombModGeneral(N, M);

        for (int n = 1; n <= N; n++) {
            auto tblC = combGeneral.buildCombTable(n);
            auto tblC2 = CombModGeneral::buildCombTable(n, M);
            for (int r = 0; r <= n; r++) {
                auto ans = combGeneral.comb(n, r);
                if (ans != tblC[r] || ans != tblC2[r]) {
                    cout << "Mismatched at " << __LINE__ << " : "
                         << "C(" << n << ", " << r << "): ans = " << ans
                                                    << ", ans2 = " << tblC[r]
                                                    << ", ans3 = " << tblC2[r] << endl;
                }
                assert(ans == tblC[r]);
                assert(ans == tblC2[r]);
            }
        }
    }

    cout << "OK!" << endl;
}
