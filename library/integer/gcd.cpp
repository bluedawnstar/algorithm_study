#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "factor.h"
#include "gcd.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGcd() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- gcd() / lcm() / extGcd() -------------------------" << endl;
    {
        assert(gcd(2 * 2 * 2 * 2 * 2, 3 * 3 * 3) == 1);
        assert(gcd(2 * 3 * 2 * 3 * 7, 3 * 3 * 3) == 9);
        int a = 134232, b = 93231;
        assert(gcd(a, b) * lcm(a, b) == a * b);

        int x, y;
        extGcd(a, b, x, y);
        assert(a * x + b * y == gcd(a, b));
        cout << "extGcd(" << a << ", " << b << ") = " << make_pair(x, y) << endl;
    }
    {
#ifdef _DEBUG
        int N = 100000;
#else
        int N = 10000000;
#endif
        vector<pair<unsigned int, unsigned int>> in(N);
        for (int i = 0; i < N; i++) {
            in[i].first = RandUInt32::get();
            in[i].second = RandUInt32::get();

            assert(gcd(in[i].first, in[i].second) == gcdFast(in[i].first, in[i].second));
        }

        PROFILE_START(0);
        {
            long long s = 0;
            for (auto& it : in) {
                s += gcd(it.first, it.second);
            }
            if (s == 0)
                cout << "Dummy" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            long long s = 0;
            for (auto& it : in) {
                s += gcd2(it.first, it.second);
            }
            if (s == 0)
                cout << "Dummy" << endl;
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        {
            long long s = 0;
            for (auto& it : in) {
                s += gcdFast(it.first, it.second);
            }
            if (s == 0)
                cout << "Dummy" << endl;
        }
        PROFILE_STOP(2);
    }

    cout << "OK!" << endl;
}
