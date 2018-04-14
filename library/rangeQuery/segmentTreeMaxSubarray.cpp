#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "segmentTreeMaxSubarray.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int getMaxSubarray(const vector<int>& v, int L, int R) {
    int ans = v[L];
    int sum = v[L];
    for (int i = L + 1; i <= R; i++) {
        sum = max(sum + v[i], v[i]);
        ans = max(ans, sum);
    }

    return ans;
}

void testSegmentTreeMaxSubarray() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Segment Tree for Maximum Subarray ----------------------------------------" << endl;
    {
        vector<int> in{ 1, 3, -1, 2, -2 };
        SegmentTreeMaxSubarray<int> tree(5);
        tree.build(in);

        auto ans = tree.query(0, 2);
        assert(ans == 4);

        ans = tree.query(0, 3);
        assert(ans == 5);

        ans = tree.query(1, 4);
        assert(ans == 4);
    }
    {
        int N = 10;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 10 - 5;

        SegmentTreeMaxSubarray<int> tree(N);
        tree.build(in);

        int T = 100;
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            auto ans = tree.query(L, R);
            auto gt = getMaxSubarray(in, L, R);
            if (ans != gt) {
                cout << "Mismatched(" << L << "," << R << ") : ans = " << ans << ", gt = " << gt << endl;
            }
            assert(ans == gt);
        }
    }
    {
        int N = 100;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 32768 - 16384;

        SegmentTreeMaxSubarray<int> tree(N);
        tree.build(in);

        int T = 100;
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            auto ans = tree.query(L, R);
            auto gt = getMaxSubarray(in, L, R);
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
