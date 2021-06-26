#include <cmath>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

#include "intMod.h"
#include "discreteLog.h"
#include "discreteLogFast.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

const int MOD = 1000000007;

void testDiscreteLog() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Discrete Log -----------------------" << endl;
    {
        FastDiscreteLog dsc;

#ifndef _DEBUG
        const int MAXA = 200;
        const int MAXX = 200;
#else
        const int MAXA = 31;
        const int MAXX = 31;
#endif
        for (int a = 2; a <= MAXA; a++) {
            for (int x = 2; x <= MAXX; x++) {
                int b = modPow(a, x, MOD);

                auto x2 = dsc.solve(a % MOD, b % MOD, MOD);
                auto b2 = modPow(a, int(x2), MOD);

                if (b2 != b || x2 > x) {
                    cout << "Mismatched : " << a << "^" << x << " = " << b << " => " << x2 << endl;
                    cout << "             " << a << "^" << x2 << " = " << b2 << endl;
                }
                assert(b == b2);
            }
        }
    }
    cout << "OK!" << endl;
    {
#ifndef _DEBUG
        const int MAXA = 200;
        const int MAXX = 200;
#else
        const int MAXA = 31;
        const int MAXX = 31;
#endif
        for (int a = 2; a <= MAXA; a++) {
            for (int x = 2; x <= MAXX; x++) {
                int b = modPow(a, x, MOD);
                auto x2 = DiscreteLog::solve32bit(a, b, MOD);
                auto b2 = modPow(a, int(x2), MOD);

                if (b2 != b || x2 > x) {
                    cout << "Mismatched : " << a << "^" << x  << " = " << b  << " => " << x2 << endl;
                    cout << "             " << a << "^" << x2 << " = " << b2 << endl;
                }
                assert(b == b2);
            }
        }
    }
    cout << "OK!" << endl;
}
