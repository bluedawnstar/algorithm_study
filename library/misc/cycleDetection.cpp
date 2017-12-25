#include <functional>

using namespace std;

#include "cycleDetection.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testCycleDetection() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Cycle Detection ------------------------" << endl;
    {
        int N = 10;
        vector<int> in(N);
        iota(in.begin(), in.begin() + N, 1);
        
        auto ans1 = CycleDetection<int>::floyd([N, &in](int x) { return x >= N ? N : in[x]; }, 0);
        auto ans2 = CycleDetection<int>::brent([N, &in](int x) { return x >= N ? N : in[x]; }, 0);
        assert(ans1 == make_pair(10, 1));
        assert(ans2 == make_pair(10, 1));
    }
    {
        int N = 10;
        vector<int> in(N);
        iota(in.begin(), in.begin() + N, 1);
        in.back() = 6;

        auto ans1 = CycleDetection<int>::floyd([N, &in](int x) { return x >= N ? N : in[x]; }, 0);
        auto ans2 = CycleDetection<int>::brent([N, &in](int x) { return x >= N ? N : in[x]; }, 0);
        assert(ans1 == make_pair(6, 4));
        assert(ans2 == make_pair(6, 4));
    }

    cout << "OK!" << endl;
}
