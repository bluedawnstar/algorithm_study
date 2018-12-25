#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primalityTestInt32.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "primalityTest.h"
#include "primeNumberBasic.h"

template <typename T>
inline bool isPrimeNumberSlow(T x) {
    if (x < 2)
        return false;
    else if (x == 2)
        return true;
    else if ((x & 1) == 0)
        return false;

    T root = (T)(sqrt(x));
    for (T i = 3; i <= root; i += 2) {
        if (x % i == 0)
            return false;
    }

    return true;
}

void testPrimalityTestInt32() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast Primality Test for Int32 -------------------" << endl;
    {
        for (int x = 1000000000; x < 1010000000; x++) {
            if (x % 1000000 == 0)
                cout << x << '\r';
            auto gt = isPrimeNumberSlow(x);
            auto ans = PrimalityTestInt32::isPrimeNumber(x);
            if (ans != gt)
                cout << "Mismatch in [" << x << "] : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "*** Speed Test" << endl;
    {
        PROFILE_START(0);
        int cnt = 0;
        for (int x = 1; x > 0; x++) {
            if (x % 10000000 == 0)
                cout << x << '\r';
            cnt += PrimalityTestInt32::isPrimeNumber(x);
        }
        PROFILE_STOP(0);
        if (cnt == 0)
            cout << "ERROR" << endl;
    }
    cout << "OK!" << endl;
}
