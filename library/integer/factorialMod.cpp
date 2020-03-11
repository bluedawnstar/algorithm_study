#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

#include "factorialMod.h"
#include "factorialModFast.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testFactorialMod() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Factorial -------------------------" << endl;
    {
        auto ans1 = factorialModNaive(25, 29);
        auto ans2 = factorialModWilson(25, 29);
        auto ans3 = factorialMod(25, 29);
        auto ans4 = FactorialModWithSieve::factorial(25, 29);
        if (ans1 != ans2 || ans1 != ans3 || ans1 != ans4 || ans1 != 5)
            cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << ans3 << ", " << ans4 << endl;
        assert(ans1 == ans2);
        assert(ans1 == ans3);
        assert(ans1 == ans4);
        assert(ans1 == 5);
    }
    {
        const int T = 100;
        const int MOD = 1000000007;
        for (int i = 0; i < T; i++) {
            int x = RandInt32::get() % 10000 + 1;

            auto ans1 = factorialModNaive(x, MOD);
            auto ans2 = factorialMod(x, MOD);
            auto ans3 = FactorialModWithSieve::factorial(x, MOD);
            if (ans1 != ans2 || ans1 != ans3)
                cout << "Mismatched : " << x << "! = " << ans1 << ", " << ans2 << ", " << ans3 << endl;
            assert(ans1 == ans2);
            assert(ans1 == ans3);
        }
    }

    cout << "OK!" << endl;
}
