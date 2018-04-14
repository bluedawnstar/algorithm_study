#include <vector>

using namespace std;

#include "sqrtDecomposition.h"

// related question: https://www.hackerrank.com/contests/101hack49/challenges/bigger-arrays

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include <algorithm>
#include "segmentTree.h"
#include "segmentTreeLazy.h"

void testSqrtDecomposition() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sqrt Decomposition ------------------------" << endl;
    {
        vector<int> in{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        auto segTree = makeSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTreeLazy = makeSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
        SqrtDecompositionLazy<int> sqrtDecom(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });

        for (int i = 0; i < 10000; i++) {
            if (RandInt32::get() % 2) {
                int L = RandInt32::get() % (int)in.size();
                int R = RandInt32::get() % (int)in.size();
                if (L > R)
                    swap(L, R);
                assert(segTree.query(L, R) == segTreeLazy.query(L, R));
                assert(segTree.query(L, R) == sqrtDecom.query(L, R));
            } else {
                int L = RandInt32::get() % (int)in.size();
                int R = RandInt32::get() % (int)in.size();
                int x = RandInt32::get() % 100;
                if (L > R)
                    swap(L, R);

                int a = segTree.updateRange(L, R, x);
                int b = segTreeLazy.updateRange(L, R, x);
                sqrtDecom.update(L, R, x);
                assert(a == b);
            }
        }
    }
    cout << "OK!" << endl;

    cout << "*** Speed Test ***" << endl;
    {
        int T = 100000;
        int N = 1000000;
#ifdef _DEBUG
        T = 1000;
        N = 10000;
#endif
        vector<int> in(N);

        auto segTree = makeSegmentTree(in, [](int a, int b) { return a + b; });
        auto segTreeLazy = makeSegmentTreeLazy(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });
        SqrtDecompositionLazy<int> sqrtDecom(in, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; });

        vector<tuple<int, int, int, int>> Q;
        for (int i = 0; i < T; i++) {
            if (RandInt32::get() % 2) {
                int L = RandInt32::get() % (int)in.size();
                int R = RandInt32::get() % (int)in.size();
                if (L > R)
                    swap(L, R);
                Q.emplace_back(1, L, R, 0);
            } else {
                int L = RandInt32::get() % (int)in.size();
                int R = RandInt32::get() % (int)in.size();
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
                int c = sqrtDecom.query(L, R);
                if (a != b)
                    cout << "It'll Never be shown!" << endl;
                if (a != c)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                segTree.updateRange(L, R, x);
                segTreeLazy.updateRange(L, R, x);
                sqrtDecom.update(L, R, x);
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

        PROFILE_START(2);
        for (auto& q : Q) {
            if (get<0>(q)) {
                int L = get<1>(q);
                int R = get<2>(q);
                if (sqrtDecom.query(L, R) == INT_MAX)
                    cout << "It'll Never be shown!" << endl;
            } else {
                int L = get<1>(q);
                int R = get<2>(q);
                int x = get<3>(q);
                sqrtDecom.update(L, R, x);
            }
        }
        PROFILE_STOP(2);
    }
}
