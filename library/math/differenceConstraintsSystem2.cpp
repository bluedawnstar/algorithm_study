#include <vector>
#include <algorithm>

using namespace std;

#include "differenceConstraintsSystem2.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

// https://www.codechef.com/problems/DGMATRIX
// https://www.codechef.com/viewsolution/40192531

static vector<vector<int>> digitMatrixSolver2(const vector<vector<int>>& A, int N) {
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
        vector<int> varMin(N * 2 + 1, 0);
        vector<int> varMax(N * 2 + 1, 0);
        for (int i = 1; i <= N; i++) {
            if (i & 1) {
                varMin[i] = 0;
                varMax[i] = 9;          // X[i]

                varMin[N + i] = -9;
                varMax[N + i] = 0;      // -Y[i]
            } else {
                varMin[i] = -9;
                varMax[i] = 0;          // -X[i]

                varMin[N + i] = 0;
                varMax[N + i] = 9;      // Y[i]
            }
        }

        DifferenceConstraintsSystem2 solver(N * 2 + 1, varMin, varMax);

        // add edges between X[i] and Y[j]
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                int lo = 0 - B[i][j];
                int hi = 9 - B[i][j];
                if ((i & 1) == (j & 1)) {
                    solver.add(j, N + i, lo, hi);   // minW <= Y[i] - X[j] <= maxW
                } else {
                    solver.add(N + i, j, lo, hi);   // minW <= X[i] - Y[j] <= maxW
                }
            }
        }

        auto values = solver.solve();
        if (!values.empty()) {
            vector<vector<int>> ans(N + 1, vector<int>(N + 1));
            ans[0][0] = o;
            for (int i = 1; i <= N; i++) {
                if (i & 1) {
                    ans[0][i] = values[i];
                    ans[i][0] = -values[N + i];
                } else {
                    ans[0][i] = -values[i];
                    ans[i][0] = values[N + i];
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

    return vector<vector<int>>{};
}

void testDifferenceConstraintsSystem2() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- System of Difference Constraints (2) ---------------------" << endl;
    {
        vector<vector<int>> A{
            { 12, 16 },
            { 24, 28 }
        };
        auto ans = digitMatrixSolver2(A, 2);

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
