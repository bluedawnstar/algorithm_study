#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "segmentTree.h"
#include "segmentTreeRMQ.h"
#include "sparseTableReverse.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "sparseTable.h"
#include "sparseTableMin.h"

void testReverseSparseTable() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Reverse Sparse Table --------------------------------" << endl;
    // RMQ
    {
        auto sparseTable = makeReverseSparseTable<int>(vector<int>{6, 1, 4, 3, 7, 1}, [](int a, int b) { return min(a, b); }, INT_MAX);
        RMQ rmq(vector<int>{6, 1, 4, 3, 7, 1});

        int ans, ansRMQ;

        ans = sparseTable.query(2, 5);
        ansRMQ = rmq.query(2, 5);
        assert(ans == ansRMQ);

        ans = sparseTable.query(0, 3);
        ansRMQ = rmq.query(0, 3);
        assert(ans == ansRMQ);
    }
    // SUM
    {
        int N = 10000;
        vector<int> inSum(N);
        for (int i = 0; i < N; i++)
            inSum[i] = RandInt32::get() % 65536;

        vector<int> prefixSum(N + 1);
        prefixSum[0] = 0;
        for (int i = 1; i <= N; i++) {
            prefixSum[i] = prefixSum[i - 1] + inSum[i - 1];
        }

        auto sparseTable = makeReverseSparseTable<int>(inSum, [](int a, int b) { return a + b; }, 0);

        for (int i = 0; i < 10000; i++) {
            int left = RandInt32::get() % N;
            int right = RandInt32::get() % N;
            if (left > right)
                swap(left, right);

            int ans1 = prefixSum[right + 1] - prefixSum[left];
            int ans2 = sparseTable.queryNoOverlap(left, right);
            assert(ans1 == ans2);
        }
    }
    cout << "OK!" << endl;

    cout << "--- Performance Test : Sparse Table & Reverse Sparse Table --------" << endl;
    {
        int N = 1000000;
        int TN = 10;
#ifdef _DEBUG
        N = 10000;
#endif
        vector<int> T(N);
        for (int i = 0; i < N; i++)
            T[i] = RandInt32().get() % 65536;

        vector<pair<int, int>> Q;
        for (int i = 0; i < N; i++) {
            int a = RandInt32::get() % N;
            int b = RandInt32::get() % N;
            Q.push_back({ min(a, b), max(a, b) });
        }

        PROFILE_START(0);
        {
            int res = 0;

            auto st = makeSparseTable(T, [](int a, int b) { return min(a, b); }, INT_MAX);
            for (int i = 0; i < TN; i++) {
                for (auto& it : Q) {
                    res += st.query(it.first, it.second);
                }
            }
            if (res == 0)
                cout << "It's just for preventing optimization" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        {
            int res = 0;

            auto st = makeReverseSparseTable(T, [](int a, int b) { return min(a, b); }, INT_MAX);
            for (int i = 0; i < TN; i++) {
                for (auto& it : Q) {
                    res += st.query(it.first, it.second);
                }
            }
            if (res == 0)
                cout << "It's just for preventing optimization" << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
