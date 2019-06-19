#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "sumOfPower.h"
#include "sumOfPowerFast.h"

// https://www.hackerrank.com/contests/infinitum-mar14/challenges/summing-the-k-n-series


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

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

void testSumOfPower() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sum of Power -------------------------" << endl;
    {
        const int MOD = 1000000007;

        int N = 100;
        int K = 100;

        SumOfPowerMod sop(K, MOD);
        FastSumOfPowerMod fsop(K, MOD);

        for (int i = 1; i <= N; i++) {
            for (int j = 0; j <= K; j++) {
                int ans = sop.faulhaberMod(i, j);
                int ans2 = fsop.faulhaberMod(i, j);
                int gt = sumOfPower(i, j, MOD);
                if (ans != gt || ans2 != gt)
                    cout << "Mismatched : " << "(" << i << ", " << j << ") = " << ans << ", " << ", " << ans2 << ", " << gt << endl;
                assert(ans == gt && ans2 == gt);
            }
        }
    }
    {
        const int MOD = 1000000007;

        int N = 100000000 - 1;
        int K = 10000 - 1;

#ifdef _DEBUG
        N = 10000000 - 1;
        K = 1000 - 1;
#endif

        SumOfPowerMod sop(K, MOD);
        FastSumOfPowerMod fsop(K, MOD);

        int ans = sop.faulhaberMod(N, K);
        int ans2 = fsop.faulhaberMod(N, K);
        int gt = sumOfPower(N, K, MOD);
        if (ans != gt || ans2 != gt)
            cout << "Mismatched : " << "(" << N << ", " << K << ") = " << ans << ", " << ", " << ans2 << ", " << gt << endl;
        assert(ans == gt && ans2 == gt);
    }
    cout << "*** speed test ..." << endl;
    {
        const int MOD = 1000000007;
        int N = 100000000 - 1;
        int K = 10000 - 1;

#ifdef _DEBUG
        N = 10000000 - 1;
        K = 1000 - 1;
#endif

        PROFILE_START(0);
        SumOfPowerMod sop(K, MOD);
        long long sum1 = 0;
        for (int i = 0; i <= K; i++)
            sum1 += sop.faulhaberMod(N, i);
        PROFILE_STOP(0);

        PROFILE_START(1);
        FastSumOfPowerMod fsop(K, MOD);
        long long sum2 = 0;
        for (int i = 0; i <= K; i++)
            sum2 += fsop.faulhaberMod(N, i);
        PROFILE_STOP(1);

        if (sum1 != sum2)
            cout << "ERROR!" << endl;
        assert(sum1 == sum2);
    }

    cout << "OK!" << endl;
}
