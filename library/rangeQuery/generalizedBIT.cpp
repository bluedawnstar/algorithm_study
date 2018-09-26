#include <vector>
#include <functional>

using namespace std;

#include "generalizedBIT.h"

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

static void addSlow(vector<int>& v, int L, int R, int x) {
    while (L <= R)
        v[L++] += x;
}

static void updateSlow(vector<int>& v, int L, int R, int x) {
    while (L <= R)
        v[L++] = x;
}

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

static int lowerBound(const vector<int>& A, int start, int x) {
    while (start < int(A.size())) {
        if (A[start] >= x)
            break;
        start++;
    }
    return start;
}

static int lowerBoundBackward(const vector<int>& A, int start, int x) {
    while (start >= 0) {
        if (A[start] >= x)
            break;
        start--;
    }
    return start;
}

void testGeneralizedBIT() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Generalized Binary Indexed Tree (Generalized FenwickTree) -------------------" << endl;
    {
                     // 0  1  2  3  4  5  6
        vector<int> in{ 1, 9, 2, 7, 5, 4, 6 };
        int N = int(in.size());

        auto bitSum = makeGeneralizedBIT(in, [](int a, int b) { return a + b; }, 0);
        auto bitMin = makeGeneralizedBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        auto bitMax = makeGeneralizedBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());

        assert(bitSum.query(0) == 1);
        assert(bitSum.query(3) == 7);

        assert(bitSum.query(0, 3) == 19);
        assert(bitSum.query(1, 3) == 18);

        assert(bitMin.query(0, 3) == 1);
        assert(bitMin.query(1, 3) == 2);

        assert(bitMax.query(0, 3) == 9);
        assert(bitMax.query(1, 3) == 9);

        bitSum.update(1, 0);
        assert(bitSum.query(0, 3) == 10);
        assert(bitSum.query(1, 3) == 9);

        bitMin.update(2, 8);
        assert(bitMin.query(0, 3) == 1);
        assert(bitMin.query(1, 3) == 7);

        bitMax.update(0, 10);
        assert(bitMax.query(0, 3) == 10);
        assert(bitMax.query(1, 3) == 9);
    }
    {
        int MaxV = 100;
        int N = 100;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % MaxV;

        auto bitSum = makeGeneralizedBIT(in, [](int a, int b) { return a + b; }, 0);
        for (int i = 0; i < N; i++) {
            int idx = RandInt32::get() % N;
            int val = RandInt32::get() % MaxV;

            in[idx] += val;
            bitSum.add(idx, val);

            for (int j = i; j < N; j++)
                assert(bitSum.query(i, j) == sumSlow(in, i, j));
        }
    }
    {
        int MaxV = 10000;
        int N = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = N - i;

        auto bitSum = makeGeneralizedBIT(in, [](int a, int b) { return a + b; }, 0);
        auto bitMin = makeGeneralizedBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        auto bitMax = makeGeneralizedBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());

        for (int i = 0; i < N; i++) {
            int idx = RandInt32::get() % N;
            int val = RandInt32::get() % MaxV;

            in[idx] = val;
            bitSum.update(idx, val);
            bitMin.update(idx, val);
            bitMax.update(idx, val);

            for (int j = i; j < N; j++) {
#ifndef _DEBUG
                if (bitSum.query(i, j) != sumSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMin.query(i, j) != minSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMax.query(i, j) != maxSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
#endif
                assert(bitSum.query(i, j) == sumSlow(in, i, j));
                assert(bitMin.query(i, j) == minSlow(in, i, j));
                assert(bitMax.query(i, j) == maxSlow(in, i, j));
            }
        }
    }
    {
        int MaxV = 10000;
        int N = 1024;
#ifdef _DEBUG
        N = 100;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % MaxV;

        auto bitSum = makeGeneralizedBIT(in, [](int a, int b) { return a + b; }, 0);
        auto bitMin = makeGeneralizedBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        auto bitMax = makeGeneralizedBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());

        for (int i = 0; i < N; i++) {
            int idx = RandInt32::get() % N;
            int val = RandInt32::get() % MaxV;

            in[idx] = val;
            bitSum.update(idx, val);
            bitMin.update(idx, val);
            bitMax.update(idx, val);

            for (int j = i; j < N; j++) {
#ifndef _DEBUG
                if (bitSum.query(i, j) != sumSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMin.query(i, j) != minSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMax.query(i, j) != maxSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
#endif
                assert(bitSum.query(i, j) == sumSlow(in, i, j));
                assert(bitMin.query(i, j) == minSlow(in, i, j));
                assert(bitMax.query(i, j) == maxSlow(in, i, j));
            }
        }
    }
    {
        int MaxV = 10000;
        int N = 1024;
#ifdef _DEBUG
        //N = 100;
        N = 10;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % MaxV;

        auto bitSum = makeGeneralizedBIT(in, [](int a, int b) { return a + b; }, 0);
        auto bitMin = makeGeneralizedBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        auto bitMax = makeGeneralizedBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());

        for (int i = 0; i < N; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            int val = RandInt32::get() % MaxV;
            if (L > R)
                swap(L, R);

            updateSlow(in, L, R, val);
            bitSum.update(L, R, val);
            bitMin.update(L, R, val);
            bitMax.update(L, R, val);

            for (int j = i; j < N; j++) {
#ifndef _DEBUG
                if (bitSum.query(i, j) != sumSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMin.query(i, j) != minSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMax.query(i, j) != maxSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
#endif
                if (bitSum.query(i, j) != sumSlow(in, i, j))
                    cout << bitSum.query(i, j) << ", " << sumSlow(in, i, j) << endl;
                assert(bitSum.query(i, j) == sumSlow(in, i, j));
                assert(bitMin.query(i, j) == minSlow(in, i, j));
                assert(bitMax.query(i, j) == maxSlow(in, i, j));
            }
        }
    }
    {
        int MaxV = 10000;
        int N = 1024;
#ifdef _DEBUG
        //N = 100;
        N = 10;
#endif
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % MaxV;

        auto bitSum = makeGeneralizedBIT(in, [](int a, int b) { return a + b; }, 0);
        auto bitMin = makeGeneralizedBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        auto bitMax = makeGeneralizedBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());

        for (int i = 0; i < N; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            int val = RandInt32::get() % MaxV;
            if (L > R)
                swap(L, R);

            addSlow(in, L, R, val);
            bitSum.add(L, R, val);
            bitMin.add(L, R, val);
            bitMax.add(L, R, val);

            for (int j = i; j < N; j++) {
#ifndef _DEBUG
                if (bitSum.query(i, j) != sumSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMin.query(i, j) != minSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
                if (bitMax.query(i, j) != maxSlow(in, i, j))
                    cout << "Mismatch! line at " << __LINE__ << endl;;
#endif
                if (bitSum.query(i, j) != sumSlow(in, i, j))
                    cout << bitSum.query(i, j) << ", " << sumSlow(in, i, j) << endl;
                assert(bitSum.query(i, j) == sumSlow(in, i, j));
                assert(bitMin.query(i, j) == minSlow(in, i, j));
                assert(bitMax.query(i, j) == maxSlow(in, i, j));
            }
        }
    }
    cout << "--- lower bound & upper bound" << endl;
    {
        static const int T = 1000;
        int N = 10000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % (T * 10);

        auto bit = makeGeneralizedBIT(in, [](int a, int b) { return max(a, b); }, numeric_limits<int>::min());
        for (int i = N - 1; i >= 0; i--) {
            int idx = RandInt32::get() % N;
            int t = RandInt32::get() % (T * 10);

            bit.update(idx, t);
            in[idx] = t;

            int ans1 = lowerBound<int>(bit, i, N - 1, T);
            int gt1 = lowerBound(in, i, T);
            assert(gt1 == ans1);

            int ans2 = lowerBoundBackward(bit, 0, i, T);
            int gt2 = lowerBoundBackward(in, i, T);
            assert(gt2 == ans2);
        }
    }
    cout << "--- findNext() & findPrev()" << endl;
    {
        static const int T = 1000;
        int N = 50000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % (T * 10);

        auto bit = makeGeneralizedBIT(in, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
        for (int i = N - 1; i >= 0; i--) {
            int idx = RandInt32::get() % N;
            int t = RandInt32::get() % (T * 10);

            bit.update(idx, t);
            in[idx] = t;

            int ans1 = findNext<int>(bit, i, [](int x) {
                return x <= T;
            });
            int gt1 = findNext(in, i, T);
            assert(gt1 == ans1);

            int ans2 = findPrev<int>(bit, i, [](int x) {
                return x <= T;
            });
            int gt2 = findPrev(in, i, T);
            assert(gt2 == ans2);
        }
    }
    {
        cout << "-- Performance Test -----------------------" << endl;
        cout << "*** Segment tree vs Compact segment tree vs GeneralizedBIT" << endl;
        cout << "* range query..." << endl;
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
                auto bit = makeGeneralizedBIT(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        res += bit.query(it.first, it.second);
                    }
                }
                cout << "result = " << res << endl;
            }
            PROFILE_STOP(2);
        }
        cout << "* point update..." << endl;
        {
            int N = 500000;
#ifdef _DEBUG
            N = 1000;
#endif

            vector<int> T(N);
            for (int i = 0; i < N; i++)
                T[i] = RandInt32::get() % 65536;

            vector<pair<int,int>> Q;
            for (int i = 0; i < N; i++) {
                int a = RandInt32::get() % N;
                int b = RandInt32::get();
                Q.emplace_back(a, b);
            }

            PROFILE_START(0);
            {
                int res = 0;
                auto seg = makeSegmentTree(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        seg.update(it.first, it.second);
                    }
                }
                cout << "result = " << seg.query(0, N - 1) << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            {
                int res = 0;
                auto seg = makeCompactSegmentTree(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        seg.update(it.first, it.second);
                    }
                }
                cout << "result = " << seg.query(0, N - 1) << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            {
                int res = 0;
                auto bit = makeGeneralizedBIT(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        bit.update(it.first, it.second);
                    }
                }
                cout << "result = " << bit.query(0, N - 1) << endl;
            }
            PROFILE_STOP(2);
        }
        cout << "* range update..." << endl;
        {
            int N = 10000;
#ifdef _DEBUG
            N = 1000;
#endif

            vector<int> T(N);
            for (int i = 0; i < N; i++)
                T[i] = RandInt32::get() % 65536;

            vector<tuple<int, int, int>> Q;
            for (int i = 0; i < N; i++) {
                int a = RandInt32::get() % N;
                int b = RandInt32::get() % N;
                Q.emplace_back(min(a, b), max(a, b), RandInt32::get());
            }

            PROFILE_START(0);
            {
                int res = 0;
                auto seg = makeSegmentTree(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        seg.updateRange(get<0>(it), get<1>(it), get<2>(it));
                    }
                }
                cout << "result = " << seg.query(0, N - 1) << endl;
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            {
                int res = 0;
                auto seg = makeCompactSegmentTree(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        seg.updateRange(get<0>(it), get<1>(it), get<2>(it));
                    }
                }
                cout << "result = " << seg.query(0, N - 1) << endl;
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            {
                int res = 0;
                auto bit = makeGeneralizedBIT(T, [](int a, int b) { return min(a, b); }, numeric_limits<int>::max());
                for (int i = 0; i < 10; i++) {
                    for (auto& it : Q) {
                        bit.update(get<0>(it), get<1>(it), get<2>(it));
                    }
                }
                cout << "result = " << bit.query(0, N - 1) << endl;
            }
            PROFILE_STOP(2);
        }
    }

    cout << "OK!" << endl;
}
