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
    //return; //TODO: if you want to test, make this line a comment.

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
    // range query : X = { 1, 2, 3, ..., n + 1 }
    {
        int T = 100;
        int N = 10;

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

            FastLagrangePolynomialMod<MOD> interp(N * 5);

            for (int i = 0; i < T; i++) {
                auto ans = interp.interpolateRange(Y, int(Y.size()) * 4);
                for (int x = 1; x <= int(ans.size()); x++) {
                    int gt = F(coeff, x);
                    if (gt != ans[x - 1])
                        cout << "Mismatched : f(" << x << ") = " << ans[x - 1] << ", " << gt << endl;
                    assert(ans[x - 1] == gt);
                }
            }
        }
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
            for (int i = 0; i <= N; i++) {
                Y[i] = F(coeff, i);
                YY[i].first = i;
                YY[i].second = Y[i];
            }

            FastLagrangePolynomialMod<MOD> interp(N);

            for (int i = 0; i < T; i++) {
                int x = RandInt32::get();

                int ans1 = LagrangePolynomialMod<MOD>::interpolate(YY.data(), N + 1, x);
                int ans2 = interp.interpolate0(Y, x);
                int gt = F(coeff, x);

                if (gt != ans1 || gt != ans2)
                    cout << "Mismatched : f(" << x << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
                assert(ans1 == gt && ans2 == gt);
            }
        }
    }
    // range query : X = { 0, 1, 2, ..., n }
    {
        int T = 100;
        int N = 10;

        while (T-- > 0) {
            vector<int> coeff(N + 1);
            for (int i = 0; i <= N; i++)
                coeff[i] = RandInt32::get() % 1000000;

            vector<pair<int, int>> YY(N + 1);
            vector<int> Y(N + 1);
            for (int i = 0; i <= N; i++) {
                Y[i] = F(coeff, i);
                YY[i].first = i;
                YY[i].second = Y[i];
            }

            FastLagrangePolynomialMod<MOD> interp(N * 5);

            for (int i = 0; i < T; i++) {
                auto ans = interp.interpolateRange0(Y, int(Y.size()) * 4);
                for (int x = 0; x < int(ans.size()); x++) {
                    int gt = F(coeff, x);
                    if (gt != ans[x])
                        cout << "Mismatched : f(" << x << ") = " << ans[x] << ", " << gt << endl;
                    assert(ans[x] == gt);
                }
            }
        }
    }

    cout << "OK!" << endl;
}
