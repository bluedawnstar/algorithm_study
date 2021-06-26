#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "primeFactorSimple.h"
#include "primeNumberBasic.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <assert.h>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define NN  100'000

void testPrimeFactorSimple() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Prime Factor -------------------" << endl;

    {
        SimplePrimeFactor simplePF(100);

        for (int i = 0; i <= 100; i++) {
            auto ans1 = simplePF.isPrime(i);
            auto ans2 = isPrimeNumber(i);
            if (ans1 != ans2)
                cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }

        for (int i = 0; i <= 100; i++) {
            auto ans1 = simplePF.getPrimeFactors(i);
            auto ans2 = getPrimeFactors2(i);

            sort(ans1.begin(), ans1.end());
            sort(ans2.begin(), ans2.end());

            if (ans1 != ans2)
                cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }
    {
        SimplePrimeFactor simplePF(NN);

        for (int x = 1; x <= NN; x++) {
            auto pf1 = simplePF.getPrimeFactors(x);
            auto pf2 = getPrimeFactors2(x);

            sort(pf1.begin(), pf1.end());
            sort(pf2.begin(), pf2.end());

            if (pf1 != pf2) {
                cout << "Mismatched : " << pf1 << ", " << pf2 << endl;
            }
            assert(pf1 == pf2);
        }
    }

    cout << "OK!" << endl;
}
