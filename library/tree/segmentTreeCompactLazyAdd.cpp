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

static int rand16() {
    return rand() % 32768;
}

static int rand32() {
    return rand16() * rand16();
}

void testSegmentTreeCompactLazyAdd() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Compact Segment Tree - Lazy Add ----------------------------------------" << endl;

    int freq[] = { 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
    int N = sizeof(freq) / sizeof(freq[0]);

    FenwickTree<int> fenwick(N);
    auto segTree = makeCompactSegmentTreeLazyAdd(freq, N, [](int a, int b) { return a + b; });

    fenwick.init(freq, N);

    int ans = fenwick.sum(5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 12);

    ans = segTree.query(0, 5);
    cout << "segTree.query(0, 5) = " << ans << endl;
    assert(ans == 12);

    fenwick.add(3, 6);
    segTree.add(3, 6);
    cout << "after add(3, 6)" << endl;

    ans = fenwick.sum(5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 18);

    ans = segTree.query(0, 5);
    cout << "segTree.query(0, 5) = " << ans << endl;
    assert(ans == 18);

    ans = fenwick.sumRange(1, 5);
    cout << "fenwick.rangeSum(1, 5) = " << ans << endl;
    assert(ans == 16);

    ans = segTree.query(1, 5);
    cout << "segTree.query(1, 5) = " << ans << endl;
    assert(ans == 16);

    cout << "OK!" << endl;

    cout << "-- Compact Segment Tree Performance Test -----------------------" << endl;
    cout << "*** Fenwick Tree vs Compact Segment Tree Lazy Add" << endl;
    {
        int N = 1000000;

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
            FenwickTree<int> ft(N);

            ft.init(T, N);
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
