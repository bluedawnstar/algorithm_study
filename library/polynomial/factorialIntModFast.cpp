#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

#include "factorialIntModFast.h"
#include "factorialIntModFast_standalone.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

const int MOD = 1000000007;

static int factorialModSlow(int n) {
    long long res = 1;
    while (n > 1)
        res = res * n-- % MOD;
    return int(res);
}

void testFactorialIntModFast() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast Modular Integer Factorial ----------------" << endl;
    {
        const int T = 10;

        FastFactorialIntMod<MOD> factorial(int(sqrt(MOD)) * 10);
        FastFactorialIntModStandalone factorial2(int(sqrt(MOD)) * 10, MOD);
        for (int i = 0; i < T; i++) {
            int x = RandInt32::get() % (MOD - 1) + 1;

            auto gt = factorialModSlow(x);
            auto ans = factorial.factorial(x);
            auto ans2 = factorial2.calcFactorial(x);
            if (ans != gt || ans2 != gt) {
                cout << "Mismatched : " << x << "! = " << ans << ", " << ans2 << ", " << gt << endl;
            }
            assert(ans == gt && ans2 == gt);
        }
    }

    cout << "OK!" << endl;
}
