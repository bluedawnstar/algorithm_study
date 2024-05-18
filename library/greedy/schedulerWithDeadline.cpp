#include <limits>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "schedulerWithDeadline.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSchedulerWithDeadline() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Task Scheduler with Deadline ------------------------------" << endl;
    {
        vector<pair<int, int>> in{ { 100, 200 }, { 200, 1300 }, { 1000, 1250 }, { 2000, 3200 } };
        int gt = 3;
        auto ans = TaskSchedulerWithDeadline::scheduleForMaxWorks(in);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<pair<int, int>> in{ { 1, 2 } };
        int gt = 1;
        auto ans = TaskSchedulerWithDeadline::scheduleForMaxWorks(in);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<pair<int, int>> in{ { 3, 2 }, { 4, 3 } };
        int gt = 0;
        auto ans = TaskSchedulerWithDeadline::scheduleForMaxWorks(in);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
