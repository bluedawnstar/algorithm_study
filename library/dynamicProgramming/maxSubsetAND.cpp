#include <cstring>
#include <numeric>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "maxSubsetAND.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static long long solveNaive(long long L, long long R, long long K) {
    long long res = 0;
    function<void(long long, long long, long long K, long long)> dfs;
    dfs = [&](long long L, long long R, long long K, long long value) {
        if (K == 0) {
            res = max(res, value);
            return;
        }

        if (R - L + 1 < K)
            return;

        for (; L <= R; L++) {
            dfs(L + 1, R, K - 1, value & L);
        }
    };
    dfs(L, R, K, 0xffffffffffffffffll);

    return res;
}

void testMaxSubsetAND() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Subset AND ------------------------" << endl;
    {
#ifdef _DEBUG
        const int MINX = 110;
        const int MAXX = 128;
#else
        const int MINX = 1000;
        const int MAXX = 1024;
#endif
        for (int R = MINX; R <= MAXX; R++) {
            for (int L = MINX; L <= R; L++) {
                for (int K = 1; K <= R - L + 1; K++) {
                    long long gt = solveNaive(L, R, K);
                    long long ans = MaxSubsetANDValue::calcMaxSubsetAND(L, R, K);
                    if (ans != gt) {
                        cout << "Mismatched in (" << L << "," << R << "," << K << ") = " << ans << ", " << gt << endl;
                    }
                    assert(ans == gt);
                }
            }
        }
    }

    cout << "OK!" << endl;
}
