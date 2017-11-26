#include <vector>
#include <algorithm>

using namespace std;

#include "arraySum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static int sum(const vector<int>& v, int left, int right) {
    int res = 0;
    for (int i = left; i <= right; i++)
        res += v[i];
    return res;
}

void testPositiveSum() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Positive Sum ----------" << endl;

    int N = 1000000;
    int T = 1000;

    vector<int> v(N);
    for (int i = 0; i < N; i++) {
        v[i] = rand() % N;
    }
    ArraySum<int> S(v);

    for (int i = 0; i < T; i++) {
        int L = rand() * rand() % N;
        int R = rand() * rand() % N;
        if (L > R)
            swap(L, R);

        int gt = sum(v, L, R);
        int ans = S.query(L, R);
        if (ans != gt) {
            cerr << "gt = " << gt << ", x = " << ans << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
