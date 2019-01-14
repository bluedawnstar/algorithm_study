#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "dynamicSegmentTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <memory.h>
#include <queue>
#include <stack>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "segmentTree.h"
#include "segmentTreeLazy.h"
#include "segmentTreeRMQ.h"

static int sumSlow(vector<int>& v, int L, int R) {
    int res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

static void updateSlow(vector<int>& v, int L, int R, int value) {
    while (L <= R)
        v[L++] = value;
}

void testDynamicSegmentTree() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Dynamic Segment Tree -------------------------------------" << endl;
    {
        int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        DynamicSegmentTreeLazy<int> tree(0, N - 1, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        for (int i = 0; i < N; i++)
            tree.update(i, i, in[i]);

        for (int i = 0; i < N; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int gt = sumSlow(in, L, R);
            int ans = tree.query(L, R);
            if (ans != gt)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 10000;
        int T = 1000;
        vector<int> in(N);

        DynamicSegmentTreeLazy<int> tree(0, N - 1, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int gt = sumSlow(in, L, R);
            int ans = tree.query(L, R);
            if (ans != gt)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);

            int value = RandInt32::get() % 1000 + 1;
            updateSlow(in, L, R, value);
            tree.update(L, R, value);
        }
    }
    cout << "OK!" << endl;
}
