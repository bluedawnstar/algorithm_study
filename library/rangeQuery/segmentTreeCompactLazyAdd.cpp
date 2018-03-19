#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "fenwickTree.h"
#include "segmentTreeCompactLazyAdd.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int findNext(const vector<int>& A, int start, int x) {
    while (start < (int)A.size()) {
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

void testSegmentTreeCompactLazyAdd() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Compact Segment Tree - Lazy Add ----------------------------------------" << endl;
    {
        vector<int> freq{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
        int N = (int)freq.size();

        FenwickTree<int> fenwick;
        auto segTree = makeCompactSegmentTreeLazyAdd(freq, [](int a, int b) { return a + b; });

        fenwick.build(freq);

        int ans = fenwick.sum(5);
        assert(ans == 12);

        ans = segTree.query(0, 5);
        assert(ans == 12);

        fenwick.add(3, 6);
        segTree.add(3, 6);

        ans = fenwick.sum(5);
        assert(ans == 18);

        ans = segTree.query(0, 5);
        assert(ans == 18);

        ans = fenwick.sumRange(1, 5);
        assert(ans == 16);

        ans = segTree.query(1, 5);
        assert(ans == 16);
    }
    cout << "*** findNext() & findPrev()" << endl;
    {
#ifdef _DEBUG
        static const int T = 100;
        int N = 10000;
#else
        static const int T = 1000;
        int N = 100000;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % (T * 10);

        auto seg = makeCompactSegmentTreeLazyAdd(in, [](int a, int b) { return min(a, b); });
        for (int i = N - 1; i >= 0; i--) {
            int idx = RandInt32::get() % N;
            int t = RandInt32::get() % (T * 10);

            seg.add(idx, t - in[idx]);
            in[idx] = t;

            int ans1 = seg.findNext(i, [](int x) {
                return x <= T;
            });
            int gt1 = findNext(in, i, T);
            assert(gt1 == ans1);

            int ans2 = seg.findPrev(i, [](int x) {
                return x <= T;
            });
            int gt2 = findPrev(in, i, T);
            assert(gt2 == ans2);
        }
    }
    {
        static const int T = 1000;
        int N = 1000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % (T * 10);

        auto seg = makeCompactSegmentTreeLazyAdd(in, [](int a, int b) { return min(a, b); });
        for (int i = N - 1; i >= 0; i--) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            int t = RandInt32::get() % 10;
            if (L > R)
                swap(L, R);

            seg.addRange(L, R, t);
            for (int j = L; j <= R; j++)
                in[j] += t;

            int ans1 = seg.findNext(i, [](int x) {
                return x <= T;
            });
            int gt1 = findNext(in, i, T);
            assert(gt1 == ans1);

            int ans2 = seg.findPrev(i, [](int x) {
                return x <= T;
            });
            int gt2 = findPrev(in, i, T);
            assert(gt2 == ans2);
        }
    }
    cout << "OK!" << endl;

    cout << "-- Compact Segment Tree Performance Test -----------------------" << endl;
    cout << "*** Fenwick Tree vs Compact Segment Tree Lazy Add" << endl;
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

        PROFILE_START(0);
        {
            int res = 0;
            FenwickTree<int> ft(T);
            for (int i = 0; i < 10; i++) {
                for (auto& it : Q) {
                    res += ft.sumRange(it.first, it.second);
                }
            }
            cout << "result = " << res << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            int res = 0;
            auto seg = makeCompactSegmentTreeLazyAdd(T, [](int a, int b) { return a + b; });
            for (int i = 0; i < 10; i++) {
                for (auto& it : Q) {
                    res += seg.query(it.first, it.second);
                }
            }
            cout << "result = " << res << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
