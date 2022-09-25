#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "primeNumberEratosthenes.h"
#include "primeNumberLinearSieve.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#ifdef _DEBUG
#define NN  100000
#else
#define NN  10000000
#endif

void testPrimeNumberLinearSieve() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Linear Sieve --------------------------" << endl;
    {
        auto p1 = eratosthenes2(65536);
        auto p2 = linearSieve(65536);
        if (p1 != p2)
            cout << "Mismatched at " << __LINE__ << endl;
        assert(p1 == p2);
    }
    {
        auto p1 = linearSieve(100);
        auto p2 = linearSieve(100);
        if (p1 != p2)
            cout << "Mismatched at " << __LINE__ << endl;
        assert(p1 == p2);
    }
    {
        auto p1 = linearSieve(NN);
        auto p2 = linearSieve(NN);
        if (p1 != p2)
            cout << "Mismatched at " << __LINE__ << endl;
        assert(p1 == p2);
    }
    cout << "OK!" << endl;

    cout << "--- performance test ---" << endl;
    {
        PROFILE_START(eratosthenes);
        auto p1 = eratosthenes2(NN);
        PROFILE_STOP(eratosthenes);

        PROFILE_START(linear);
        auto p2 = linearSieve(NN);
        PROFILE_STOP(linear);

        if (p1 != p2)
            cout << "Mismatched at " << __LINE__ << endl;
        assert(p1 == p2);
    }
}
