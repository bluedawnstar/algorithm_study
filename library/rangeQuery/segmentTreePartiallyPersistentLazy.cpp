#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePersistent.h"
#include "segmentTreePersistentLazy.h"
#include "segmentTreePartiallyPersistentLazy.h"

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

void testSegmentTreePartiallyPersistent() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Partially Persistent Segment Tree ----------------------------------------" << endl;
    {
        const int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree1(in, [](int a, int b) { return a + b; }, 0);
        PartiallyPersistentSegmentTreeLazy<int> tree2(in, [](int a, int b) { return a + b; }, 0);

        int root = tree1.getInitRoot();
        for (int i = 0; i < N; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            int gt = sumSlow(in, L, R);

            auto ans1 = tree1.query(root, L, R);
            auto ans2 = tree2.query(L, R);
            if (ans1 != gt)
                cerr << "Mismatched " << ans1 << ", " << gt << endl;
            if (ans2 != gt)
                cerr << "Mismatched " << ans2 << ", " << gt << endl;
            assert(ans1 == gt);
            assert(ans2 == gt);
        }
    }
    {
        const int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree1(in, [](int a, int b) { return a + b; }, 0);
        PartiallyPersistentSegmentTreeLazy<int> tree2(in, [](int a, int b) { return a + b; }, 0);

        int root = tree1.getInitRoot();
        for (int i = 0; i < N; i++) {
            int R = RandInt32::get() % N;
            int sum = sumSlow(in, 0, R);

            auto ans1 = tree1.lowerBound(root, [sum](int val) { return val >= sum; });
            auto ans2 = tree2.lowerBound([sum](int val) { return val >= sum; });
            int gt = lowerBoundSlow(in, sum);
            if (ans1 != gt) {
                cerr << "[" << sum << "] ans = " << ans1 << ", gt = " << gt << endl;
                ans1 = tree1.lowerBound(root, [sum](int val) { return val >= sum; });
            }
            if (ans2 != gt) {
                cerr << "[" << sum << "] ans = " << ans2 << ", gt = " << gt << endl;
                ans2 = tree2.lowerBound([sum](int val) { return val >= sum; });
            }
            assert(ans1 == gt);
            assert(ans2 == gt);
        }
    }
    cout << "OK!" << endl;
    {
        const int N = 100;
        const int T = 100;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree(in, [](int a, int b) { return a + b; }, 0);
        PartiallyPersistentSegmentTreeLazy<int> tree2(in, [](int a, int b) { return a + b; }, 0);
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
            int newVal = RandInt32::get() % 1000;
            root = tree.set(root, R, newVal);
            tree2.update(R, newVal);
            in[R] = newVal;
        }

        for (int j = 0; j < T; j++) {
            for (int i = 0; i < N; i++) {
                int sum = gt[j][i].first;

                auto ans1 = tree.lowerBound(roots[j], [sum](int val) { return val >= sum; });
                if (ans1 != gt[j][i].second) {
                    cerr << "[" << sum << "] ans = " << ans1 << ", gt = " << gt << endl;
                    ans1 = tree.lowerBound(j, [sum](int val) { return val >= sum; });
                }
                auto ans2 = tree2.lowerBound(j, [sum](int val) { return val >= sum; });
                if (ans2 != gt[j][i].second) {
                    cerr << "[" << sum << "] ans = " << ans2 << ", gt = " << gt << endl;
                    ans2 = tree.lowerBound(j, [sum](int val) { return val >= sum; });
                }
                assert(ans1 == gt[j][i].second);
                assert(ans2 == gt[j][i].second);
            }
        }
    }
    cout << "OK!" << endl;
    {
#ifdef _DEBUG
        const int T = 1000;
        const int N = 10000;
#else
        const int T = 100000;
        const int N = 1000000;
#endif
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        auto segTreeLazy = makePersistentSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
        auto segTreeLazy2 = makePartiallyPersistentSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });

        vector<int> roots;
        int root = segTreeLazy.getInitRoot();
        roots.push_back(root);

        root = segTreeLazy.update(root, 3, 4, 7);
        roots.push_back(root);
        segTreeLazy2.updateRange(3, 4, 7);

        vector<tuple<int, int, int, int>> Q;
        for (int i = 0; i < T; i++) {
            if (RandInt32::get() % 2) {
                int L = RandInt32::get() % int(in.size());
                int R = RandInt32::get() % int(in.size());
                if (L > R)
                    swap(L, R);
                Q.emplace_back(1, L, R, 0);
            } else {
                int L = RandInt32::get() % int(in.size());
                int R = RandInt32::get() % int(in.size());
                int x = RandInt32::get() % 100;
                if (L > R)
                    swap(L, R);
                Q.emplace_back(0, L, R, x);
            }
        }

        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);

                int h = RandInt32::get() % int(roots.size());

                int a = segTreeLazy.query(roots[h], L, R);
                int b = segTreeLazy2.query(h, L, R);
                if (a != b) {
                    cout << "FAIL!" << endl;
                    assert(a == b);
                }
            } else {
                continue;

                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                root = segTreeLazy.update(root, L, R, x);
                roots.push_back(root);
                segTreeLazy2.updateRange(L, R, x);

                int a = segTreeLazy.query(root, L, R);
                int b = segTreeLazy2.query(L, R);
                if (a != b) {
                    cout << "FAIL!" << endl;
                    assert(a == b);
                }
            }
        }
    }
    cout << "OK!" << endl;
}
