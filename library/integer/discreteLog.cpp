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
#include "../common/rand.h"
#include "../common/profile.h"

const int MOD = 1000000007;

void testDiscreteLog() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Discrete Log -----------------------" << endl;
    {
        int T = 2;
        int M = 1000000009;
        for (int i = 0; i < T; i++) {
            long long a = RandInt64::get();
            int x = RandInt64::get() % M;
            long long b = modPow(a, (long long)x, (long long)M);
            auto ans = DiscreteLog::solve32bit(a, b, M);
            if (ans < 0) {
                cout << "[" << i << "] Can't find answer : " << a << "^x = " << b << " (mod " << M << ")" << endl;
            } else {
                if (modPow(a, (long long)ans, (long long)M) != b) {
                    cout << "[" << i << "] Wrong answer : " << a << "^x = " << b << " (mod " << M << ")" << endl;
                }
                assert(modPow(a, (long long)ans, (long long)M) == b);
            }
        }
    }
    {
        int T = 2;
        long long M = 100000015277ll;
        for (int i = 0; i < T; i++) {
            long long a = RandInt64::get() % M;
            long long x = RandInt64::get() % M;
            long long b = modPow(a, x, M);
            auto ans = DiscreteLog::solve64bit(a, b, M);
            if (ans < 0) {
                cout << "[" << i << "] Can't find answer : " << a << "^x = " << b << " (mod " << M << ")" << endl;
            } else {
                if (modPow(a, ans, M) != b) {
                    cout << "[" << i << "] Wrong answer : " << a << "^x = " << b << " (mod " << M << ")" << endl;
                }
                assert(modPow(a, ans, M) == b);
            }
        }
    }
    cout << "OK!" << endl;
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
