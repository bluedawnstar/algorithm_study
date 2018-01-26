#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeFactorLongLong.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include "primalityTest.h"
#include "primeNumberBasic.h"

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

static int rand16() {
    return rand() & 0x7fff;
}

static int rand32() {
    return (rand16() * rand16() & 0xffff) * rand16();
}

static long long rand64() {
    return (long long)rand32() * rand32();
}

void testPrimeFactorLongLong() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    srand((unsigned int)time(nullptr));

    cout << "--- Prime Factorization of 64bit Integer -------------------" << endl;
    {
        int N = 1000000;
        PrimeFactorizerLongLong pfLL;
        pfLL.init(N);

        auto x = 34129034312412ll;
        auto ans = pfLL.factorize(x);
        cout << x << " = ";
        for (int i = 0; i < (int)ans.size(); i++) {
            cout << "(" << ans[i].first << ", " << ans[i].second << "), ";
        }
        cout << endl;
    }
    {
        int N = 1000000;
        PrimeFactorizerLongLong pfLL;
        pfLL.init(N);

        cout << "-- GT | Rho | Fermat | Miller ---" << endl;
        long long L = 10000000000000ll;
#ifdef _DEBUG
        long long R = 10000000001000ll;
#else
        long long R = 10000000010000ll;
#endif
        for (long long i = L; i <= R; i++) {
            auto gt = PrimalityTest<long long>::isPrimeNumber(i);

            auto t = pfLL.factorize(i);
            auto ans = (t.size() == 1 && t[0].second == 1);
            if (gt != ans)
                cout << "[" << i << "] " << gt << ", " << ans << endl;
            assert(gt == ans);
        }
    }
    {
        int N = 1000000;
        PrimeFactorizerLongLong pfLL;
        pfLL.init(N);

        cout << "-- GT | Rho | Fermat | Miller ---" << endl;

        int T = 1000;
        while (T-- > 0) {
            long long x = rand64() + 1;
            auto gt = PrimalityTest<long long>::isPrimeNumber(x);

            auto t = pfLL.factorize(x);
            auto ans = (t.size() == 1 && t[0].second == 1);
            if (gt != ans)
                cout << "[" << x << "] " << gt << ", " << ans << endl;
            assert(gt == ans);
        }
    }
    // speed test
    {
        cout << "-- Speed Test ---" << endl;

        int N = 1000000;
        PrimeFactorizerLongLong pfLL;
        pfLL.init(N);

        int T = 1000;

        PROFILE_START(0);
        while (T-- > 0) {
            long long x = rand64() + 1;
            auto t = pfLL.factorize(x);
            if (x > 1 && t.empty())
                cout << "ERROR!" << endl;
        }
        PROFILE_STOP(0);
    }

    cout << "OK!" << endl;
}
