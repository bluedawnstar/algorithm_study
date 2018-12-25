#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primalityTestInt64.h"

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
#include "primalityTestFast.h"

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

void testPrimalityTestInt64() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast Primality Test for Int64 -------------------" << endl;
    {
        long long L = 10000000000000ll;
        long long R = 10000000001000ll;
        for (long long i = L; i <= R; i++) {
            auto gt = isPrimeNumber(i);
            auto ans = PrimalityTestInt64::isPrimeNumber(i);
            if (ans != gt)
                cout << "Mismatch in [" << i << "] : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        long long x = 100000000000000000ll;
        for (int i = 0; i < 200; i++, x++) {
            auto gt = isPrimeNumber(x);
            auto ans = PrimalityTestInt64::isPrimeNumber(x);
            if (ans != gt)
                cout << "Mismatch in [" << i << "] : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        //int T = 10000;
        int T = 100;
        while (T-- > 0) {
            //long long x = RandInt64::get();
            long long x = RandInt64::get() % (1ll << 62);
            auto gt = isPrimeNumberSlow(x);
            auto ans = PrimalityTestInt64::isPrimeNumber(x);
            if (ans != gt)
                cout << "Mismatch in [" << T << ", " << x << "] : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "*** Speed Test" << endl;
    {
        int T = 10000;

        vector<long long> in(T);
        for (int i = 0; i < T; i++)
            in[i] = RandInt64::get() % (1ll << 62);

        int cnt1 = 0;
        PROFILE_START(0);
        for (int i = 0; i < T; i++)
            cnt1 += PrimalityTest<long long>::isPrimeNumber(in[i]);
        PROFILE_STOP(0);

        int cnt2 = 0;
        PROFILE_START(1);
        for (int i = 0; i < T; i++)
            cnt2 += FastPrimalityTest<long long>::isPrimeNumber(in[i]);
        PROFILE_STOP(1);

        int cnt3 = 0;
        PROFILE_START(2);
        for (int i = 0; i < T; i++)
            cnt3 += PrimalityTestInt64::isPrimeNumber(in[i]);
        PROFILE_STOP(2);

        if (cnt1 != cnt2 || cnt1 != cnt3)
            cout << "Mismatch : " << cnt1 << ", " << cnt2 << ", " << cnt3 << endl;
        assert(cnt1 == cnt2 && cnt1 == cnt3);
    }
    cout << "OK!" << endl;
}
