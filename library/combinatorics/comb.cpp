#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "comb.h"
#include "combDP.h"
#include "combMod.h"
#include "combModDP.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testComb() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Combination --------------------------------------" << endl;
    {
        int N = 30;

        CombAllDP dpA(N);
        CombCompactDP dpC(N);
        FastCombCompactDP dpFC(N);

        for (int r = 0; r <= N; r++) {
            assert(comb(N, r) == dpA.comb(N, r));
            assert(comb(N, r) == dpC.comb(r));
            assert(comb(N, r) == dpFC.comb(r));
        }

        N = 60;

        long long sum1 = 0, sum2 = 0;
        PROFILE_START(0);
        for (int i = 0; i < 10000; i++) {
            CombCompactDP dpC(N);
            sum1 += dpC.comb(N / 2);
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < 10000; i++) {
            FastCombCompactDP dpFC(N);
            sum2 += dpFC.comb(N / 2);
        }
        PROFILE_STOP(1);

        if (sum1 != sum2)
            cout << "Mismatched" << endl;
        assert(sum1 == sum2);
    }
    {
        int N = 70;
        int MOD = 1000000007;
#ifdef _DEBUG
        N = 30;
#endif

        CombAllModDP dpA(N, MOD);

        for (int r = 0; r <= N; r++) {
            assert(combMod(N, r, MOD) == dpA.comb(N, r));
            assert(combModPrime(N, r, MOD) == dpA.comb(N, r));
            assert(combLucasMod(N, r, MOD) == dpA.comb(N, r));
        }

        assert(combLucasMod(1000, 900, 13) == 8);
    }
    {
        int N = 70;
        int MOD = 1000000007;
#ifdef _DEBUG
        N = 30;
#endif

        CombCompactModDP dp(N, MOD);
        FastCombCompactModDP dpF(N, MOD);

        for (int r = 0; r <= N; r++) {
            assert(dp.comb(r) == dpF.comb(r));
        }

        long long sum1 = 0, sum2 = 0;
        PROFILE_START(2);
        for (int i = 0; i < 10000; i++) {
            CombCompactModDP dp(N, MOD);
            sum1 += dp.comb(N / 2);
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        for (int i = 0; i < 10000; i++) {
            FastCombCompactModDP dpF(N, MOD);
            sum2 += dpF.comb(N / 2);
        }
        PROFILE_STOP(3);

        if (sum1 != sum2)
            cout << "Mismatched" << endl;
        assert(sum1 == sum2);
    }
    {
        int MOD = 1117;
        CombLucasModDP dpL(MOD);

        long long NN = 13124314124214ll;
        for (long long r = 3124314124214ll; r <= NN; r += 124314124214ll) {
            assert(combLucasMod(NN, r, MOD) == dpL.comb(NN, r));
        }
    }

    cout << "OK!" << endl;
}
