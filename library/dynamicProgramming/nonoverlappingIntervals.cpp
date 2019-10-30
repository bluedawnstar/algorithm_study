#include <tuple>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

#include "nonoverlappingIntervals.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://leetcode.com/problems/maximum-profit-in-job-scheduling/

void testMaxWeightedNonoverlappingIntervals() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Maximum Weighted Non-overlapping Intervals ---------------------" << endl;
    {
        vector<tuple<int, int, int>> in{ { 1, 3, 50 }, { 2, 4, 10 }, { 3, 5, 40 }, { 3, 6, 70 } };
        int ans = maximumWeightedNonoverlappingIntervals(in);
        int gt = 120;
        if (ans != gt)
            cout << "invalid answer : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<tuple<int, int, int>> in{ { 1, 3, 20 }, { 2, 5, 20 }, { 3, 10, 100 }, { 4, 6, 70 }, { 6, 9, 60 } };
        int ans = maximumWeightedNonoverlappingIntervals(in);
        int gt = 150;
        if (ans != gt)
            cout << "invalid answer : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<tuple<int, int, int>> in{ { 1, 2, 5 }, { 1, 3, 6 }, { 1, 4, 4 } };
        int ans = maximumWeightedNonoverlappingIntervals(in);
        int gt = 6;
        if (ans != gt)
            cout << "invalid answer : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
