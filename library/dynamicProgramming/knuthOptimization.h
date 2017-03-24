#pragma once


// O(n^3) => O(n^2)
// 
// dp[i][j] = min { dp[i][k] + dp[k][j] } + C[i][j]
//            i<k<j
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
template <typename T, T INF = 2e9>
void knuthDP(vector<vector<T>>& dp, vector<vector<T>>& C) {
    // ...
}

// ...
