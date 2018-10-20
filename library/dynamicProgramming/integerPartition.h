#pragma once

/*
1. inter n to [1..m] groups
    partition(4, 2) = 3
        1: 4
        2: 3 + 1
        3: 2 + 2

    partition(6, 3) = 7
        1: 6
        2: 5 + 1
        3: 4 + 2
        4: 4 + 1 + 1
        5: 3 + 3
        6: 3 + 2 + 1
        7: 2 + 2 + 2

                n
    S(n, m) =  SUM { S(n - i, m - 1) }
            i=ceil(n/m)

         S(0, m) = 1
         S(1, m) = 1
         S(n, 1) = 1

2. inter n to m groups
    partition(4, 2) = 2
        1: 3 + 1
        2: 2 + 2

    partition(6, 3) = 3
        1: 4 + 1 + 1
        2: 3 + 2 + 1
        3: 2 + 2 + 2

    F(n, m) = S(n - m, m)

        F(n, n + i) = 0   (i > 0)
        F(n, n) = 1
        F(n, 1) = 1
*/
struct IntegerPartition {
    // partition integer n to [1..m] groups
    static long long countWaysMaxM(int n, int m, int maxN = numeric_limits<int>::max()) {
        if (n <= 1 || m <= 1)
            return 1ll;

        maxN = min(maxN, n);

        long long res = 0ll;
        for (int i = (n + m - 1) / m; i <= maxN; i++)
            res += countWaysMaxM(n - i, m - 1, i);

        return res;
    }

    // partition integer n to m groups
    static long long countWays(int n, int m) {
        if (n < m || m <= 0)
            return 0ll;
        else if (m == 1 || n <= m + 1)
            return 1ll;

        return countWaysMaxM(n - m, m);
    }

    // partition integer n to m groups
    static long long countWaysDirect(int n, int m) {
        if (n < m || m <= 0)
            return 0ll;
        else if (m == 1 || n <= m + 1)
            return 1ll;

        int k = n / m;
        if (m == 2)
            return k;

        long long cnt = 0;
        for (; k--; n -= m)
            cnt += countWaysDirect(n - 1, m - 1);

        return cnt;
    }

    // partition integer n to m groups
    static long long countWaysDP(vector<vector<long long>>& dp, int n, int m) {
        if (n < m || m <= 0)
            return 0ll;
        else if (m == 1 || n <= m + 1)
            return 1ll;

        if (dp[n][m] > 0)
            return dp[n][m];

        int k = n / m;
        if (m == 2)
            return dp[n][m] = k;

        long long cnt = 0;
        for (; k--; n -= m)
            cnt += countWaysDirect(n - 1, m - 1);

        return dp[n][m] = cnt;
    }

    static long long countWaysDP(int n, int m) {
        vector<vector<long long>> dp(n + 1, vector<long long>(m + 1));
        return countWaysDP(dp, n, m);
    }
};

struct IntegerPartitionDP {
    int N;  // max integer value
    int M;  // max group count
    vector<vector<long long>> dp;

    IntegerPartitionDP() : N(0) {
    }

    IntegerPartitionDP(int N, int M) {
        build(N, M);
    }

    void build(int N, int M) {
        this->N = max(2, N);
        this->M = max(2, M);

        dp = vector<vector<long long>>(N + 1, vector<long long>(M + 1));

        dp[1][1] = 1ll;
        for (int i = 2; i <= N; i++) {
            dp[i][1] = 1ll;

            int maxJ = min(M, i - 2);
            for (int j = 2; j <= maxJ; j++) {
                int k = i / j;
                if (j == 2)
                    dp[i][j] = k;
                else {
                    long long cnt = 0ll;
                    for (int t = i; k--; t -= j)
                        cnt += dp[t - 1][j - 1];
                    dp[i][j] = cnt;
                }
            }

            if (i <= M)
                dp[i][i] = 1ll;
            if (i - 1 <= M)
                dp[i][i - 1] = 1ll;
        }
    }

    // partition integer n to m groups
    long long countWays(int n, int m) const {
        return dp[n][m];
    }
};
