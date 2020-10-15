#pragma once

// http://codeforces.com/blog/entry/8219
// 
// O(kn^2) => O(knlogn)
// 
// dp[i][j] = min { dp[i - 1][k] + C[k + 1][j] }
//            k<j
// 
// <precondition> A[i][j] is the smallest k that gives optimal answer in dp[i][j]
//     A[i][j] <= A[i][j + 1]
//   or
//     C[a][c] + C[b][d] <= C[a][d] + C[b][c], a <= b <= c <= d (quadrangle inequality)
// 
// https://www.hackerrank.com/contests/ioi-2014-practice-contest-2/challenges/guardians-lunatics-ioi14
// https://www.hackerrank.com/contests/101hack53/challenges/optimal-bus-stops/problem
// 
//
/*
    - T     : cost type
    - CostF : cost function
        => T CostF(int i, int left, int right); 1 <= i <= K, 1 <= left <= right <= N, inclusive
*/
template <typename T = double, typename CostF = function<T(int, int, int)>>
struct DivideAndConquerOptimizer {
    const T INF;

    int N;
    vector<vector<T>> dp;
    CostF costF;

    // PRECONDITION: A must be sorted by ascending order
    DivideAndConquerOptimizer(int _N, T _INF, CostF _costF)
        : N(_N), INF(_INF), costF(_costF) {
    }

    T solve(int K) {
        dp = vector<vector<T>>(K + 1, vector<T>(N + 1, INF));
        dp[0][0] = T(0);
        for (int i = 1; i <= K; i++)
            divideAndConquerDP(i, 1, N, 0, N);
        return dp[K][N];
    }

private:
    void divideAndConquerDP(int i, int jlo, int jhi, int klo, int khi) {
        if (jlo > jhi)
            return;

        int jmid = jlo + (jhi - jlo) / 2;
        int minK = klo;

        dp[i][jmid] = INF;
        for (int k = klo; k <= khi && k < jmid; k++) {
            T val = dp[i - 1][k] + costF(i, k + 1, jmid);
            if (val < dp[i][jmid]) {
                dp[i][jmid] = val;
                minK = k;
            }
        }

        divideAndConquerDP(i, jlo, jmid - 1, klo, minK);
        divideAndConquerDP(i, jmid + 1, jhi, minK, khi);
    }
};

template <typename T, typename CostF>
DivideAndConquerOptimizer<T, CostF> makeDivideAndConquerOptimizer(int N, T INF, CostF costFunc) {
    return DivideAndConquerOptimizer<T, CostF>(N, INF, costFunc);
}
