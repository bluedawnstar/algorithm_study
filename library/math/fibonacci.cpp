
#include <vector>
#include <algorithm>

using namespace std;

#include "fibonacci.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define MOD     1000000007

static long long fibonacciSlow(int n) {
    if (n == 0)
        return 0ll;
    else if (n == 1)
        return 1ll;
    else if (n == 2)
        return 1ll;

    long long a = 1ll, b = 1ll;
    for (int i = 3; i <= n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }

    return b;
}

static long long fibonacciSlowMod(int n) {
    if (n == 0)
        return 0;
    else if (n == 1)
        return 1;
    else if (n == 2)
        return 1;

    long long a = 1, b = 1;
    for (int i = 3; i <= n; i++) {
        long long c = a + b;
        if (c >= MOD)
            c -= MOD;
        a = b;
        b = c;
    }

    return b;
}

void testFibonacci() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fibonacci ------------------------------" << endl;
    {
        for (int i = 0; i <= 1000; i++) {
            auto gt = fibonacciSlow(i);
            auto ans = fibonacci<long long>(i);
            assert(ans == gt);
        }

        for (int i = 0; i <= 1000; i++) {
            auto gt = fibonacciSlowMod(i);
            auto ans = fibonacciMod<int, MOD>(i);
            auto ans2 = fibonacciModFast<int, MOD>(i);
            if (ans != gt || ans2 != gt) {
                cout << "Mismatched F(" << i << ") = " << ans << ", " << ans2 << ", " << gt << endl;
            }
            assert(ans == gt && ans2 == gt);
        }
    }
    {
#ifdef _DEBUG
        const int T = 10000;
#else
        const int T = 1000000;
#endif

        PROFILE_START(0);
        for (int i = 0; i <= T; i++) {
            auto t = fibonacciMod<int, MOD>(i);
            if (t < 0)
                cerr << "?" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i <= T; i++) {
            auto t = fibonacciModFast<int, MOD>(i);
            if (t < 0)
                cerr << "?" << endl;
        }
        PROFILE_STOP(1);

#ifdef _DEBUG
        PROFILE_START(2);
        for (int i = 0; i <= T; i++) {
            auto t = fibonacciSlowMod(i);
            if (t < 0)
                cerr << "?" << endl;
        }
        PROFILE_STOP(2);
#endif
    }
    cout << "OK!" << endl;
}
