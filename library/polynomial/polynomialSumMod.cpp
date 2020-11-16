#include <numeric>
#include <vector>
#include <algorithm>

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "polynomialSumMod.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

//<Related problems>
// https://www.codechef.com/problems/CHEFSSM

const int MOD = 998'244'353;
const int ROOT = 3;

static int calcF(const vector<int>& C, int x) {
    // F(x) = (x - C[0])(x - C[1])...(x - C[n-1])
    long long res = 1ll;
    for (int cc : C) {
        res = res * (x - cc) % MOD;
        if (res < 0)
            res += MOD;
    }
    return int(res);
}

static int calcG(const vector<int>& C, int k) {
    long long res = 0;
    for (int x = 0; x <= k; x++) {
        res += calcF(C, x);
        if (res >= MOD)
            res -= MOD;
    }
    return int(res);
}

void testPolynomialSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Polynomial Sum -----------------------" << endl;
    {
#ifdef _DEBUG
        const int T = 10;
        const int N = 100;
        const int MAXX = 10'000;
#else
        const int T = 20;
        const int N = 100;
        const int MAXX = 100'000;
#endif

        PolynomialSumNTT<MOD, ROOT> solver1(N + 2);
        PolynomialSumFFT<MOD> solver2(N + 2);

        vector<int> C(N);
        for (int i = 0; i < N; i++)
            C[i] = RandInt32::get() % MAXX;

        for (int tt = 0; tt < T; tt++) {
            int k = RandInt32::get() % 1'000'000;
            int ans1 = solver1.solve(C, k);
            int ans2 = solver2.solve(C, k);
            int gt = calcG(C, k);

            if (ans1 != gt || ans2 != gt) {
                cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << gt << endl;
            }
            assert(ans1 == gt && ans2 == gt);
        }
    }

    cout << "OK!" << endl;
}
