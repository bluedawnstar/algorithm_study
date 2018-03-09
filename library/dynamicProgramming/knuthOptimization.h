#pragma once


// O(n^3) => O(n^2)
// 
// dp[i][j] = min { dp[i][k] + dp[k][j] } + C[i][j]
//           i<k<j
// 
// One of both preconditions must be satisfied
// 
// <precondition>
//   A[i][j - 1] <= A[i][j] <= A[i + 1][j]
//     : A[i][j] is the smallest k that gives optimal answer in dp[i][j]
// 
// <precondition>
//     C[a][c] + C[b][d] <= C[a][d] + C[b][c], a <= b <= c <= d (quadrangle inequality)
//   and
//     C[b][c] <= C[a][d], a <= b <= c <= d (monotonicity)
//
// https://www.acmicpc.net/problem/11066
//
/*
    - T     : cost type
    - CostF : cost function in [i, j)
        => T CostF(int i, int j); 0 <= i < j <= N, exclusive
*/
template <typename T = double, typename CostF = function<T(int, int)>>
struct KnuthOptimizer {
    const T INF;

    int N;
    vector<vector<T>> dp;
    vector<vector<int>> mid;
    CostF costF;

    KnuthOptimizer(int _N, T _INF, CostF _costF)
        : N(_N), INF(_INF), costF(_costF) {
    }

    T solve() {
        dp = vector<vector<T>>(N + 1, vector<T>(N + 1, INF));
        mid = vector<vector<int>>(N + 1, vector<int>(N + 1));

        for (int d = 0; d <= N; d++) {
            for (int i = 0; i + d <= N; i++) {
                int j = i + d;
                if (d < 2) {
                    dp[i][j] = 0;
                    mid[i][j] = i;
                    continue;
                }

                int minK = mid[i][j - 1];
                int maxK = mid[i + 1][j];

                dp[i][j] = INF;
                for (int k = minK; k <= maxK; k++) {
                    T val = dp[i][k] + dp[k][j];
                    if (val < dp[i][j]) {
                        dp[i][j] = val;
                        mid[i][j] = k;
                    }
                }
                dp[i][j] += costF(i, j);
            }
        }
        return dp[0][N];
    }
};

template <typename T, typename CostF>
KnuthOptimizer<T, CostF> makeKnuthOptimizer(int N, T INF, CostF costFunc) {
    return KnuthOptimizer<T, CostF>(N, INF, costFunc);
}
