#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "pirateGame.h"
#include "pirateGameExtended.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testPirateGame() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Pirate Game --------------------------" << endl;
    {
        long long M = 100;
        for (int i = 1; i <= 202; i++) {
            auto ans = PirateGame::solve(i, M);

            int half = (i + 1) / 2;
            long long gt = M - (half - 1);
            if (ans != gt)
                cout << "Wrong anser : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
        assert(PirateGame::solve(203, M) == -1ll);
        assert(PirateGame::solve(204, M) == 0ll);
        assert(PirateGame::solve(205, M) == -1ll);
        assert(PirateGame::solve(206, M) == -1ll);
        assert(PirateGame::solve(207, M) == -1ll);
        assert(PirateGame::solve(208, M) == 0ll);
    }
    {
        for (int K = 2; K <= 10; K++) {
            long long MAXN = 1000000000000000000ll;
            long long MAXM = 1000000000000000000ll;
            int Q = 1000;

            vector<pair<long long, long long>> qry;
            for (int i = 0; i < Q; i++)
                qry.emplace_back(RandInt64::get() % MAXN + 1, RandInt64::get() % MAXM + 1);

            // offline solution
            auto offlineAns = ExtendedPirateGameOffline::solve(qry, K);

            // online solution
            ExtendedPirateGame solver;
            solver.build(MAXN, K);
            for (int i = 0; i < Q; i++) {
                long long N, M;
                tie(N, M) = qry[i];

                auto ans = solver.solve(N, M);
                if (offlineAns[i] != ans)
                    cout << "ERROR! answers are mismatched!" << endl;
                assert(offlineAns[i] == ans);
            }
        }
    }

    cout << "OK!" << endl;
}
