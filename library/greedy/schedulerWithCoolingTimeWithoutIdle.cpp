#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "schedulerWithCoolingTimeWithoutIdle.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static vector<int> generateTaskCount(int N, int maxCnt) {
    vector<int> res(N);
    for (int i = 0; i < N; i++)
        res[i] = RandInt32::get() % maxCnt + 1;
    return res;
}


void testSchedulerWithCoolingTimeWithoutIdle() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Task Scheduler with Cooling Time without Idle -------------------------" << endl;
    //--- test cases
    {
        /*
          { A: 5, B: 5, C: 3, D: 3, E: 3 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 19
              C    C    C    D    D
               D    E    E    E
        */
        vector<int> in{ 5, 5, 3, 3, 3 };
        int gt = 19;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 3, D: 3, E: 2 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 18
              C    C    C    D    .
               D    D    E    E
        */
        vector<int> in{ 5, 5, 3, 3, 2 };
        int gt = 18;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 2, D: 2, E: 2 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 14
              C    C    D    .    .
               D    E    E
        */
        vector<int> in{ 5, 5, 2, 2, 2 };
        int gt = 14;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 2, D: 2, E: 1 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 11
              C    C    D    .    .
               D    E
        */
        vector<int> in{ 5, 5, 2, 2, 1 };
        int gt = 11;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 3, D: 1, E: 1 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 11
              C    C    C    .    .
               D    E
        */
        vector<int> in{ 5, 5, 3, 1, 1 };
        int gt = 11;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 3, D: 1, E: 1, F:1, G:1, H:1, I:1 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 19
              C    C    C    D    E
               F    G    H    I
        */
        vector<int> in{ 5, 5, 3, 1, 1, 1, 1, 1, 1 };
        int gt = 19;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 3, D: 1, E: 1, F:1, G:1, H:1 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 18
              C    C    C    D
               E    F    G    H
        */
        vector<int> in{ 5, 5, 3, 1, 1, 1, 1, 1 };
        int gt = 18;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 3, D: 1, E: 1, F:1, G:1 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 15
              C    C    C    D
               E    F    G
        */
        vector<int> in{ 5, 5, 3, 1, 1, 1, 1 };
        int gt = 15;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 3, D: 1, E: 1, F:1, G:1 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 25
              C    C    C    D    D 
               E    F    G    H    I
                J    K    L    M    N
        */
        vector<int> in{ 5, 5, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        int gt = 25;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        /*
          { A: 5, B: 5, C: 3, D: 1, E: 1, F:1, G:1 }, coolingTime=3

            A    A    A    A    A
             B    B    B    B    B     => 24
              C    C    C    D    D
               E    F    G    H    I
                J    K    L    M
        */
        vector<int> in{ 5, 5, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        int gt = 24;
        auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, 3);
        if (ans != gt)
            cout << "invalid answer at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    //--- random test
    {
        const int T = 1000;
        const int MAXN = 5;
        const int MAXCNT = 5;
        const int MAXCOOLTIME = 5;
        for (int t = 0; t < T; t++) {
            int N = RandInt32::get() % MAXN + 1;
            int C = RandInt32::get() % MAXCOOLTIME + 1;
            vector<int> in = generateTaskCount(N, MAXCNT);

            auto ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, C);

            auto seq = TaskSchedulerWithCoolingTimeWithoutIdle::generateTaskSequence(in, C);
            bool valid = TaskSchedulerWithCoolingTimeWithoutIdle::isValidTaskSequence(seq, C);
            if (!valid) {
                cout << "[" << t << "] Invalid at " << __LINE__ << " : seq = " << seq << endl;
            }
            auto gt = int(seq.size());
            if (ans != gt) {
                cout << "[" << t << "] Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
                cout << "  ==> " << N << ", C = " << C << ", in = " << in << endl;

                ans = TaskSchedulerWithCoolingTimeWithoutIdle::countPossibleTasks(in, C);
                seq = TaskSchedulerWithCoolingTimeWithoutIdle::generateTaskSequence(in, C);
            }
            assert(valid);
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
