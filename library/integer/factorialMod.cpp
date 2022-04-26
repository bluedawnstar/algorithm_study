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

#include "primeNumberEratosthenes.h"

#ifdef _DEBUG
const int MOD = 10'007;
#else
const int MOD = 1'000'000'007;
#endif

void testFactorialMod() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Factorial -------------------------" << endl;

    cout << "calculate prime numbers..." << endl;
    vector<int> primes = eratosthenes2(MOD);
    cout << "... done!" << endl;

    {
        FastFactorialMod2 fastFact2(29);

        auto ans1 = FactorialMod::factorialNaive(25, 29);
        auto ans2 = FactorialMod::factorialWilson(25, 29);
        auto ans3 = FastFactorialMod::factorial(25, 29);
        auto ans4 = FastFactorialMod::factorialFast(25, 29);
        auto ans5 = fastFact2.factorial(25);
        auto ans6 = FactorialModWithSieve::factorial(primes, 25, 29);
        if (ans1 != ans2 || ans1 != ans3 || ans1 != ans4 || ans1 != ans5 || ans1 != ans6 || ans1 != 5)
            cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << ans3 << ", " << ans4 << ", " << ans5 << ", " << ans6 << endl;
        assert(ans1 == ans2);
        assert(ans1 == ans3);
        assert(ans1 == ans4);
        assert(ans1 == ans5);
        assert(ans1 == ans6);
        assert(ans1 == 5);
    }
    {
        FastFactorialMod2 fastFact2(29);

        auto ans1 = FastFactorialMod::factorial(29, 29);
        auto ans2 = FastFactorialMod::factorialFast(29, 29);
        auto ans3 = fastFact2.factorial(29);
        if (ans1 != ans2 || ans1 != ans3 || ans1 != 28)
            cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << ans3 << endl;
        assert(ans1 == ans2);
        assert(ans1 == ans3);
        assert(ans1 == 28);
    }
    {
        const int T = 100;
        for (int i = 0; i < T; i++) {
            int x = RandInt32::get() % 10000 + 1;

            auto ans1 = FactorialMod::factorialNaive(x, MOD);
            auto ans2 = FastFactorialMod::factorial(x, MOD);
            auto ans3 = FactorialModWithSieve::factorial(primes, x, MOD);
            if (ans1 != ans2 || ans1 != ans3)
                cout << "Mismatched : " << x << "! = " << ans1 << ", " << ans2 << ", " << ans3 << endl;
            assert(ans1 == ans2);
            assert(ans1 == ans3);
        }
    }

    cout << "OK!" << endl;
}
