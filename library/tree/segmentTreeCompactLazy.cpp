#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "segmentTree.h"
#include "segmentTreeLazy.h"
#include "segmentTreeCompact.h"
#include "segmentTreeCompactLazy.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static int rand16() {
    return rand() % 32768;
}

static int rand32() {
    return rand16() * rand16();
}

void testSegmentTreeCompactLazy() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Compact Segment Tree Lazy ----------------------------------------" << endl;

    auto segTree = makeCompactSegmentTreeLazyUpdate(vector<int>{6, 5, 4, 3, 2, 1}, [](int a, int b) { return a + b; }, [](int a, int k) { return a * k; });
    auto segTree2 = makeCompactSegmentTreeLazyUpdate(vector<int>{6, 5, 4, 3, 2, 1}, [](int a, int b) { return min(a, b); }, [](int a, int k) { return a; }, INT_MAX);

    int ans;

    ans = segTree.query(1, 3);
    cout << ans << endl;
    assert(ans == 12);

    segTree.update(2, 10);
    ans = segTree.query(1, 3);
    cout << ans << endl;
    assert(ans == 18);

    ans = segTree2.query(1, 3);
    cout << ans << endl;
    assert(ans == 3);

    segTree2.update(2, -10);
    ans = segTree2.query(1, 3);
    cout << ans << endl;
    assert(ans == -10);

    segTree.updateRange(0, 2, 3);
    ans = segTree.query(1, 3);
    cout << ans << endl;
    assert(ans == 9);

    segTree2.updateRange(0, 2, 2);
    ans = segTree2.query(1, 3);
    cout << ans << endl;
    assert(ans == 2);

    cout << "-- Compact Segment Tree Performance Test -----------------------" << endl;
    cout << "*** Segment tree vs compact segment tree" << endl;
    {
        int N = 10000;

#if _DEBUG
        N = 10000;
#endif

        vector<int> T(N);
        for (int i = 0; i < N; i++)
            T[i] = rand16();

        vector<pair<int, int>> Q;
        for (int i = 0; i < N; i++) {
            int a = rand32() % N;
            int b = rand32() % N;
            Q.push_back({ min(a, b), max(a, b) });
        }

        PROFILE_START(0);
        {
            int res = 0;
            auto seg = makeSegmentTree(T, [](int a, int b) { return min(a, b); }, INT_MAX);
            for (int i = 0; i < 10; i++) {
                for (auto& it : Q) {
                    res += seg.query(it.first, it.second);
                }
            }
            cout << "result = " << res << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            int res = 0;
            auto seg = makeCompactSegmentTree(T, [](int a, int b) { return min(a, b); }, INT_MAX);
            for (int i = 0; i < 10; i++) {
                for (auto& it : Q) {
                    res += seg.query(it.first, it.second);
                }
            }
            cout << "result = " << res << endl;
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        {
            int res = 0;
            auto seg = makeCompactSegmentTreeLazyUpdate(T, [](int a, int b) { return min(a, b); }, [](int a, int k) { return a; }, INT_MAX);
            for (int i = 0; i < 10; i++) {
                for (auto& it : Q) {
                    res += seg.query(it.first, it.second);
                }
            }
            cout << "result = " << res << endl;
        }
        PROFILE_STOP(2);
    }
    cout << "*** Segment tree vs compact segment tree with range update & query" << endl;
    {
        int T = 100000;
        int N = 100000;
        vector<int> in(N);

        auto segTree = makeSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTree1 = makeSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int k) { return a * k; });
        auto segTree2 = makeCompactSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTree3 = makeCompactSegmentTreeLazyUpdate(in, [](int a, int b) { return a + b; }, [](int a, int k) { return a * k; });

        vector<tuple<int, int, int, int>> Q;
        for (int i = 0; i < T; i++) {
            if (rand() % 2) {
                int L = rand() % (int)in.size();
                int R = rand() % (int)in.size();
                if (L > R)
                    swap(L, R);
                Q.emplace_back(1, L, R, 0);
            } else {
                int L = rand() % (int)in.size();
                int R = rand() % (int)in.size();
                int x = rand() % 100;
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
                int b = segTree2.query(L, R);
                int c = segTree3.query(L, R);
                if (a != b)
                    cout << "It'll Never be shown!" << endl;
                if (a != c)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree.updateRange(L, R, x);
                segTree2.updateRange(L, R, x);
                segTree3.updateRange(L, R, x);
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
                if (segTree1.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree1.updateRange(L, R, x);
            }
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (segTree2.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree2.updateRange(L, R, x);
            }
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (segTree3.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree3.updateRange(L, R, x);
            }
        }
        PROFILE_STOP(3);
    }

    cout << "OK!" << endl;
}
