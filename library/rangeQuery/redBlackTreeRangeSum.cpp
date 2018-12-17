#include <algorithm>

using namespace std;

#include "redBlackTreeRangeSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <memory.h>
#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static int sumSlow(vector<int>& v, int L, int R) {
    int res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

void testRedBlackTreeRangeSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Red-Black Tree for Range Sum -------------------" << endl;
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 7, 8, 6, 11 };
        int N = int(in.size());

        RBTreeRangeSum<> tree;
        for (int i = 0; i < int(in.size()); i++)
            tree.add(i, in[i]);

        int ans = tree.sum(5);
        assert(ans == 12);

        tree.add(3, 6);
        in[3] += 6;
        ans = tree.sum(5);
        assert(ans == 18);

        ans = tree.sum(1, 5);
        assert(ans == 16);

        tree.add(0, 1);
        in[0] += 1;
        assert(tree.sum(15) == sumSlow(in, 0, 15));

        tree.add(15, 1);
        in[15] += 1;
        assert(tree.sum(0, 15) == sumSlow(in, 0, 15));
    }
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
        int N = int(in.size());

        RBTreeRangeSum<> tree;
        for (int i = 0; i < int(in.size()); i++)
            tree.add(i, in[i]);

        for (int i = 0; i < N; i++) {
            int ans = tree.get(i).first;
            int gt = sumSlow(in, i, i);
            if (ans != gt) {
                ans = tree.get(i).first;
                cout << "[" << i << "] ans = " << ans << ", gt = " << gt << endl;
            }
            assert(ans == gt);
        }

        for (int i = 0; i < 10000; i++) {
            if (i % 1) {
                int idx = RandInt32::get() % N;
                int val = RandInt32::get() % 100;
                in[idx] += val;
                tree.add(idx, val);

                int R = idx;
                int ans = tree.get(R).first;
                int gt = sumSlow(in, R, R);
                if (ans != gt) {
                    ans = tree.get(R).first;
                    cout << "[" << R << "] ans = " << ans << ", gt = " << gt << endl;
                }
                assert(ans == gt);
            } else {
                int R = RandInt32::get() % N;
                int ans = tree.get(R).first;
                int gt = sumSlow(in, R, R);
                if (ans != gt) {
                    ans = tree.get(R).first;
                    cout << "[" << R << "] ans = " << ans << ", gt = " << gt << endl;
                }
                assert(ans == gt);
            }
        }
    }

    cout << "OK!" << endl;
}
