#pragma once

/*
https://www.hackerrank.com/contests/101hack53/challenges/optimal-bus-stops/problem

1. Basic formula
    DP(C, i) = min{ DP(C - 1, j - 1) + cost(j, i) }, C = the number of cluster
             1<=j<=i

    cost(j, i) = SUM (X(k) - m(j,i))^2     , m(j,i) = {j + (j + 1) + (j + 2) + ... + i} / (i - j + 1) -- mean
               j<=k<=i
    cost(j, i) = SUM X(k)^2 - 2 * SUM X(k) * m(j,i) + SUM m(j,i)^2
               j<=k<=i         j<=k<=i              j<=k<=i
               = S2(i) - S2(j - 1) - 2 * m(j,i) * {S1(i) - S1(j - 1)} + m(j,i)^2 * (i - j + 1)
               = S2(i) - S2(j - 1) - 2 * {S1(i) - S1(j - 1)} / (i - j + 1) * {S1(i) - S1(j - 1)}
                                   + {S1(i) - S1(j - 1)}^2 / (i - j + 1)^2 * (i - j + 1)
               = S2(i) - S2(j - 1) - {S1(i) - S1(j - 1)}^2 / (i - j + 1)

    S2(i) = SUM X(k)^2,  S1(i) = SUM X(k)
          1<=k<=i              1<=k<=i

2. Optimization
    DP(C, i) <= DP(C, i + 1)
*/
// O(K*N*logN)
struct OptimalSquareDistance1D {
    const double INF = 1e15;

    int N;
    vector<vector<double>> dp;
    vector<long long> S, SS;

    // PRECONDITION: A must be sorted by ascending order
    OptimalSquareDistance1D(const vector<int>& A) : N((int)A.size()), S(N + 1), SS(N + 1) {
        for (int i = 1; i <= N; i++) {
            S[i] = S[i - 1] + A[i - 1];
            SS[i] = SS[i - 1] + 1ll * A[i - 1] * A[i - 1];
        }
    }

    double solve(int K) {
        dp = vector<vector<double>>(K + 1, vector<double>(N + 1, INF));
        dp[0][0] = 0.0;
        for (int i = 1; i <= K; i++)
            divideAndConquerDP(i, 1, N, 0, N);
        return dp[K][N];
    }

    // 1 <= i <= K, 1 <= left <= right <= N
    double cost(int i, int left, int right) const {
        return SS[right] - SS[left - 1] - double(S[right] - S[left - 1]) * (S[right] - S[left - 1]) / (right - left + 1);
    }

private:
    void divideAndConquerDP(int i, int jlo, int jhi, int klo, int khi) {
        if (jlo > jhi)
            return;

        int jmid = jlo + (jhi - jlo) / 2;
        int minK = klo;

        dp[i][jmid] = INF;
        for (int k = klo; k <= khi && k < jmid; k++) {
            double val = dp[i - 1][k] + cost(i, k + 1, jmid);
            if (val < dp[i][jmid]) {
                dp[i][jmid] = val;
                minK = k;
            }
        }

        divideAndConquerDP(i, jlo, jmid - 1, klo, minK);
        divideAndConquerDP(i, jmid + 1, jhi, minK, khi);
    }
};
