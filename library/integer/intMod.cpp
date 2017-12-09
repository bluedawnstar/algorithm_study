#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

#include "intMod.h"
#include "eulerTheorem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testIntMod() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Modular Operations -------------------------" << endl;

    assert(modAdd(3, 7, 5) == 0);
    assert(modMul(3, 7, 5) == 1);

    {
        int MOD = 1000000007;
        long long a = 124124124214123412ll, b = 39039834038204432ll;
        assert(modMul(a, b, (long long)MOD) == modMul(int(a % MOD), int(b % MOD), MOD));
    }

    vector<int> modR = modInvPrimeRange(96, 97);
    for (int i = 1; i <= 1000; i++) {
        assert(modInv(i, 97) == modInvPrime(i, 97));
    }

    for (int i = 1; i < 97; i++) {
        assert(modInv(i, 97) == modR[i]);
    }

    cout << "phi() ========" << endl;
    for (int i = 1; i <= 100; i++) {
        cout << i << " : " << phi(i) << endl;
    }

    cout << "OK!" << endl;
}
