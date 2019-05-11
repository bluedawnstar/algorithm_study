#include <functional>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePersistentLazySimple.h"

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

static void updateSlow(vector<int>& v, int L, int R, int x) {
    while (L <= R)
        v[L++] += x;
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

static long long sumSlow(const vector<long long>& v, int L, int R) {
    long long res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

static void updateSlow(vector<long long>& v, int L, int R, int x) {
    while (L <= R)
        v[L++] += x;
}

void testSegmentTreePersistentLazySimple() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Lazy Persistent Segment Tree --------------------------------------" << endl;
    {
        int N = 100;
        int T = 1000;

        vector<vector<int>> vv(1);
        vv[0].resize(N);

        SimplePersistentSegmentTreeLazy<int> tree(N, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; }, 0);

        int root = tree.getInitRoot();
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            int x = RandInt32::get() % 1000;
            {
                int gt = sumSlow(vv.back(), L, R);
                int ans = tree.query(root, L, R);
                assert(gt == ans);
            }
            {
                int sum = tree.query(root, 0, L) + RandInt32::get() % 1000;
                int gt = lowerBoundSlow(vv.back(), sum);
                int ans = tree.lowerBound(root, [sum](int x) { return sum <= x; });
                assert(gt == ans);
            }

            vv.push_back(vv.back());
            updateSlow(vv.back(), L, R, x);

            root = tree.update(root, L, R, x);
        }
    }
    {
        int T = 1000;
        int N = 10000;
        int maxX = 100000;

#ifdef _DEBUG
        T = 100;
        N = 1000;
#endif

        for (int t = 0; t < T; t++) {
            vector<vector<long long>> v(T + 1);
            v[0].assign(N, 0);

            SimplePersistentSegmentTreeLazy<long long> tree(
                [](long long a, long long b) { return a + b; },
                [](long long x, int n) { return x * n; },
                0ll);
            vector<int> roots;
            roots.push_back(tree.build(N));

            for (int i = 0; i < T; i++) {
                int L = rand() % N;
                int R = rand() % N;
                if (L > R)
                    swap(L, R);

                //int history = i;
                int history = rand() % int(roots.size());

                int x = rand() % maxX + 1;

                auto ans = tree.query(roots[history], L, R);
                long long gt = sumSlow(v[history], L, R);
                if (ans != gt)
                    cout << "ERROR : mismatched!" << endl;
                assert(ans == gt);

                roots.push_back(tree.update(roots[history], L, R, x));
                v[i + 1] = v[history];
                updateSlow(v[i + 1], L, R, x);
            }
        }
    }

    cout << "OK!" << endl;
}
