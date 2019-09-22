#include <tuple>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "minimumKnightMoves.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

// https://www.geeksforgeeks.org/minimum-steps-reach-target-knight/
// https://www.geeksforgeeks.org/minimum-steps-reach-target-knight-set-2/

// https://leetcode.com/problems/minimum-knight-moves/

void testMinimumKnightMoves() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Minimum Knight Moves --------------------------" << endl;
    {
        auto ans1 = MinimumKnightMove::calculateMinStepsBFS(2, 1);
        auto ans2 = MinimumKnightMove::calculateMinStepsDFS(2, 1);
        auto ans3 = MinimumKnightMove::calculateMinSteps(2, 1);
        int gt = 1;
        if (ans1 != gt || ans2 != gt || ans3 != gt)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ans1 == gt && ans2 == gt && ans3 == gt);
    }
    {
        auto ans1 = MinimumKnightMove::calculateMinStepsBFS(5, 5);
        auto ans2 = MinimumKnightMove::calculateMinStepsDFS(5, 5);
        auto ans3 = MinimumKnightMove::calculateMinSteps(5, 5);
        int gt = 4;
        if (ans1 != gt || ans2 != gt || ans3 != gt)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ans1 == gt && ans2 == gt && ans3 == gt);
    }
    cout << "OK!" << endl;
}
