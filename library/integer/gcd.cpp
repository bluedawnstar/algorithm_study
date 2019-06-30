#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "factor.h"
#include "gcd.h"
#include "gcdAlgo.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define MOD     1000000007

// https://www.hackerearth.com/challenges/competitive/june-circuits-19/algorithm/calculate-gcd-june-circuit-e5e41856/
struct GcdCombSolver {
    Gcd1CombCounter gcd1;

    void build(int maxN) {
        gcd1.build(maxN, 4, MOD);
    }

    // SUM  SUM   SUM   SUM  gcd(i,j,k,l)^4
    // i=1 j=i+1 k=j+1 l=k+1
    // O(N)
    int solve(int N) const {
        long long res = 0;
        for (int i = 1; i <= N - 3; i++) {
            long long xx = 1ll * i * i % MOD;
            long long xxxx = 1ll * xx * xx % MOD;
            res = (res + (xxxx - 1) * gcd1.get(N / i) % MOD) % MOD; // subtract the number of gcd(a,b,c,d) != 1
        }
        res = (res + gcd1.comb(N, 4)) % MOD;    // all gcd(a,b,c,d) = 1

        return int(res);
    }
};


void testGcd() {
    //return; //TODO: if you want to test, make this line a comment.

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
    {
        GcdCombSolver solver;
        solver.build(20);

        vector<int> gt{
            0,
            0,
            0,
            1,
            5,
            15,
            35,
            85,
            141,
            285,
            405,
            800,
            1020,
            1606,
            2290,
            3510,
            4070,
            6390,
            7206,
            11019
        };

        for (int i = 1; i <= 20; i++) {
            int ans = solver.solve(i);
            if (ans != gt[i - 1])
                cout << "Mismatched at " << i << ": " << ans << ", " << gt[i] << endl;
            assert(solver.solve(i) == gt[i - 1]);
        }
    }

    cout << "OK!" << endl;
}
