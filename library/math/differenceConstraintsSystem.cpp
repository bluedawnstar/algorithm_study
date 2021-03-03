#include <vector>
#include <algorithm>

using namespace std;

#include "differenceConstraintsSystem.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

// https://www.codechef.com/problems/DGMATRIX
// https://discuss.codechef.com/t/dgmatrix-editorial/82435

static vector<vector<int>> digitMatrixSolver(const vector<vector<int>>& A, int N) {
    for (int o = 0; o < 10; o++) {
        vector<vector<int>> B(N + 1, vector<int>(N + 1));
        B[0][0] = o;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                B[i + 1][j + 1] = A[i][j] - B[i][j] - B[i][j + 1] - B[i + 1][j];
            }
        }

        /*
            B = | 0       x1       x2       x3   ... xn |
                | y1   -y1-x1   +y1-x2   -y1-x3  ... .  |
                | y2   -y2+x1   +y2+x2   -y2+x3  ... .  |
                | y3   -y3-x1   +y3-x2   -y3-x3  ... .  |
                | ...     .        .        .     .  .  |
                | yn      .        .        .     .  .  |

            ==>

                  0         1                  2
                +----+------------------+------------------
              0 | 0  |      x1          |      x2
                +----+------------------+------------------
              1 | y1 |    -y1 - x1      |    +y1 - x2
                |    | => (-Y) - (+X)   | => (-X) - (-Y)       -
                +----+------------------+------------------
              2 | y2 |    -y2 + x1      |    +y2 + x2
                |    | => (+X) - (+Y)   | => (+Y) - (-X)       +

                             +                  -

            ==>

            variables =
                       +    -    +
                |  x   1    2    3  ... N |
              - | N+1                     |
              + | N+2                     |
              - | N+3                     |
                | ...                     |
                | N+N                     |
        */
        DifferenceConstraintsSystem constraintsGraph(N * 2 + 1);
        // step #1 : add edges from/to O
        for (int i = 1; i <= N; i++) {
            if (i & 1) {
                constraintsGraph.add(0, i, 0, 9);       //   0 <=  X[i] <= 9
                constraintsGraph.add(N + i, 0, 0, 9);   //  -9 <= -Y[i] <= 0
            } else {
                constraintsGraph.add(i, 0, 0, 9);       //  -9 <= -X[i] <= 0
                constraintsGraph.add(0, N + i, 0, 9);   //   0 <=  Y[i] <= 9
            }
        }

        // step #2 : add edges between X[i] and Y[j]
        bool bad = false;
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                int minW = 0 - B[i][j];
                int maxW = 9 - B[i][j];
                if (minW > maxW) {
                    bad = true;
                    goto lbBadCase;
                }

                if ((i & 1) == (j & 1)) {
                    constraintsGraph.add(j, N + i, minW, maxW); // minW <= Y[i] - X[j] <= maxW
                } else {
                    constraintsGraph.add(N + i, j, minW, maxW); // minW <= X[i] - Y[j] <= maxW
                }
            }
        }
lbBadCase:

        if (!bad) {
            auto dist = constraintsGraph.solve(0);
            if (!dist.empty()) {
                vector<vector<int>> ans(N + 1, vector<int>(N + 1));
                ans[0][0] = o;
                for (int i = 1; i <= N; i++) {
                    if (i & 1) {
                        ans[0][i] = dist[i];
                        ans[i][0] = -dist[N + i];
                    } else {
                        ans[0][i] = -dist[i];
                        ans[i][0] = dist[N + i];
                    }
                }
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N; j++) {
                        ans[i + 1][j + 1] = A[i][j] - ans[i][j] - ans[i][j + 1] - ans[i + 1][j];
                    }
                }
                return ans;
            }
        }
    }

    return vector<vector<int>>{};
}

void testDifferenceConstraintsSystem() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- System of Difference Constraints -----------------------" << endl;
    {
        vector<vector<int>> A{
            { 12, 16 },
            { 24, 28 }
        };
        auto ans = digitMatrixSolver(A, 2);
        
        bool ok = !ans.empty();
        if (ok) {
            for (auto& v : ans) {
                for (auto x : v) {
                    if (x < 0 || x > 9)
                        ok = false;
                }
            }
        }
        assert(ok);
    }

    cout << "OK!" << endl;
}
