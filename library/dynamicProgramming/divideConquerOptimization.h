#pragma once

// O(kn^2) => O(knlogn)
// 
// dp[i][j] = min { dp[i - 1][k] + C[k][j] }
//            k<j
// 
// <precondition> A[i][j] is the smallest k that gives optimal answer in dp[i][j]
//     A[i][j] <= A[i][j + 1]
//   or
//     C[a][c] + C[b][d] <= C[a][d] + C[b][c], a <= b <= c <= d (quadrangle inequality)
template <typename T, T INF = 2e9>
void divideAndConquerDP(vector<vector<T>>& dp, vector<vector<T>>& C, int i, int jlo, int jhi, int klo, int khi) {
    // calculate dp[i][jlo], D[i][jlo+1], ..., D[i][jhi]

    if (jlo > jhi)
        return;

    int jmid = jlo + (jhi - jlo) / 2;

    dp[i][jmid] = INF;
    int minK = -1;
    for (int k = klo; k <= khi && k < jmid; k++) {
        T v = dp[i - 1][k] + C[k][jmid];
        if (dp[i][jmid] > v) {
            dp[i][jmid] = v;
            minK = k;
        }
    }

    divideAndConquerDP<T, INF>(dp, C, i, jlo, jmid - 1, klo, minK);
    divideAndConquerDP<T, INF>(dp, C, i, jmid + 1, jhi, minK, khi);
}

// https://www.hackerrank.com/contests/ioi-2014-practice-contest-2/challenges/guardians-lunatics-ioi14
