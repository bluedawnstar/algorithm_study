#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "segmentTree.h"
#include "segmentTreeCompact.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSegmentTreeCompact() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Compact Segment Tree ----------------------------------------" << endl;

    auto segTree = makeCompactSegmentTree(vector<int>{6, 5, 4, 3, 2, 1}, [](int a, int b) { return a + b; });
    auto segTree2 = makeCompactSegmentTree(vector<int>{6, 5, 4, 3, 2, 1}, [](int a, int b) { return min(a, b); }, INT_MAX);

    int ans;

    ans = segTree2.query(1, 3);
    cout << ans << endl;
    assert(ans == 3);

    segTree2.update(2, -10);
    ans = segTree2.query(1, 3);
    cout << ans << endl;
    assert(ans == -10);

    cout << "-- Compact Segment Tree Performance Test -----------------------" << endl;
    cout << "*** Segment tree vs compact segment tree" << endl;
    {
        int N = 1000000;

#if _DEBUG
        N = 100000;
#endif

        vector<int> T(N);
        for (int i = 0; i < N; i++)
            T[i] = rand();

        vector<pair<int, int>> Q;
        for (int i = 0; i < N; i++) {
            int a = rand() * rand() % N;
            int b = rand() * rand() % N;
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
    }

    cout << "OK!" << endl;
}
