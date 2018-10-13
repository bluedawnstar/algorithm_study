#include <vector>
#include <limits>
#include <functional>
#include <algorithm>

using namespace std;

#include "fenwickTreeEx.h"

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

#include "segmentTreeCompact.h"

static int lowerBoundSlow(vector<int>& v, int k) {
    int res = 0;
    for (int i = 0; i < int(v.size()); i++) {
        res += v[i];
        if (res >= k)
            return i;
    }
    return int(v.size());
}

void testFenwickTreeEx() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Extended FenwickTree ---------------------------------" << endl;
    {
        int N = 1000;
        int T = 10000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 65536;

        auto ft = makeFenwickTreeEx(in, [](int a, int b) { return max(a, b); }, 0);
        auto segTree = makeCompactSegmentTree(in, [](int a, int b) { return max(a, b); }, 0);
        for (int i = 0; i < 1000; i++) {
            int R = RandInt32::get() % N;
            assert(ft.query(R) == segTree.query(0, R));
        }
    }
    {
        int N = 1000;
        int T = 10000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 65536;

        auto ft = makeFenwickTreeEx(in, [](int a, int b) { return a + b; }, 0);
        auto segTree = makeCompactSegmentTree(in, [](int a, int b) { return a + b; }, 0);
        for (int i = 0; i < 1000; i++) {
            int R = RandInt32::get() % N;
            assert(ft.query(R) == segTree.query(0, R));

            int sum = ft.query(R) + RandInt32::get() % 65536;
            auto ans = ft.lowerBound([sum](int x) { return sum <= x; });
            auto gt = lowerBoundSlow(in, sum);
            if (ans != gt)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            //assert(ans == gt);
        }
    }
    {
        int N = 1000;
        int T = 10000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 65536;

        auto ft = makeFenwickTreeEx(in, [](int a, int b) { return max(a, b); }, 0);
        auto segTree = makeCompactSegmentTree(in, [](int a, int b) { return max(a, b); }, 0);

        vector<int> R(T);
        for (int i = 0; i < T; i++)
            R[i] = RandInt32::get() % N;

        PROFILE_START(0);
        for (int i = 0; i < T; i++) {
            if (ft.query(R[i]) < 0)
                cerr << "This statements is for preventing optimization" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < T; i++) {
            if (segTree.query(0, R[i]) < 0)
                cerr << "This statements is for preventing optimization" << endl;
        }
        PROFILE_STOP(1);
    }
    cout << "OK!" << endl;
}
