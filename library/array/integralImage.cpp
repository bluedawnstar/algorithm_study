#include <vector>
#include <algorithm>

using namespace std;

#include "integralImage.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int sum(const vector<vector<int>>& v, int left, int top, int right, int bottom) {
    int res = 0;
    for (int i = top; i <= bottom; i++) {
        for (int j = left; j <= right; j++)
            res += v[i][j];
    }
    return res;
}

void testIntegralImage() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Integral Image ----------" << endl;

    int N = 100;
    int T = 1000;

    vector<vector<int>> v(N, vector<int>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            v[i][j] = RandInt32::get() % N;
    }

    IntegralImage<int> S(v);

    for (int i = 0; i < T; i++) {
        int L = RandInt32::get() % N;
        int R = RandInt32::get() % N;
        int T = RandInt32::get() % N;
        int B = RandInt32::get() % N;
        if (L > R)
            swap(L, R);
        if (T > B)
            swap(T, B);

        int gt = sum(v, L, T, R, B);
        int ans = S.query(L, T, R, B);
        if (ans != gt) {
            cerr << "gt = " << gt << ", x = " << ans << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
