#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "mobius.h"
#include "primeNumberBasic.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define NN  1000000

void testMobius() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Mobius Function -----------------------" << endl;
    {
        for (int n = 10; n <= 1000000000; n *= 10) {
            cout << "square free [1, " << n << "] : " << countSquareFree(n) << endl;
        }
        cout << "square free [1, " << 2000000000 << "] : " << countSquareFree(2000000000) << endl;
    }
    {
        int N = 1000;
        vector<int> mu = mobiusSeive(N);
        vector<bool> sf = squareFree(N);
        for (int i = 1; i <= N; i++) {
            int a = mu[i];
            int b = mobius(i);
            if (a != b)
                cout << "Mismatched at " << i << ": " << a << ", " << b << endl;
            assert(a == b);
            assert(sf[i] == (b != 0));
        }

        int sfCnt = count(sf.begin(), sf.end(), true);
        assert(sfCnt == countSquareFree(N));
    }
    {
        int N = 100;
        vector<bool> sf = squareFree(N);

        PROFILE_START(0);
        vector<int> mu = mobiusSeive(65536);
        PROFILE_STOP(0);

        assert(nthSquareFree(9, mu) == 14);

        PROFILE_START(1);
        vector<int> sfNumber;
        for (int i = 1; i <= N; i++) {
            if (!sf[i])
                continue;

            int nth = (int)sfNumber.size();
            sfNumber.push_back(i);
            assert(nthSquareFree(nth, mu) == i);
        }
        PROFILE_STOP(1);
    }
    {
        int L = 20000;
        int R = 100000;
        vector<int> mu = mobiusSeive(R);
        vector<int> mu2 = mobiusSeive(L, R);
        assert(mu2 == vector<int>(mu.begin() + L, mu.end()));
    }
    cout << "OK!" << endl;
}
