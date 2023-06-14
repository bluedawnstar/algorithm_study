#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "unitTimeSchedulerWithCooldownPeriod.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testUnitTimeSchedulerWithCooldownPeriod() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Unit-time Task Scheduler with Cooldown Period -------------------------" << endl;
    {
        vector<int> in{ 'A', 'A', 'A', 'B', 'B', 'B' };
        int gt = 8;
        auto ans = UnitTimeTaskSchedulerWithCooldownPeriod::leastInterval(in, 2);
        auto ans2 = UnitTimeTaskSchedulerWithCooldownPeriod::scheduleForLeastInterval(in, 2);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        if (!UnitTimeTaskSchedulerWithCooldownPeriod::isValidSchedule(ans2, 2)
            || int(ans2.size()) != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans2 << ", " << gt << endl;
        assert(ans == gt);
        assert(int(ans2.size()) == gt);
    }
    {
        vector<int> in{ 'A', 'A', 'A', 'A', 'A', 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
        int gt = 16;
        auto ans = UnitTimeTaskSchedulerWithCooldownPeriod::leastInterval(in, 2);
        auto ans2 = UnitTimeTaskSchedulerWithCooldownPeriod::scheduleForLeastInterval(in, 2);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        if (!UnitTimeTaskSchedulerWithCooldownPeriod::isValidSchedule(ans2, 2)
            || int(ans2.size()) != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans2 << ", " << gt << endl;
        assert(ans == gt);
        assert(int(ans2.size()) == gt);
    }
    {
        vector<int> in{ 'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C', 'D', 'D', 'E' };
        int gt = 12;
        auto ans = UnitTimeTaskSchedulerWithCooldownPeriod::leastInterval(in, 2);
        auto ans2 = UnitTimeTaskSchedulerWithCooldownPeriod::scheduleForLeastInterval(in, 2);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        if (!UnitTimeTaskSchedulerWithCooldownPeriod::isValidSchedule(ans2, 2)
            || int(ans2.size()) != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans2 << ", " << gt << endl;
        assert(ans == gt);
        assert(int(ans2.size()) == gt);
    }
    {
        vector<int> in{
            'A', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
        int gt = 31;
        auto ans = UnitTimeTaskSchedulerWithCooldownPeriod::leastInterval(in, 29);
        auto ans2 = UnitTimeTaskSchedulerWithCooldownPeriod::scheduleForLeastInterval(in, 29);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        if (!UnitTimeTaskSchedulerWithCooldownPeriod::isValidSchedule(ans2, 29)
            || int(ans2.size()) != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans2 << ", " << gt << endl;
        assert(ans == gt);
        assert(int(ans2.size()) == gt);
    }
    cout << "OK!" << endl;
    {
        vector<int> in{ 'A', 'A', 'A', 'B', 'B', 'B' };
        int gt = 4;
        auto ans = UnitTimeTaskSchedulerWithCooldownPeriod::calcRequiredIdlesWithFixedOrder(in, 1);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<int> in{ 'A', 'B', 'A', 'B', 'A', 'B' };
        int gt = 0;
        auto ans = UnitTimeTaskSchedulerWithCooldownPeriod::calcRequiredIdlesWithFixedOrder(in, 1);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        vector<int> in{ 'A', 'B', 'A', 'B', 'A', 'B' };
        int gt = 2;
        auto ans = UnitTimeTaskSchedulerWithCooldownPeriod::calcRequiredIdlesWithFixedOrder(in, 2);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
