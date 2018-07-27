#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

// https://www.hackerrank.com/contests/w36/challenges/expert-computation
// https://www.hackerrank.com/external_redirect?to=http://codeforces.com/blog/entry/51684

#include "persistentUpperConvexHull.h"
#include "persistentLowerConvexHull.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define MOD     1000000007

static int solveWoC36_P6(const vector<int>& X, const vector<int>& Y, const vector<int>& Z) {
    int N = int(X.size());

    PersistentLowerConvexHull<int> ch(N + 1);

    //ch.add(X[0], Y[0]);
    ch.addLinear(X[0], Y[0]);

    int G = 0;
    for (int i = 1; i < N; i++) {
        int H = X[i] ^ G;
        int C = Y[i] ^ G;
        int L = Z[i] ^ G;

        //ch.add(H, C);
        ch.addLinear(H, C);

        auto t = ch.queryWithSlope(i - L, H, C);
        auto p = ch.get(t[0]);
        long long F = (long long)p.first * C - (long long)H * p.second;

        if (F < 0)
            F = F % MOD + MOD;

        G = int(((long long)G + F) % MOD);
    }
    return G;
}

static int solveWoC36_P6_2(const vector<int>& X, const vector<int>& Y, const vector<int>& Z) {
    int N = int(X.size());

    PersistentUpperConvexHull<int> ch(N + 1);

    //ch.add(X[0], -Y[0]);
    ch.addLinear(X[0], -Y[0]);

    int G = 0;
    for (int i = 1; i < N; i++) {
        int H = X[i] ^ G;
        int C = Y[i] ^ G;
        int L = Z[i] ^ G;

        //ch.add(H, -C);
        ch.addLinear(H, -C);

        auto t = ch.queryWithSlope(i - L, H, -C);
        auto p = ch.get(t[0]);
        long long F = (long long)p.first * C + (long long)H * p.second;

        if (F < 0)
            F = F % MOD + MOD;

        G = int(((long long)G + F) % MOD);
    }
    return G;
}

void testPersistentConvexHull() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Persistent Convex Hull ------------------------" << endl;
    assert(solveWoC36_P6(vector<int>{3, 4, 14, 5, 6}, vector<int>{4, 9, 12, 8, 9}, vector<int>{0, 0, 9, 12, 13}) == 17);
    assert(solveWoC36_P6_2(vector<int>{3, 4, 14, 5, 6}, vector<int>{4, 9, 12, 8, 9}, vector<int>{0, 0, 9, 12, 13}) == 17);

    cout << "OK!" << endl;
}
