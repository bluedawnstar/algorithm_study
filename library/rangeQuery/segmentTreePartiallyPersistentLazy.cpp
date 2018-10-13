#include <memory.h>
#include <queue>
#include <stack>
#include <unordered_map>
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
        int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree1(in, [](int a, int b) { return a + b; }, 0);
        PartiallyPersistentSegmentTreeLazy<int> tree2(in, [](int a, int b) { return a + b; }, 0);

        for (int i = 0; i < N; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            int gt = sumSlow(in, L, R);

            auto ans1 = tree1.query(L, R);
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
        int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree1(in, [](int a, int b) { return a + b; }, 0);
        PartiallyPersistentSegmentTreeLazy<int> tree2(in, [](int a, int b) { return a + b; }, 0);

        for (int i = 0; i < N; i++) {
            int R = RandInt32::get() % N;
            int sum = sumSlow(in, 0, R);

            auto ans1 = tree1.lowerBound([sum](int val) { return val >= sum; });
            auto ans2 = tree2.lowerBound([sum](int val) { return val >= sum; });
            int gt = lowerBoundSlow(in, sum);
            if (ans1 != gt) {
                cerr << "[" << sum << "] ans = " << ans1 << ", gt = " << gt << endl;
                ans1 = tree1.lowerBound([sum](int val) { return val >= sum; });
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
        int N = 100;
        int T = 100;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree(in, [](int a, int b) { return a + b; }, 0);
        PartiallyPersistentSegmentTreeLazy<int> tree2(in, [](int a, int b) { return a + b; }, 0);

        vector<vector<pair<int, int>>> gt(T, vector<pair<int, int>>(N));    // (sum, index)
        for (int j = 0; j < T; j++) {
            for (int i = 0; i < N; i++) {
                int R = RandInt32::get() % N;
                int sum = sumSlow(in, 0, R);
                gt[j][i].first = sum;
                gt[j][i].second = lowerBoundSlow(in, sum);
            }

            int R = RandInt32::get() % N;
            int newVal = RandInt32::get() % 1000;
            tree.upgrade(R, newVal);
            tree2.update(R, newVal);
            in[R] = newVal;
        }

        for (int j = 0; j < T; j++) {
            for (int i = 0; i < N; i++) {
                int sum = gt[j][i].first;

                auto ans1 = tree.lowerBound(j, [sum](int val) { return val >= sum; });
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
        int T = 100000;
        int N = 1000000;
#ifdef _DEBUG
        T = 1000;
        N = 10000;
#endif
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        auto segTree = makePersistentSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTreeLazy = makePersistentSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
        auto segTreeLazy2 = makePartiallyPersistentSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });

        //segTree.upgrade(3, 7);
        segTree.upgradeRange(3, 4, 7);
        segTreeLazy.upgradeRange(3, 4, 7);
        segTreeLazy2.updateRange(3, 4, 7);

        vector<int> history;
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

                int h = RandInt32::get() % segTree.getHistorySize();
                history.push_back(h);

                int a = segTree.query(h, L, R);
                int b = segTreeLazy.query(h, L, R);
                int c = segTreeLazy2.query(h, L, R);
                if (a != b || a != c) {
                    cout << "FAIL!" << endl;
                    assert(a == b);
                    assert(a == c);
                }
            } else {
                continue;

                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree.upgradeRange(L, R, x);
                segTreeLazy.upgradeRange(L, R, x);
                segTreeLazy2.updateRange(L, R, x);

                int a = segTree.query(L, R);
                int b = segTreeLazy.query(L, R);
                int c = segTreeLazy2.query(L, R);
                if (a != b || a != c) {
                    cout << "FAIL!" << endl;
                    assert(a == b);
                    assert(a == c);
                }
            }
        }
    }
    cout << "OK!" << endl;
    cout << "-- Persistent Segment Tree Performance Test -----------------------" << endl;
    cout << "*** Persistent segment tree vs persistent segment tree with lazy propagation" << endl;
    {
        int T = 100000;
        int N = 100000;
#ifdef _DEBUG
        T = 1000;
        N = 10000;
#endif
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        auto segTree = makePersistentSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTreeLazy = makePersistentSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });

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

                int a = segTree.query(L, R);
                int b = segTreeLazy.query(L, R);
                if (a != b) {
                    cout << "FAIL in query()" << endl;
                    assert(a == b);
                }
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                int a = segTree.updateRange(L, R, x);
                int b = segTreeLazy.updateRange(L, R, x);
                if (a != b) {
                    cout << "FAIL in update()" << endl;
                    assert(a == b);
                }
            }
        }
        cout << "OK!" << endl;

        PROFILE_START(0);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (segTree.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree.updateRange(L, R, x);
            }
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (segTreeLazy.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTreeLazy.updateRange(L, R, x);
            }
        }
        PROFILE_STOP(1);
    }
    {
        int T = 1000000;
        int N = 1000000;
#ifdef _DEBUG
        T = 1000;
        N = 10000;
#endif
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        int cnt = 2;
        vector<int> history;
        vector<tuple<int, int, int, int>> Q;
        for (int i = 0; i < T; i++) {
            if (i != 5) {
                int L = RandInt32::get() % int(in.size());
                int R = RandInt32::get() % int(in.size());
                if (L > R)
                    swap(L, R);
                Q.emplace_back(1, L, R, 0);
                history.push_back(RandInt32::get() % cnt);
            } else {
                int x = RandInt32::get() % 100;
                Q.emplace_back(0, 0, N - 1, x);
                cnt++;
            }
        }

        {
            auto segTree = makePersistentSegmentTree(in, [](int a, int b) { return a + b; });
            segTree.upgradeRange(3, 4, 7);

            PROFILE_START(0);
            int h = 0;
            for (auto& q : Q) {
                if (get<0>(q)) {
                    int L = get<1>(q);
                    int R = get<2>(q);

                    if (segTree.query(min(segTree.getHistorySize() - 1, history[h++]), L, R) == INT_MAX)
                        cout << "It'll Never be shown!" << endl;
                } else {
                    int L = get<1>(q);
                    int R = get<2>(q);
                    int x = get<3>(q);
                    segTree.upgradeRange(L, R, x);
                }
            }
            PROFILE_STOP(0);
        }
        {
            auto segTreeLazy = makePersistentSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
            segTreeLazy.upgradeRange(3, 4, 7);

            PROFILE_START(1);
            int h = 0;
            for (auto& q : Q) {
                if (get<0>(q)) {
                    int L = get<1>(q);
                    int R = get<2>(q);

                    if (segTreeLazy.query(min(segTreeLazy.getHistorySize() - 1, history[h++]), L, R) == INT_MAX)
                        cout << "It'll Never be shown!" << endl;
                } else {
                    int L = get<1>(q);
                    int R = get<2>(q);
                    int x = get<3>(q);
                    segTreeLazy.upgradeRange(L, R, x);
                }
            }
            PROFILE_STOP(1);
        }
        {
            auto segTreeLazy2 = makePartiallyPersistentSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
            segTreeLazy2.updateRange(3, 4, 7);

            PROFILE_START(2);
            int h = 0;
            for (auto& q : Q) {
                if (get<0>(q)) {
                    int L = get<1>(q);
                    int R = get<2>(q);

                    if (segTreeLazy2.query(min(segTreeLazy2.getHistorySize() - 1, history[h++]), L, R) == INT_MAX)
                        cout << "It'll Never be shown!" << endl;
                } else {
                    int L = get<1>(q);
                    int R = get<2>(q);
                    int x = get<3>(q);
                    segTreeLazy2.updateRange(L, R, x);
                }
            }
            PROFILE_STOP(2);
        }
    }

    cout << "OK!" << endl;
}
