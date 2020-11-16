#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "longestIncreasingStep.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// longest increasing step
static int LIS(const vector<int>& v, int L, int R) {
    int res = 0;

    vector<pair<int, int>> stk;
    stk.emplace_back(numeric_limits<int>::max(), 0);
    for (int i = L; i <= R; i++) {
        int step = 0;
        while (stk.back().first < v[i]) {
            step = max(step, stk.back().second);
            stk.pop_back();
        }
        stk.emplace_back(v[i], step + 1);
        res = max(res, step + 1);
    }

    return res;
}

static vector<int> solveSlow(const vector<int>& v, const vector<pair<int, int>>& qry) {
    vector<int> res;
    res.reserve(qry.size());

    for (int i = 0; i < int(qry.size()); i++)
        res.push_back(LIS(v, qry[i].first, qry[i].second));

    return res;
}

void testLongestIncreasingStep() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Increasing Step for range-based queries --------------------" << endl;
#ifndef _DEBUG
    const int T = 100;
    const int N = 1'000;
#else
    const int T = 10;
    const int N = 100;
#endif
    const int MAXX = 100'000;

    for (int tt = 0; tt < T; tt++) {
        cout << tt << " ...\r";

        vector<int> A(N);
        for (int i = 0; i < N; i++)
            A[i] = RandInt32::get() % MAXX + 1;

        vector<pair<int, int>> qry;
        qry.reserve(N);
        for (int i = 0; i < N; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            qry.emplace_back(L, R);
        }

        LongestIncreasingStep solver;
        solver.init(A);

        for (int i = 0; i < int(qry.size()); i++) {
            int gt = LIS(A, qry[i].first, qry[i].second);
            int ans = solver.query(qry[i].first, qry[i].second);
            if (ans != gt) {
                cout << "Mismatched : " << ans << ", " << gt << endl;
                cout << "         A = " << A << endl;
                cout << "       ans = " << ans << endl;
                cout << "        gt = " << gt << endl;
                break;
            }
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
}
