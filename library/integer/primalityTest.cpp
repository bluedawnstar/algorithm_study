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
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Primality Test -------------------" << endl;
    {
        cout << "-- GT | Rho | Fermat | Miller ---" << endl;
        long long L = 10000000000000ll;
        long long R = 10000000001000ll;
        for (long long i = L; i <= R; i++) {
            auto gt = isPrimeNumber(i);
            auto ans = PrimalityTest<long long>::isPrimeNumber(i);
            if (gt != ans)
                cout << "[" << i << "] " << gt << ", " << ans << endl;
        }
    }
#if 0
    {
        cout << "*** prime numbers for Garner Algorithm" << endl;
        int x = 1000000000;
        for (int i = 0; i < 200; x++) {
            auto gt = isPrimeNumber(x);
            auto ans = PrimalityTest<int>::isPrimeNumber(x);
            if (gt != ans)
                cout << "[" << i << "] " << gt << ", " << ans << endl;
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
        int T = 10000;
        while (T-- > 0) {
            long long x = RandInt64::get();
            auto gt = isPrimeNumber(x);
            auto ans = PrimalityTest<long long>::isPrimeNumber(x);
            if (gt != ans)
                cout << "[" << x << "] " << gt << ", " << ans << endl;
        }
    }

    cout << "OK!" << endl;
}
