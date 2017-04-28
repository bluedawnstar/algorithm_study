#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeSegmentTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testSegmentTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Segment Tree ----------------------------------------" << endl;

    auto segTree = makeSegmentTree(vector<int>{6, 5, 4, 3, 2, 1}, [](int a, int b) { return a + b; });
    auto segTree2 = makeSegmentTree(vector<int>{6, 5, 4, 3, 2, 1}, [](int a, int b) { return min(a, b); }, INT_MAX);
    RMQ rmq(vector<int>{6, 5, 4, 3, 2, 1});

    int ans, ansRMQ;

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

    ansRMQ = rmq.query(1, 3);
    assert(ans == ansRMQ);

    segTree2.update(2, -10);
    ans = segTree2.query(1, 3);
    cout << ans << endl;
    assert(ans == -10);

    rmq.update(2, -10);
    ansRMQ = rmq.query(1, 3);
    assert(ans == ansRMQ);

    segTree.updateRange(0, 2, 3);
    ans = segTree.query(1, 3);
    cout << ans << endl;
    assert(ans == 9);

    segTree2.updateRange(0, 2, 2);
    ans = segTree2.query(1, 3);
    cout << ans << endl;
    assert(ans == 2);

    cout << "-- Segment Tree Performance Test -----------------------" << endl;

    {
        int N = 1000000;
        vector<int> T(N);
        for (int i = 0; i < N; i++)
            T[i] = rand();

        vector<pair<int, int>> Q;
        for (int i = 0; i < N; i++) {
            int a = rand() * rand() % N;
            int b = rand() * rand() % N;
            Q.push_back({ min(a, b), max(a, b) });
        }

        int res = 0;
        clock_t start = clock();
        auto seg = makeSegmentTree(T, [](int a, int b) { return min(a, b); }, INT_MAX);
        for (int i = 0; i < 10; i++) {
            for (auto& it : Q) {
                res += seg.query(it.first, it.second);
            }
        }
        cout << "elapsed time(" << res << ") : " << double(clock() - start) / CLOCKS_PER_SEC << endl;

        res = 0;
        start = clock();
        RMQ rmq(T);
        for (int i = 0; i < 10; i++) {
            for (auto& it : Q) {
                res += rmq.query(it.first, it.second);
            }
        }
        cout << "elapsed time(" << res << ") : " << double(clock() - start) / CLOCKS_PER_SEC << endl;
    }

    cout << "OK!" << endl;
}
