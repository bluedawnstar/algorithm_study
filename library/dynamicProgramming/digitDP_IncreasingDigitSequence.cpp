#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "digitDP_IncreasingDigitSequenceNaive.h"
#include "digitDP_IncreasingDigitSequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

const int MOD = 1000000007;

// https://www.codechef.com/DEC19A/problems/APAIRS

static long long calcScore(long long L, long long R) {
    auto dp = IncreasingDigitSequenceMod<MOD>::digitCount(L, R);

    long long res = 0;
    for (int i = 0; i < int(dp.size()); i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = j + 1; k < 10; k++) {
                res = (res + dp[i][j] * dp[i][k] % MOD * 2ll * (k - j)) % MOD;
            }
        }
    }

    return res;
}

static long long calcScoreNaive(long long L, long long R) {
    auto dp = IncreasingDigitSequenceNaive::digitCount(L, R);

    long long res = 0;
    for (int i = 0; i < int(dp.size()); i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = j + 1; k < 10; k++) {
                res = (res + dp[i][j] * dp[i][k] % MOD * 2ll * (k - j)) % MOD;
            }
        }
    }

    return res;
}


void testDigitDP_IncreasingDigitSequence() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Digit DP - Increasing Digit Sequence ------------------------------" << endl;
    {
        int MAXR = 1000;
#ifdef _DEBUG
        MAXR = 100;
#endif
        for (int L = 0; L <= MAXR; L++) {
            for (int R = L; R <= MAXR; R++) {
                auto ans1 = calcScoreNaive(L, R);
                auto ans2 = calcScore(L, R);
                if (ans1 != ans2)
                    cout << "Mismatched in [" << L << ", " << R << "] = " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
#ifndef _DEBUG
    {
        long long MAXR = 100011010000ll;
        long long range = 2;
        for (long long L = MAXR - range; L <= MAXR; L++) {
            for (long long R = L; R <= MAXR; R++) {
                auto ans1 = calcScoreNaive(L, R);
                auto ans2 = calcScore(L, R);
                if (ans1 != ans2)
                    cout << "Mismatched in [" << L << ", " << R << "] = " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
#endif
    cout << "OK!" << endl;
}
