#include <vector>
#include <algorithm>

using namespace std;

#include "divideAndSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int solveNaive(const vector<int>& A, int K) {
    int N = int(A.size());

    int lo = 0;
    while (lo < N) {
        long long s = 0;
        for (int i = lo, j = 1; i < N; i++, j++) {
            s += A[i] / j;
            if (s > K)
                break;
        }
        if (s <= K)
            break;
        lo++;
    }

    return lo;
}

void testDivideAndSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Divide And Sum -------------" << endl;
    {
        int T = 10;
        int N = 100000;
        int maxA = 100000;
#if _DEBUG
        N = 5000;
        maxA = 5000;
#endif
        while (T-- > 0) {
            vector<int> A(N);
            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % maxA + 1;
            int K = RandInt32::get() % 1000000000 + 1;

            int gt = solveNaive(A, K);

            DivideAndSumSolver solver;
            solver.build(A);
            int ans = solver.findFirstLessThanEqual(K).first;

            if (ans != gt)
                cout << "Mismatched: " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
}
