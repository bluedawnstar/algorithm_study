#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "gridMultistepPathCounterWithObstacles.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGridMultistepPathCounterWithObstacles() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Multistep Path Counter in a Grid With Obstacles --------------------" << endl;
    {
        const int N = 3;
        const int M = 3;
        const vector<vector<bool>> grid{
            { false, false, false },
            { false,  true, false },
            { false, false, false }
        };
        const int gt = 8;

        int ans = GridMultistepPathCounterWithObstaclesMod<int>::count(grid, N, M);
        assert(ans == gt);
    }
    {
        const int N = 3;
        const int M = 4;
        const vector<vector<bool>> grid{
            { false, false, false, false },
            { false, false,  true, false },
            { false, false, false, false }
        };
        const int gt = 22;

        int ans = GridMultistepPathCounterWithObstaclesMod<int>::count(grid, N, M);
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
