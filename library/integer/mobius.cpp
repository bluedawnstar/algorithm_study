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
        int N = 1000;
        MinFactors minFactors(N);
        vector<int> mu = mobiusSeive(N, minFactors);
        vector<bool> sf = squareFree(N);
        for (int i = 1; i <= N; i++) {
            int a = mu[i];
            int b = mobius(i);
            if (a != b)
                cout << "Mismatched at " << i << ": " << a << ", " << b << endl;
            assert(a == b);
            assert(sf[i] == (b != 0));
        }
    }
    cout << "OK!" << endl;
}
