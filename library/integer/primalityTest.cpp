#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primalityTest.h"
#include "primeNumberBasic.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

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

void testPrimalityTest() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Primality Test -------------------" << endl;
    {
        cout << "-- GT | Rho | Fermat | Miller ---" << endl;
        long long L = 10000000000000ll;
        long long R = 10000000001000ll;
        for (long long i = L; i <= R; i++) {
            auto gt = isPrimeNumberSlow(i);
            auto ans = PrimalityTest<long long>::isPrimeNumber(i);
            if (ans != gt)
                cout << "[" << i << "] " << ans << ", " << gt << endl;
        }
    }
#if 0
    {
        cout << "*** prime numbers for Garner Algorithm" << endl;
        int x = 1000000000;
        for (int i = 0; i < 200; x++) {
            auto gt = isPrimeNumberSlow(x);
            auto ans = PrimalityTest<int>::isPrimeNumber(x);
            if (ans != gt)
                cout << "[" << i << "] " << ans << ", " << gt << endl;
            if (gt) {
                cout << x << "," << endl;
                i++;
            }
        }
        return;
    }
#endif
    {
        cout << "-- GT | Rho | Fermat | Miller ---" << endl;
        //int T = 10000;
        int T = 100;
        while (T-- > 0) {
            //long long x = RandInt64::get();
            long long x = RandInt64::get() % (1ll << 62);
            auto gt = isPrimeNumberSlow(x);
            auto ans = PrimalityTest<long long>::isPrimeNumber(x);
            if (ans != gt)
                cout << "Mismatch in [" << T << ", " << x << "] " << ans << ", " << gt << endl;
        }
    }

    cout << "OK!" << endl;
}
