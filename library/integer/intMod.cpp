#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

#include "intMod.h"
#include "intModExt.h"
#include "eulerTheorem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testIntMod() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Operations -------------------------" << endl;
    {
        assert(modAdd(3, 7, 5) == 0);
        assert(modMul(3, 7, 5) == 1);
    }
    {
        int MOD = 1000000007;
        long long a = 124124124214123412ll, b = 39039834038204432ll;
        assert(modMul(a, b, (long long)MOD) == modMul(int(a % MOD), int(b % MOD), MOD));
        assert(modMul2(a, b, (long long)MOD) == modMul(int(a % MOD), int(b % MOD), MOD));
    }
    {
        vector<int> modR = modInvPrimeRange(96, 97);
        for (int i = 1; i <= 1000; i++) {
            assert(modInv(i, 97) == modInvPrime(i, 97));
        }

        for (int i = 1; i < 97; i++) {
            assert(modInv(i, 97) == modR[i]);
        }
    }
    {
        auto ans1 = factorialModNaive(25, 29);
        auto ans2 = factorialModWilson(25, 29);
        auto ans3 = factorialMod(25, 29);
        assert(ans1 == ans2);
        assert(ans1 == ans3);
        assert(ans1 == 5);
    }
    {
        cout << "phi() ========" << endl;
        for (int i = 1; i <= 100; i++) {
            cout << i << " : " << phi(i) << endl;
        }
    }
    cout << "OK!" << endl;
}
