#include <cmath>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "eulerTheorem.h"
#include "eulerTheoremEx.h"

#include "primeFactor.h"
#include "primeNumberBasic.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define NN  1000000

static long long phiSum(int n) {
    long long res = 0;
    for (int i = 1; i <= n; i++)
        res += phi(i);
    return res;
}

void testEulerTheorem() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Euler Theorem -----------------------" << endl;
    {
        //int N = 1000000009;
        int N = 1009;
        for (int i = 1; i <= N; i++) {
            auto ans1 = phi(i);
            auto ans2 = phiFast(i);
            if (ans1 != ans2)
                cerr << "Mismatched!" << endl;
            assert(ans1 == ans2);
        }
    }
    {
        int n = 1000000;
        auto ans = phiAll(n);
        for (int i = 0; i <= n; i++) {
            int gt = phi(i);
            if (ans[i] != gt)
                cout << "Mismatched at " << i << " : " << ans[i] << ", " << gt << endl;
            assert(ans[i] == gt);
        }
    }
    {
        int N = 10000;
#ifdef _DEBUG
        N = 1000;
#endif

        PhiSum solver(N);

        for (int i = 1; i <= N; i++) {
            auto ans1 = phiSum(i);
            auto ans2 = solver.sum(i);
            if (ans1 != ans2)
                cout << "Mismatched at " << i << " : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
