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
#include "../common/rand.h"

static int findNext(const vector<int>& A, int start, int x) {
    while (start < int(A.size())) {
        if (A[start] <= x)
            return start;
        start++;
    }
    return -1;
}

static int findPrev(const vector<int>& A, int start, int x) {
    while (start >= 0) {
        if (A[start] <= x)
            return start;
        start--;
    }
    return -1;
}

void testSegmentTreeCompactLazy() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Compact Segment Tree Lazy ----------------------------------------" << endl;
    {
        auto segTree = makeCompactSegmentTreeLazyUpdate(vector<int>{6, 5, 4, 3, 2, 1},
            [](int a, int b) { return a + b; }, [](int a, int k) { return a * k; });
        auto segTree2 = makeCompactSegmentTreeLazyUpdate(vector<int>{6, 5, 4, 3, 2, 1},
            [](int a, int b) { return min(a, b); }, [](int a, int k) { return a; }, numeric_limits<int>::max());

        int ans;

        ans = segTree.query(1, 3);
        assert(ans == 12);

        segTree.update(2, 10);
        ans = segTree.query(1, 3);
        assert(ans == 18);

        ans = segTree2.query(1, 3);
        assert(ans == 3);

        segTree2.update(2, -10);
        ans = segTree2.query(1, 3);
        assert(ans == -10);

        segTree.updateRange(0, 2, 3);
        ans = segTree.query(1, 3);
        assert(ans == 9);

        segTree2.updateRange(0, 2, 2);
        ans = segTree2.query(1, 3);
        assert(ans == 2);
    }
    cout << "*** findNext() & findPrev()" << endl;
    {
#ifdef _DEBUG
        static const int T = 100;
        int N = 1000;
#else
        static const int T = 1000;
        int N = 10000;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % (T * 10);

        auto seg = makeCompactSegmentTreeLazyUpdate(in,
            [](int a, int b) { return min(a, b); }, [](int a, int k) { return a; }, numeric_limits<int>::max(), true);
        for (int i = N - 1; i >= 0; i--) {
            int index = RandInt32::get() % N;
            int t = RandInt32::get() % (T * 10);

            seg.update(index, t);
            in[index] = t;

            int ans1 = findNext<int>(seg, i, [](int x) {
                return x <= T;
            });
            int gt1 = findNext(in, i, T);
            assert(gt1 == ans1);

            int ans2 = findPrev<int>(seg, i, [](int x) {
                return x <= T;
            });
            int gt2 = findPrev(in, i, T);
            assert(gt2 == ans2);
        }
    }
    {
#ifdef _DEBUG
        static const int T = 100;
        int N = 1000;
#else
        static const int T = 1000;
        int N = 10000;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % (T * 10);

        auto seg = makeCompactSegmentTreeLazyUpdate(in,
            [](int a, int b) { return min(a, b); }, [](int a, int k) { return a; }, numeric_limits<int>::max(), true);
        for (int i = N - 1; i >= 0; i--) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            int t = RandInt32::get() % (T * 10);
            if (L > R)
                swap(L, R);

            seg.updateRange(L, R, t);
            for (int j = L; j <= R; j++)
                in[j] = t;

            int ans1 = findNext<int>(seg, i, [](int x) {
                return x <= T;
            });
            int gt1 = findNext(in, i, T);
            assert(gt1 == ans1);

            int ans2 = findPrev<int>(seg, i, [](int x) {
                return x <= T;
            });
            int gt2 = findPrev(in, i, T);
            assert(gt2 == ans2);
        }
    }
    cout << "-- Compact Segment Tree Performance Test -----------------------" << endl;
    cout << "*** Segment tree vs compact segment tree" << endl;
    {
        int N = 10000;
#ifdef _DEBUG
        N = 1000;
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

        PROFILE_START(0);
        {
            int res = 0;
            auto seg = makeSegmentTree(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
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
            auto seg = makeCompactSegmentTree(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
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
            auto seg = makeCompactSegmentTreeLazyUpdate(T,
                [](int a, int b) { return min(a, b); }, [](int a, int k) { return a; }, numeric_limits<int>::max());
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
#ifdef _DEBUG
        T = 1000;
        N = 1000;
#endif

        vector<int> in(N);

        auto segTree = makeSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTree1 = makeSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int k) { return a * k; });
        auto segTree2 = makeCompactSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTree3 = makeCompactSegmentTreeLazyUpdate(in, [](int a, int b) { return a + b; }, [](int a, int k) { return a * k; });

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
                if (segTree.query(L, R) == numeric_limits<int>::max())
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
                if (segTree1.query(L, R) == numeric_limits<int>::max())
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
                if (segTree2.query(L, R) == numeric_limits<int>::max())
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
                if (segTree3.query(L, R) == numeric_limits<int>::max())
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
