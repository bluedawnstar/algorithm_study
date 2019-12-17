#include <complex>
#include <vector>
#include <algorithm>

using namespace std;

#include "polynomial.h"
#include "modInt.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "polyFFTMod.h"

#include "lagrange.h"
#include "lagrangeMod.h"
#include "lagrangeModFast.h"

using namespace algebra;

const int MOD = 1000000007;
typedef ModInt<int, MOD> baseT;
//typedef ModInt<long long, MOD> baseT;
typedef poly<baseT> polyT;

//---

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

void testPolynomial() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Polynomial ------------------------" << endl;
    // multiplication
    {
        const int N = 1000;

        for (int i = 0; i < 10; i++) {
            vector<int> A(N);
            vector<int> B(N);

            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MOD;
            for (int i = 0; i < N; i++)
                B[i] = RandInt32::get() % MOD;

            vector<int> out1 = PolyFFTMod<MOD>::multiplySlow(A, B);
            vector<int> out2 = PolyFFTMod<MOD>::multiply(A, B);
            if (out1 != out2)
                cout << "ERROR at " << __LINE__ << endl;
            assert(out1 == out2);

            auto out3 = polyT(A) * polyT(B);
            if (polyT(out1) != out3)
                cout << "ERROR at " << __LINE__ << endl;
            assert(polyT(out1) == out3);
        }
    } 
    {
        int n = 100000;
#ifdef _DEBUG
        n = 1000;
#endif
        polyT a;
        vector<baseT> x;
        
        for (int i = 0; i <= n; i++) {
            a.a.push_back(1 + RandInt32::get() % 100);
            x.push_back(1 + RandInt32::get() % (2 * n));
        }
        sort(begin(x), end(x));
        x.erase(unique(begin(x), end(x)), end(x));

        auto b = a.eval(x);
        auto c = interpolate(x, b);
        polyT md = kmul(begin(x), end(x));

        if (c != a % md)
            cout << "Mismatched!" << endl;
        assert(c == a % md);
    }
    // interpolation
    {
        {
            auto ans = LagrangePolynomialMod<MOD>::interpolate(vector<pair<int, int>>{ { 0, 2 }, { 1, 3 }, { 2, 12 }, { 5, 147 } }, 3);
            cout << "f(3) = " << ans << endl;
            assert(ans == 35);

            auto ans2 = interpolate(vector<baseT>{0, 1, 2, 5}, vector<baseT>{2, 3, 12, 147}).eval(baseT(3));
            cout << "f(3) = " << ans2 << endl;
            assert(ans2.value == 35);
        }
        {
            int T = 100;
            int N = 100;

            while (T-- > 0) {
                vector<int> coeff(N + 1);
                for (int i = 0; i <= N; i++)
                    coeff[i] = RandInt32::get() % 1000000;

                vector<int> X(N + 1);
                vector<pair<int, int>> YY(N + 1);
                vector<int> Y(N + 1);
                for (int i = 1; i <= N + 1; i++) {
                    X[i - 1] = i;
                    Y[i - 1] = F(coeff, i);
                    YY[i - 1].first = i;
                    YY[i - 1].second = Y[i - 1];
                }

                FastLagrangePolynomialMod<MOD> interp(N);

                //vector<baseT> modX, modY;
                //copyFrom(modX, X);
                //copyFrom(modY, Y);
                //auto poly = interpolate(modX, modY);
                auto poly = interpolate(copyFrom<int, MOD>(X), copyFrom<int, MOD>(Y));

                for (int i = 0; i < T; i++) {
                    int x = RandInt32::get();

                    int ans1 = LagrangePolynomialMod<MOD>::interpolate(YY.data(), N + 1, x);
                    int ans2 = interp.interpolate(Y, x);
                    auto ans3 = poly.eval(baseT(x));
                    int gt = F(coeff, x);

                    if (gt != ans1 || gt != ans2 || gt != ans3.value)
                        cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << ans3 << ", " << gt << endl;
                    assert(ans1 == gt && ans2 == gt && ans3.value == gt);
                }
            }
        }
    }
    cout << "*** Speed test ***" << endl;
    {
        for (int n = 32; n <= 2048; n <<= 1) {
            vector<int> in1(n);
            vector<int> in2(n);
            for (int i = 0; i < n; i++) {
                in1[i] = RandInt32::get() % 1024;
                in2[i] = RandInt32::get() % 1024;
            }

            vector<int> out1, out2;

            cout << "N = " << n << endl;

            cout << "  PolyFFTMod::multiplySlow() : ";
            PROFILE_START(0);
            for (int i = 0; i < 1000; i++)
                out1 = PolyFFTMod<MOD>::multiplySlow(in1, in2);
            PROFILE_STOP(0);

            cout << "  PolyFFTMod::multiply() : ";
            PROFILE_START(1);
            for (int i = 0; i < 1000; i++)
                out2 = PolyFFTMod<MOD>::multiply(in1, in2);
            PROFILE_STOP(1);

            cout << "  Polynomial : ";
            polyT A(in1), B(in2), out3;
            PROFILE_START(2);
            for (int i = 0; i < 1000; i++)
                out3 = A * B;
            PROFILE_STOP(2);

            if (out1 != out2 || polyT(out1) != out3)
                cout << "ERROR at " << __LINE__ << endl;
            assert(out1 == out2 && polyT(out1) == out3);
        }
    }

    cout << "OK!" << endl;
}
