#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "gridPathCounterWithObstacles.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGridPathCounterWithObstacles() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Path Counter in a Grid With Obstacles -------------------------------" << endl;
    {
        const int T = 100;
        const int W = 100;
        const int H = 50;
        const int K = 100;

        GridPathCounterWithObstaclesMod<> solver(W, H);

        for (int t = 0; t < T; t++) {
            vector<pair<int, int>> P;
            for (int i = 0; i < K; i++) {
                int x = RandInt32::get() % W;
                int y = RandInt32::get() % H;
                P.emplace_back(x, y);
            }
            sort(P.begin(), P.end());
            P.resize(unique(P.begin(), P.end()) - P.begin());

            auto ans1 = solver.countValidPathNaive(W, H, P);
            auto ans2 = solver.countValidPath(W, H, P);
            if (ans1 != ans2) {
                cout << "Mismatched : " << ans1 << ", " << ans2 << endl;
            }
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
