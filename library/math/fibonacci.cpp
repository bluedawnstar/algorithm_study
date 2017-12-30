
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

long long fibonacciSlow(int n) {
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

void testFibonacci() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Fibonacci ------------------------------" << endl;

    for (int i = 0; i <= 1000; i++) {
        assert(fibonacci<long long>(i).a01 == fibonacciSlow(i));
    }

    PROFILE_START(0);
    for (int i = 0; i <= 1000; i++) {
        auto t = fibonacci<long long>(i);
        if (t.a11 == LLONG_MAX)
            cerr << "?" << endl;
    }
    PROFILE_STOP(0);

    PROFILE_START(1);
    for (int i = 0; i <= 1000; i++) {
        auto t = fibonacciSlow(i);
        if (t == LLONG_MAX)
            cerr << "?" << endl;
    }
    PROFILE_STOP(1);

    cout << "OK!" << endl;
}
