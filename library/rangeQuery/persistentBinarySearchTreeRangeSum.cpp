#include <vector>
#include <algorithm>

using namespace std;

#include "persistentBinarySearchTreeRangeSum.h"

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

void testPersistentBinarySearchTreeRangeSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Persistent Binary Search Tree for Range Sum -------------------" << endl;
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 7, 8, 6, 11 };
        int N = int(in.size());

        PersistentBSTRangeSum<> tree;

        PersistentBSTRangeSum<>::Node* root = nullptr;
        for (int i = 0; i < int(in.size()); i++)
            root = tree.add(root, i, in[i]);

        int ans = tree.sum(root, 5);
        assert(ans == 12);

        root = tree.add(root, 3, 6);
        in[3] += 6;
        ans = tree.sum(root, 5);
        assert(ans == 18);

        ans = tree.sum(root, 1, 5);
        assert(ans == 16);

        root = tree.add(root, 0, 1);
        in[0] += 1;
        assert(tree.sum(root, 15) == sumSlow(in, 0, 15));

        root = tree.add(root, 15, 1);
        in[15] += 1;
        assert(tree.sum(root, 0, 15) == sumSlow(in, 0, 15));
    }
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
        int N = int(in.size());

        PersistentBSTRangeSum<> tree;

        PersistentBSTRangeSum<>::Node* root = nullptr;
        for (int i = 0; i < int(in.size()); i++)
            root = tree.add(root, i, in[i]);

        for (int i = 0; i < N; i++) {
            int ans = tree.get(root, i).first;
            int gt = sumSlow(in, i, i);
            if (ans != gt) {
                ans = tree.get(root, i).first;
                cout << "[" << i << "] ans = " << ans << ", gt = " << gt << endl;
            }
            assert(ans == gt);
        }

        for (int i = 0; i < 10000; i++) {
            if (i % 1) {
                int idx = RandInt32::get() % N;
                int val = RandInt32::get() % 100;
                in[idx] += val;
                root = tree.add(root, idx, val);

                int R = idx;
                int ans = tree.get(root, R).first;
                int gt = sumSlow(in, R, R);
                if (ans != gt) {
                    ans = tree.get(root, R).first;
                    cout << "[" << R << "] ans = " << ans << ", gt = " << gt << endl;
                }
                assert(ans == gt);
            } else {
                int R = RandInt32::get() % N;
                int ans = tree.get(root, R).first;
                int gt = sumSlow(in, R, R);
                if (ans != gt) {
                    ans = tree.get(root, R).first;
                    cout << "[" << R << "] ans = " << ans << ", gt = " << gt << endl;
                }
                assert(ans == gt);
            }
        }
    }

    cout << "OK!" << endl;
}
