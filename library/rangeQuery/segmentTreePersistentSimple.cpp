#include <memory.h>
#include <queue>
#include <stack>
#include <functional>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "segmentTreePersistentSimple.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
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

void testSegmentTreePersistentSimple() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Simple Persistent Segment Tree ----------------------------------------" << endl;
    {
        int N = 100;
        int T = 1000;

        vector<vector<int>> vv(1);
        vv[0].resize(N);

        SimplePersistentSegmentTree<int> tree(N, [](int a, int b) { return a + b; }, 0);

        for (int i = 1; i <= T; i++) {
            int idx = RandInt32::get() % N;
            int x = RandInt32::get() % 10000;
            vv.push_back(vv.back());
            if (rand() % 2) {
                vv.back()[idx] += x;
                tree.update(tree.roots.back(), idx, x);
            } else {
                vv.back()[idx] = x;
                tree.set(tree.roots.back(), idx, x);
            }

            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int root = RandInt32::get() % i + 1;
            {
                int gt = sumSlow(vv[root], L, R);
                int ans = tree.query(tree.roots[root], L, R);
                assert(gt == ans);
            }
            {
                int sum = tree.query(tree.roots[root], 0, L) + RandInt32::get() % 10000;
                int gt = lowerBoundSlow(vv[root], sum);
                int ans = tree.lowerBound(tree.roots[root], [sum](int x) { return sum <= x; });
                assert(gt == ans);
            }
        }
    }

    cout << "OK!" << endl;
}
