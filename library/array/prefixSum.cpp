#include <vector>
#include <algorithm>

using namespace std;

#include "prefixSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int sum(const vector<int>& v, int left, int right) {
    int res = 0;
    for (int i = left; i <= right; i++)
        res += v[i];
    return res;
}

void testPrefixSum() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Prefix Sum ----------" << endl;

    int N = 1000000;
    int T = 1000;
#ifdef _DEBUG
    N = 10000;
    T = 1000;
#endif

    vector<int> v(N);
    for (int i = 0; i < N; i++) {
        v[i] = RandInt32::get() % N;
    }
    PrefixSum<int> S(v);

    for (int i = 0; i < T; i++) {
        int L = RandInt32::get() % N;
        int R = RandInt32::get() % N;
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
