#include <vector>
#include <algorithm>

using namespace std;

#include "lagrange.h"
#include "lagrangeMod.h"
#include "lagrangeModFast.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static const int MOD = 1000000007;

static int modPowSlow(int x, int n) {
    long long res = 1;
    while (n-- > 0)
        res = res * x % MOD;
    return int(res);
}

static int F(const vector<int>& coeff, int x) {
    long long res = 0;

    for (int i = 0; i < int(coeff.size()); i++)
        res = (res + 1ll * coeff[i] * modPowSlow(x, i) % MOD) % MOD;

    return int(res);
}

void testLagrange() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Lagrange's Interpolation ------------------------" << endl;
    {
        auto ans = LagrangePolynomial::interpolate(vector<pair<int, int>>{ { 0, 2 }, { 1, 3 }, { 2, 12 }, { 5, 147 } }, 3);
        cout << "f(3) = " << ans << endl;
        assert(ans == 35);
    }
    {
        auto ans = LagrangePolynomialMod<MOD>::interpolate(vector<pair<int, int>>{ { 0, 2 }, { 1, 3 }, { 2, 12 }, { 5, 147 } }, 3);
        cout << "f(3) = " << ans << endl;
        assert(ans == 35);
    }
    {
        int T = 100;
        int N = 100;

        while (T-- > 0) {
            vector<int> coeff(N + 1);
            for (int i = 0; i <= N; i++)
                coeff[i] = RandInt32::get() % 1000000;

            vector<pair<int, int>> YY(N + 1);
            vector<int> Y(N + 1);
            for (int i = 1; i <= N + 1; i++) {
                Y[i - 1] = F(coeff, i);
                YY[i - 1].first = i;
                YY[i - 1].second = Y[i - 1];
            }

            FastLagrangePolynomialMod<MOD> interp(N);

            for (int i = 0; i < T; i++) {
                int x = RandInt32::get();

                int ans1 = LagrangePolynomialMod<MOD>::interpolate(YY.data(), N + 1, x);
                int ans2 = interp.interpolate(Y, x);
                int gt = F(coeff, x);

                if (gt != ans1 || gt != ans2)
                    cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << gt << endl;
                assert(ans1 == gt && ans2 == gt);
            }
        }
    }

    cout << "OK!" << endl;
}
