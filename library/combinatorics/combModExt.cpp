#include <math.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "comb.h"
#include "combMod.h"
#include "combModExt.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testCombModExt() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Combination - Extended --------------------------------------" << endl;
    {
        int N = 1000;
        //int P = 761, b = 3;
        int P = 31, b = 3;
        long long M = 1ll;

        for (int i = 1; i <= b; i++)
            M *= P;

        auto combPPow = CombModPPow(N, P, b);
        auto combGeneral = CombModGeneral(N, M, vector<pair<int, int>>{ { P, b } });

        for (int n = 1; n <= N; n++) {
            for (int r = 0; r <= n; r++) {
                auto ans = combPPow.comb(n, r);
                auto ans2 = combGeneral.comb(n, r);
                if (ans != ans2) {
                    cout << "C(" << n << ", " << r << "): ans = " << ans << ", ans2 = " << ans2 << endl;
                }
                assert(ans == ans2);
            }
        }
    }

    cout << "OK!" << endl;
}
