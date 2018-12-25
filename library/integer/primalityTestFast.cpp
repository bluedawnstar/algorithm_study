#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primalityTestFast.h"

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

static bool isPrimeNumber(long long x, const vector<long long>& primes) {
    int idx = 0;
    long long p = primes[0];
    while (p * p <= x) {
        if (x % p == 0)
            return false;
        p = primes[++idx];
    }
    return true;
}

void testPrimalityTestFast() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast Primality Test -------------------" << endl;
    {
        long long L = 10000000000000ll;
        long long R = 10000000001000ll;
        for (long long i = L; i <= R; i++) {
            auto gt = isPrimeNumber(i);
            auto ans = FastPrimalityTest<long long>::isPrimeNumber(i);
            if (ans != gt)
                cout << "Mismatch in [" << i << "] : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int x = 1000000000;
        for (int i = 0; i < 200; i++, x++) {
            auto gt = isPrimeNumber(x);
            auto ans = FastPrimalityTest<int>::isPrimeNumber(x);
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
            auto ans = FastPrimalityTest<long long>::isPrimeNumber(x);
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

        if (cnt1 != cnt2)
            cout << "Mismatch : " << cnt1 << ", " << cnt2 << endl;
        assert(cnt1 == cnt2);
    }
    cout << "OK!" << endl;
}
