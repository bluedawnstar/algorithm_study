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
    for (int i = 0; i < int(v.size()); i++) {
        res += v[i];
        if (res >= k)
            return i;
    }
    return int(v.size());
}

void testCaseFenwickTree(int N, int X, int T) {
    vector<int> in(N);
    for (int i = 0; i < N; i++)
        in[i] = RandInt32::get() % X;

    FenwickTree<int> fenwick(in);
    {
        FenwickTree<int> ft;
        ft.init(N);
        for (int i = 0; i < N; i++)
            ft.add(i, in[i]);

        if (fenwick.tree != ft.tree) {
            cerr << "Mismatched tree structure at " << __LINE__ << " in " << __FILE__ << endl;
        }
        assert(fenwick.tree == ft.tree);
    }
    for (int i = 0; i < T; i++) {
        if ((RandInt32::get() & 1) == 0) {
            int idx = RandInt32::get() % N;
            int val = RandInt32::get() % X;
            fenwick.add(idx, val - in[idx]);
            in[idx] = val;
        } else {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            int ans = fenwick.sumRange(L, R);
            int gt = sumSlow(in, L, R);
            if (ans != gt) {
                cerr << "Mismatched (" << L << ", " << R << ") : ans = " << ans << ", gt = " << gt << endl;
            }
            assert(ans == gt);
        }
    }
}

void testFenwickTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- FenwickTree (Binary Indexed Tree) -------------------" << endl;
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 7, 8, 6, 11 };
        int N = int(in.size());

        FenwickTree<int> fenwick(in);

        int ans = fenwick.sum(5);
        cout << "fenwick.sum(5) = " << ans << endl;
        assert(ans == 12);

        fenwick.add(3, 6);
        in[3] += 6;
        cout << "after fenwick.add(3, 6)" << endl;

        ans = fenwick.sum(5);
        cout << "fenwick.sum(5) = " << ans << endl;
        assert(ans == 18);

        ans = fenwick.sumRange(1, 5);
        cout << "fenwick.rangeSum(1, 5) = " << ans << endl;
        assert(ans == 16);

        fenwick.add(0, 1);
        in[0] += 1;
        assert(fenwick.sum(15) == sumSlow(in, 0, 15));

        fenwick.add(15, 1);
        in[15] += 1;
        assert(fenwick.sumRange(0, 15) == sumSlow(in, 0, 15));
    }
    {
        vector<int> in{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
        int N = int(in.size());

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
    cout << "OK!" << endl;
    testCaseFenwickTree(1024  , 100, 10000);
    cout << "OK!" << endl;
    testCaseFenwickTree(1024-1, 100, 10000);
    cout << "OK!" << endl;
    testCaseFenwickTree(1024+1, 100, 10000);
    cout << "OK!" << endl;
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

            int ans = fenwick.lowerBound(sum);
            int gt = lowerBoundSlow(in, sum);
            if (ans != gt) {
                cerr << "[" << sum << "] ans = " << ans << ", gt = " << gt << endl;
                ans = fenwick.lowerBound(sum);
            }
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
}
