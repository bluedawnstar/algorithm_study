#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "longestIncreasingSubsequence.h"
#include "longestIncreasingSubsequenceOnline.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int LIS(const vector<int>& v, int L, int R) {
    vector<int> res;
    res.reserve(R - L + 1);
    res.push_back(v[L]);
    for (int i = L + 1; i <= R; i++) {
        if (res.back() < v[i])
            res.push_back(v[i]);
        else
            *lower_bound(res.begin(), res.end(), v[i]) = v[i];
    }
    return int(res.size());
}

static vector<int> solveSlow(const vector<int>& v, const vector<pair<int, int>>& qry) {
    vector<int> res;
    res.reserve(qry.size());

    for (int i = 0; i < int(qry.size()); i++)
        res.push_back(LIS(v, qry[i].first, qry[i].second));

    return res;
}

void testLongestIncreasingSubsequence() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Increasing Subsequence for range-based queries --------------------" << endl;
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

        auto gt = solveSlow(A, qry);

        LongestIncreasingSubsequence solver;
        solver.init(A, MAXX);
        auto ans = solver.solve(qry);

        if (ans != gt) {
            cout << "Mismatched : " << int(ans.size()) << ", " << int(gt.size()) << endl;
            cout << "         A = " << A << endl;
            cout << "       ans = " << ans << endl;
            cout << "        gt = " << gt << endl;
        }
        assert(ans == gt);
    }
    cout << "OK!" << endl;

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

        LongestIncreasingSubsequenceOnline solver;
        solver.init(A, MAXX);

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
