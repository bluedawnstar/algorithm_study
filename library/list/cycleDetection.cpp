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
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Cycle Detection ------------------------" << endl;
    {
        int N = 10;
        vector<CycleDetector::Node> in(N);
        for (int i = 1; i < N; i++)
            in[i - 1].next = &in[i];
        in.back().next = nullptr;

        auto ans = CycleDetector::findCycle(&in[0]);
        assert(ans.first == nullptr && ans.second == -1);
    }
    {
        int N = 10;
        vector<CycleDetector::Node> in(N);
        for (int i = 1; i < N; i++)
            in[i - 1].next = &in[i];
        in.back().next = &in[6];

        auto ans = CycleDetector::findCycle(&in[0]);
        assert(ans == make_pair(&in[6], 6));
    }

    cout << "OK!" << endl;
}
