#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "knuthOptimization.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.acmicpc.net/problem/11066
static long long solve(const vector<long long>& A) {
    int N = (int)A.size();

    vector<long long> S(N + 1);
    for (int i = 1; i <= N; i++)
        S[i] = S[i - 1] + A[i - 1];

    auto solver = makeKnuthOptimizer(N, 0x3f3f3f3f3f3f3f3fll, [&S](int i, int j) {
        return S[j] - S[i];
    });
    return solver.solve();
}

static long long solveNaive(const vector<long long>& A) {
    int N = (int)A.size();

    vector<long long> S(N + 1);
    for (int i = 1; i <= N; i++)
        S[i] = S[i - 1] + A[i - 1];

    vector<vector<long long>> dp(N + 1, vector<long long>(N + 1, 0x3f3f3f3f3f3f3f3fll));
    for (int d = 0; d <= N; d++) {
        for (int i = 0; i + d <= N; i++) {
            int j = i + d;
            if (d < 2) {
                dp[i][j] = 0;
                continue;
            }

            dp[i][j] = 0x3f3f3f3f3f3f3f3fll;
            for (int k = i; k < j; k++) {
                long long val = dp[i][k] + dp[k][j];
                if (val < dp[i][j])
                    dp[i][j] = val;
            }
            dp[i][j] += S[j] - S[i];
        }
    }
    return dp[0][N];
}

void testKnuthOptimization() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Knuth Optimization ------------------------" << endl;

    int N = 100;
    vector<long long> A(N);
    for (int i = 0; i < N; i++)
        A[i] = RandInt32::get() % 1000;

    auto gt = solveNaive(A);
    auto ans = solve(A);
    if (gt != ans)
        cout << "Mismatch! : gt = " << gt << ", ans = " << ans << endl;
    assert(gt == ans);

    cout << "OK!" << endl;
}
