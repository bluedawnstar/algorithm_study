#include <cmath>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "convexHullTrick.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

typedef long long ll;

// https://www.hackerrank.com/contests/w30/challenges/poles
ll solveWoC30(int KK, vector<pair<int,int>>& xw) {
    int NN = (int)xw.size();
    vector<ll> sumW(NN);
    vector<ll> sumXW(NN);
    vector<vector<ll>> dp(KK + 1, vector<ll>(NN));

    reverse(xw.begin(), xw.end());

    sumW[0] = xw[0].second;
    sumXW[0] = ll(xw[0].second) * xw[0].first;
    for (int i = 1; i < NN; i++) {
        sumW[i] = sumW[i - 1] + xw[i].second;
        sumXW[i] = sumXW[i - 1] + ll(xw[i].second) * xw[i].first;
    }

    for (int n = 1; n < NN; n++)
        dp[1][n] = sumXW[n] - sumW[n] * xw[n].first;

    for (int k = 2; k <= KK; k++) {
        DPConvexHullTrick<ll> cht;
        cht.insertReverse(sumW[k - 2], dp[k - 1][k - 2] - sumXW[k - 2]);
        for (int n = k - 1; n < NN; n++) {
            if (n < k)
                dp[k][n] = 0;
            else
                dp[k][n] = sumXW[n] - xw[n].first * sumW[n] + cht.query(xw[n].first);
            cht.insertReverse(sumW[n], dp[k - 1][n] - sumXW[n]);
        }
    }

    return dp[KK][NN - 1];
}


void testConvexHullTrick() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Convex Hull Trick ------------------------" << endl;

    assert(solveWoC30(12, vector<pair<int, int>>{
        { 4, 994 },
        { 27, 980 },
        { 88, 974 },
        { 89, 966 },
        { 117, 940 },
        { 171, 936 },
        { 191, 898 },
        { 209, 876 },
        { 247, 873 },
        { 290, 839 },
        { 356, 791 },
        { 372, 738 },
        { 405, 705 },
        { 456, 698 },
        { 457, 637 },
        { 485, 572 },
        { 488, 456 },
        { 555, 422 },
        { 608, 413 },
        { 612, 372 },
        { 669, 327 },
        { 713, 304 },
        { 718, 276 },
        { 728, 247 },
        { 758, 228 },
        { 779, 197 },
        { 826, 153 },
        { 856, 131 },
        { 861, 130 },
        { 871, 123 },
        { 893, 108 },
        { 937, 41 },
        { 979, 30 },
    }) == 224606ll);

    assert(solveWoC30(5, vector<pair<int, int>>{
        { 28, 445 },
        { 34, 468 },
        { 56, 304 },
        { 63, 752 },
        { 71, 432 },
        { 72, 38 },
        { 78, 697 },
        { 97, 872 },
    }) == 6606ll);

    cout << "OK!" << endl;
}
