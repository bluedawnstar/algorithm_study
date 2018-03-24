using namespace std;

#include "fenwickTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>
#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static int sumSlow(vector<int>& v, int L, int R) {
    int res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

static int lowerBoundSlow(vector<int>& v, int k) {
    int res = 0;
    for (int i = 0; i < (int)v.size(); i++) {
        res += v[i];
        if (res >= k)
            return i;
    }
    return (int)v.size();
}

void testFenwickTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- FenwickTree (Binary Indexed Tree) -------------------" << endl;
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
        int N = (int)in.size();

        FenwickTree<int> fenwick(in);

        int ans = fenwick.sum(5);
        cout << "fenwick.sum(5) = " << ans << endl;
        assert(ans == 12);

        fenwick.add(3, 6);
        cout << "after fenwick.add(3, 6)" << endl;

        ans = fenwick.sum(5);
        cout << "fenwick.sum(5) = " << ans << endl;
        assert(ans == 18);

        ans = fenwick.sumRange(1, 5);
        cout << "fenwick.rangeSum(1, 5) = " << ans << endl;
        assert(ans == 16);
    }
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
        int N = (int)in.size();

        FenwickTree<int> fenwick(in);

        for (int i = 0; i < N; i++) {
            int ans = fenwick.get(i);
            int gt = sumSlow(in, i, i);
            if (ans != gt) {
                ans = fenwick.get(i);
                cerr << "[" << i << "] ans = " << ans << ", gt = " << gt << endl;
            }
            assert(ans == gt);
        }

        for (int i = 0; i < 10000; i++) {
            if (i % 1) {
                int idx = RandInt32::get() % N;
                int val = RandInt32::get() % 100;
                in[idx] = val;
                fenwick.set(idx, val);

                int R = idx;
                int ans = fenwick.get(R);
                int gt = sumSlow(in, R, R);
                if (ans != gt) {
                    ans = fenwick.get(R);
                    cerr << "[" << R << "] ans = " << ans << ", gt = " << gt << endl;
                }
                assert(ans == gt);
            } else {
                int R = RandInt32::get() % N;
                int ans = fenwick.get(R);
                int gt = sumSlow(in, R, R);
                if (ans != gt) {
                    ans = fenwick.get(R);
                    cerr << "[" << R << "] ans = " << ans << ", gt = " << gt << endl;
                }
                assert(ans == gt);
            }
        }
    }
    cout << "*** lower bound test" << endl;
    {
        int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        FenwickTree<int> fenwick(in);

        for (int i = 0; i < N; i++) {
            int R = RandInt32::get() % N;
            int sum = sumSlow(in, 0, R);

            int ans = lowerBound(fenwick, sum);
            int gt = lowerBoundSlow(in, sum);
            if (ans != gt) {
                cerr << "[" << sum << "] ans = " << ans << ", gt = " << gt << endl;
                ans = lowerBound(fenwick, sum);
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
