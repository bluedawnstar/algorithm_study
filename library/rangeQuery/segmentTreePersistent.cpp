#include <memory.h>
#include <queue>
#include <stack>
#include <functional>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "segmentTreePersistent.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "segmentTree.h"
#include "segmentTreeRMQ.h"

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

void testSegmentTreePersistent() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Persistent Segment Tree ----------------------------------------" << endl;
    {
        const int T = 1000;
        const int N = 100;

        vector<vector<int>> vv(1);
        vv[0].resize(N);

        PersistentSegmentTree<int> tree(N, [](int a, int b) { return a + b; }, 0);

        int root = tree.getInitRoot();
        vector<int> roots;
        roots.push_back(root);
        for (int i = 1; i <= T; i++) {
            int idx = RandInt32::get() % N;
            int x = RandInt32::get() % 10000;
            vv.push_back(vv.back());
            if (rand() % 2) {
                vv.back()[idx] += x;
                root = tree.update(root, idx, x);
            } else {
                vv.back()[idx] = x;
                root = tree.set(root, idx, x);
            }
            roots.push_back(root);

            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int qryRoot = RandInt32::get() % i + 1;
            {
                int gt = sumSlow(vv[qryRoot], L, R);
                int ans = tree.query(roots[qryRoot], L, R);
                if (ans != gt)
                    cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(ans == gt);
            }
            {
                int sum = tree.query(roots[qryRoot], 0, L) + RandInt32::get() % 10000;
                int gt = lowerBoundSlow(vv[qryRoot], sum);
                int ans = tree.lowerBound(roots[qryRoot], [sum](int x) { return sum <= x; });
                if (ans != gt)
                    cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
                assert(ans == gt);
            }
        }
    }
    cout << "OK!" << endl;
    {
        const int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTree<int> tree(in, [](int a, int b) { return a + b; }, 0);
        int root = tree.getInitRoot();
        for (int i = 0; i < N; i++) {
            int R = RandInt32::get() % N;
            int sum = sumSlow(in, 0, R);

            auto ans = tree.lowerBound(root, [sum](int val) { return val >= sum; });
            int gt = lowerBoundSlow(in, sum);
            if (ans != gt) {
                cout << "[" << sum << "] ans = " << ans << ", gt = " << gt << endl;
                ans = tree.lowerBound(root, [sum](int val) { return val >= sum; });
            }
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
    {
        const int T = 100;
        const int N = 100;
        const int X = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % X;

        PersistentSegmentTree<int> tree(in, [](int a, int b) { return a + b; }, 0);
        vector<int> roots;
        int root = tree.getInitRoot();

        vector<vector<pair<int, int>>> gt(T, vector<pair<int, int>>(N));    // (sum, index)
        for (int j = 0; j < T; j++) {
            for (int i = 0; i < N; i++) {
                int R = RandInt32::get() % N;
                int sum = sumSlow(in, 0, R);
                gt[j][i].first = sum;
                gt[j][i].second = lowerBoundSlow(in, sum);
            }
            roots.push_back(root);

            int R = RandInt32::get() % N;
            int newVal = RandInt32::get() % X;
            if (j & 1) {
                root = tree.update(root, R, newVal);
                in[R] += newVal;
            } else {
                root = tree.set(root, R, newVal);
                in[R] = newVal;
            }
        }

        for (int j = 0; j < T; j++) {
            for (int i = 0; i < N; i++) {
                int sum = gt[j][i].first;

                auto ans = tree.lowerBound(roots[j], [sum](int val) { return val >= sum; });
                if (ans != gt[j][i].second) {
                    cout << "[" << sum << "] ans = " << ans << ", gt = " << gt[j][i].second << endl;
                    ans = tree.lowerBound(roots[j], [sum](int val) { return val >= sum; });
                }
                assert(ans == gt[j][i].second);
            }
        }
    }
    cout << "OK!" << endl;
    cout << "** Persistent segment tree vs RMQ" << endl;
    {
        int N = 1000000;
#ifdef _DEBUG
        N = 10000;
#endif

        vector<int> T(N);
        for (int i = 0; i < N; i++)
            T[i] = RandInt32::get() % 65536;

        vector<pair<int, int>> Q;
        for (int i = 0; i < N; i++) {
            int a = RandInt32::get() % N;
            int b = RandInt32::get() % N;
            Q.push_back({ min(a, b), max(a, b) });
        }

        PROFILE_START(pst);
        {
            int res = 0;
            auto seg = makePersistentSegmentTree(T, [](int a, int b) { return min(a, b); }, INT_MAX);
            for (int i = 0; i < 10; i++) {
                for (auto& it : Q) {
                    res += seg.query(seg.getInitRoot(), it.first, it.second);
                }
            }
            cout << "result = " << res << endl;
        }
        PROFILE_STOP(pst);

        PROFILE_START(rmq);
        {
            int res = 0;
            RMQ<int> rmq(T);
            for (int i = 0; i < 10; i++) {
                for (auto& it : Q) {
                    res += rmq.query(it.first, it.second);
                }
            }
            cout << "result = " << res << endl;
        }
        PROFILE_STOP(rmq);
    }
    cout << "OK!" << endl;
}
