#include <vector>
#include <functional>

using namespace std;

#include "generalBinaryIndexedTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>
#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "segmentTree.h"
#include "segmentTreeCompact.h"

static int sumSlow(vector<int>& v, int L, int R) {
    int res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

static int minSlow(vector<int>& v, int L, int R) {
    int res = numeric_limits<int>::max();
    while (L <= R)
        res = min(res, v[L++]);
    return res;
}

static int maxSlow(vector<int>& v, int L, int R) {
    int res = numeric_limits<int>::min();
    while (L <= R)
        res = max(res, v[L++]);
    return res;
}

void testGeneralBinaryIndexedTree() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- General FenwickTree (General Binary Indexed Tree) -------------------" << endl;
    {
                     // 0  1  2  3  4  5  6
        vector<int> in{ 1, 9, 2, 7, 5, 4, 6 };
        int N = (int)in.size();

        auto bitSum = makeGeneralBIT(in, [](int a, int b) { return a + b; }, 0);
        auto bitMin = makeGeneralBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        auto bitMax = makeGeneralBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());

        assert(bitSum.query(0) == 1);
        assert(bitSum.query(3) == 7);

        assert(bitSum.queryRange(0, 3) == 19);
        assert(bitSum.queryRange(1, 3) == 18);

        assert(bitMin.queryRange(0, 3) == 1);
        assert(bitMin.queryRange(1, 3) == 2);

        assert(bitMax.queryRange(0, 3) == 9);
        assert(bitMax.queryRange(1, 3) == 9);

        bitSum.update(1, 0);
        assert(bitSum.queryRange(0, 3) == 10);
        assert(bitSum.queryRange(1, 3) == 9);

        bitMin.update(2, 8);
        assert(bitMin.queryRange(0, 3) == 1);
        assert(bitMin.queryRange(1, 3) == 7);

        bitMax.update(0, 10);
        assert(bitMax.queryRange(0, 3) == 10);
        assert(bitMax.queryRange(1, 3) == 9);
    }
    {
        int MaxV = 100;
        int N = 100;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % MaxV;

        auto bitSum = makeGeneralBIT(in, [](int a, int b) { return a + b; }, 0);
        for (int i = 0; i < N; i++) {
            int idx = RandInt32::get() % N;
            int val = RandInt32::get() % MaxV;

            in[idx] += val;
            bitSum.add(idx, val);

            for (int j = i; j < N; j++)
                assert(bitSum.queryRange(i, j) == sumSlow(in, i, j));
        }
    }
    {
        int MaxV = 10000;
        int N = 1024;
#ifdef _DEBUG
        int N = 100;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % MaxV;

        auto bitSum = makeGeneralBIT(in, [](int a, int b) { return a + b; }, 0);
        auto bitMin = makeGeneralBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        auto bitMax = makeGeneralBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());

        for (int i = 0; i < N; i++) {
            int idx = RandInt32::get() % N;
            int val = RandInt32::get() % MaxV;

            in[idx] = val;
            bitSum.update(idx, val);
            bitMin.update(idx, val);
            bitMax.update(idx, val);

            for (int j = i; j < N; j++) {
#ifndef _DEBUG
                if (bitSum.queryRange(i, j) != sumSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMin.queryRange(i, j) != minSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMax.queryRange(i, j) != maxSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
#endif
                assert(bitSum.queryRange(i, j) == sumSlow(in, i, j));
                assert(bitMin.queryRange(i, j) == minSlow(in, i, j));
                assert(bitMax.queryRange(i, j) == maxSlow(in, i, j));
            }
        }
    }
    {
        cout << "-- Performance Test -----------------------" << endl;
        cout << "*** Segment tree vs Compact segment tree vs GeneralBIT" << endl;
        {
            int N = 500000;
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
                auto bit = makeGeneralBIT(T, [](int a, int b) { return min(a, b); }, INT_MAX);
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        res += bit.queryRange(it.first, it.second);
                    }
                }
                cout << "result = " << res << endl;
            }
            PROFILE_STOP(2);
        }
    }

    cout << "OK!" << endl;
}
