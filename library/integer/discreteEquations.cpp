#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "discreteEquations.h"
#include "discreteLog.h"
#include "discreteRoot.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <random>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "intMod.h"

void testDiscreteEquations() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Equations -------------------------" << endl;
    {
        auto ans = LinearCongruence<int>::solve(14, 30, 100);
        assert(ans.size() == 2 && ans[0] == 45 && ans[1] == 95);
    }
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
    {
        int T = 1000;
#ifdef _DEBUG
        T = 100;
#endif
        int M = 1000000009;
        for (int i = 0; i < 1000; i++) {
            int x = RandInt32::get() % 65536 + 1;
            int xx = int(1ll * x * x % M);
            auto roots = DiscreteRoot<int>::rootAll(2, xx, M);
            auto ok = (find(roots.begin(), roots.end(), x) != roots.end());
            if (!ok)
                cout << "Can't find answer! : " << x << "^2 = " << xx << endl;
            assert(ok);
        }
    }

    cout << "OK!" << endl;
}
