#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

#include "fenwickTreeMax.h"

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

void testFenwickTreeMax() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- FenwickTree for Max ---------------------------------" << endl;

    int N = 1000;
    int T = 10000;
    vector<int> in(N);

    for (int i = 0; i < N; i++)
        in[i] = RandInt32::get() % 65536;

    FenwickTreeMax<int> ft(in, 0);

    auto segTree = makeCompactSegmentTree(in, [](int a, int b) { return max(a, b); }, 0);
    for (int i = 0; i < 1000; i++) {
        int R = RandInt32::get() % N;
        assert(ft.get(R) == segTree.query(0, R));
    }

    vector<int> R(T);
    for (int i = 0; i < T; i++)
        R[i] = RandInt32::get() % N;

    PROFILE_START(0);
    for (int i = 0; i < T; i++) {
        if (ft.get(R[i]) < 0)
            cerr << "This statements is for preventing optimization" << endl;
    }
    PROFILE_STOP(0);

    PROFILE_START(1);
    for (int i = 0; i < T; i++) {
        if (segTree.query(0, R[i]) < 0)
            cerr << "This statements is for preventing optimization" << endl;
    }
    PROFILE_STOP(1);

    cout << "OK!" << endl;
}
