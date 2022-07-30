#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "circularMove.h"
#include "circularMoveDP.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

//...

void testCircularMove() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Circular Movement -----------------------" << endl;
    {
        const int N = 7080;
        const int M = 3519;

        pair<int, int> srcXY[]{
            { 1051, 1415 },
            { 3678,  993 },
            { 6758, 1141 },
            { 2045, 1851 },
            {  556, 3308 }
        };
        pair<int, int> dstXY[]{
            { 4642, 1782 },
            { 2831,  434 },
            { 5189, 2563 },
            { 5917, 3338 },
            { 6994,  247 }
        };

        int gt[]{
            5879808,
            3919872,
            8819712,
            7839744,
            8819712
        };

        CircularMove solver(10000);
        CircularMoveDP solverN(N), solverM(M);

        for (int i = 0; i < 5; i++) {
            int ans = solver.countJumpSteps(srcXY[i].first, dstXY[i].first, N)
                    * solver.countJumpSteps(srcXY[i].second, dstXY[i].second, M);
            assert(ans == gt[i]);
            if (ans != gt[i])
                cout << "Mismatched at " << __LINE__ << " : [" << i << "] " << ans << ", " << gt << endl;

            //---

            ans = solverN.countJumpSteps(srcXY[i].first, dstXY[i].first)
                * solverM.countJumpSteps(srcXY[i].second, dstXY[i].second);
            assert(ans == gt[i]);
            if (ans != gt[i])
                cout << "Mismatched at " << __LINE__ << " : [" << i << "] " << ans << ", " << gt << endl;
        }
    }

    cout << "OK!" << endl;
}
