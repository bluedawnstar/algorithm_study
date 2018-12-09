#include <cmath>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

#include "integerPartition.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testIntegerPartition() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Integer Partition -----------------------------------" << endl;
    {
        // gt[n][m]
        vector<vector<long long>> gt = {
            { 1, 1, 1,  1,  1,  1,  1,  1,  1,  1, },
            { 1, 1, 1,  1,  1,  1,  1,  1,  1,  1, },
            { 1, 1, 2,  2,  2,  2,  2,  2,  2,  2, },
            { 1, 1, 2,  3,  3,  3,  3,  3,  3,  3, },
            { 1, 1, 3,  4,  5,  5,  5,  5,  5,  5, },
            { 1, 1, 3,  5,  6,  7,  7,  7,  7,  7, },
            { 1, 1, 4,  7,  9, 10, 11, 11, 11, 11, },
            { 1, 1, 4,  8, 11, 13, 14, 15, 15, 15, },
            { 1, 1, 5, 10, 15, 18, 20, 21, 22, 22, },
            { 1, 1, 5, 12, 18, 23, 26, 28, 29, 30, },
        };

        int N = int(gt[0].size());
        int M = int(gt.size());

        IntegerPartitionDP ipDP(N, M);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                long long ans1 = IntegerPartition::countWaysMaxM(i, j);
                if (ans1 != gt[i][j]) {
                    cout << "Mismatched at [" << i << "][" << j << "] = " << ans1 << ", " << gt[i][j] << endl;
                    //ans1 = IntegerPartition::countWaysMaxM(i, j);
                }
                assert(ans1 == gt[i][j]);
            }
        }

        assert(IntegerPartition::countWaysDP(150, 23) == 1901740434ll);
    }
    {
        int N = 20;
        int M = 20;

        IntegerPartitionDP ipDP(N, M);

        for (int i = 1; i < N; i++) {
            for (int j = 1; j < M; j++) {
                long long ans1 = IntegerPartition::countWays(i, j);
                long long ans2 = ipDP.countWays(i, j);
                if (ans1 != ans2)
                    cout << "Mismatched at dp[" << i << "][" << j << "] = " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
    {
        int N = 20;
        int M = 20;

        for (int i = 0; i < N; i++) {
            cout << "{ ";
            for (int j = 0; j < M; j++) {
                long long ans = IntegerPartition::countWays(i, j);
                cout << setw(2) << ans << ", ";
            }
            cout << " }" << endl;
        }
    }
    cout << "---------------------" << endl;
    {
        int N = 20;
        int M = 20;

        for (int j = 0; j < M; j++) {
            cout << "{ ";
            for (int i = j; i < N; i++) {
                long long ans = IntegerPartition::countWays(i, j);
                cout << setw(2) << ans << ", ";
            }
            cout << " }" << endl;
        }
    }


    cout << "OK!" << endl;
}
