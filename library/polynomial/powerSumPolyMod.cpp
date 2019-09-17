#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "powerSumPolyMod.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include "../math/sumOfPowerFast.h"

static int modPow(int x, int n, int M) {
    if (n == 0)
        return 1;

    long long t = x % M;
    long long res = 1;
    for (; n > 0; n >>= 1) {
        if (n & 1)
            res = res * t % M;
        t = t * t % M;
    }
    return int(res);
}

static int sumOfPower(int x, int k, int mod) {
    int res = 0;
    for (int i = 1; i <= x; i++)
        res = (res + modPow(i, k, mod)) % mod;
    return res;
}

void testPowerSumPolyMod() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Polynomial of Power Sum style ----------------------------" << endl;
    {
        const int MOD = 1000000007;

        int N = 100000000 - 1;
        int K = 10000 - 1;

#ifdef _DEBUG
        N = 10000000 - 1;
        K = 1000 - 1;
#endif

        FastSumOfPowerMod<MOD> fsop(K);
        PowerSumPolyMod<MOD> psp(K);

        vector<int> Y(K + 2);
        Y[0] = modPow(1, K, MOD);
        for (int i = 1; i < K + 2; i++)
            Y[i] = (Y[i - 1] + modPow(i + 1, K, MOD)) % MOD;    // Y[i] = SUM_(1..i) { i^k }

        int ans = psp.solve(Y, N, K);
        int gt = sumOfPower(N, K, MOD);
        if (ans != gt)
            cout << "Mismatched : " << "(" << N << ", " << K << ") = " << ans << ", " << ", " << gt << endl;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
