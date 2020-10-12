#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "optimizationDivideConquer.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testDivideAndConquerOptimization() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Divide & Conquer Optimization ------------------------" << endl;
    // Optimal Square Distance of 1D values
    // https://www.hackerrank.com/contests/101hack53/challenges/optimal-bus-stops/problem
    {
        vector<int> A{ 1, 2, 4 };
        int N = int(A.size());

        vector<long long> S(N + 1), SS(N + 1);
        for (int i = 1; i <= N; i++) {
            S[i] = S[i - 1] + A[i - 1];
            SS[i] = SS[i - 1] + 1ll * A[i - 1] * A[i - 1];
        }

        auto optimizer = makeDivideAndConquerOptimizer(N, 1.0e15, [&S, &SS](int i, int L, int R) {
            return SS[R] - SS[L - 1] - double(S[R] - S[L - 1]) * (S[R] - S[L - 1]) / (R - L + 1);
        });

        assert(optimizer.solve(2) == 0.5);
    }

    cout << "OK!" << endl;
}
