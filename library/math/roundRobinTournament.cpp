#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

#include "roundRobinTournament.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

// https://www.codechef.com/problems/DFMTRX
static bool solveDFMTRX(int N, vector<vector<int>>& ans) {
    if (N == 1) {
        ans[0][0] = 1;
        return true;
    }
    if (N & 1)
        return false;

    for (int i = 0; i < N; i++)
        ans[i][i] = N * 2 - 1;

    vector<int> A(N);
    iota(A.begin(), A.end(), 0);
    for (int round = 1; round < N; round++) {
        auto games = RoundRobinTournament::scheduleEven(A, round);

        int x = round;
        int y = round + N - 1;
        for (auto& it : games) {
            ans[it.first][it.second] = x;
            ans[it.second][it.first] = y;
        }
    }

    return true;
}

static bool checkDFMTRX(int N, vector<vector<int>>& ans) {
    vector<vector<bool>> used(N * 2, vector<bool>(N));
    for (int i = 1; i < N; i++) {
        if (ans[i][i] != ans[i - 1][i - 1])
            return false;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                int x = ans[i][j];
                if (used[x][i] || used[x][j])
                    return false;
                used[x][i] = true;
                used[x][j] = true;
            }
        }
    }
    return true;
}

void testRoundRobinTournament() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Round-robin Tournament - Scheduling Algorithm -----------------------" << endl;
    {
        for (int N = 2; N <= 10; N += 2) {
            vector<vector<int>> ans(N, vector<int>(N));
            solveDFMTRX(N, ans);

            bool ok = checkDFMTRX(N, ans);
            if (!ok)
                cout << "Invalid answer!" << endl;
            assert(ok);
        }
    }

    cout << "OK!" << endl;
}
