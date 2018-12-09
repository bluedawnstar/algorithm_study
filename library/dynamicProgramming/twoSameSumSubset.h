#pragma once

// https://leetcode.com/problems/tallest-billboard/

struct TwoSameSumSubset {
    // O(N*maxSum)
    static int solveDP(const vector<int>& values, int maxSum) {
        int N = int(values.size());
        if (N < 2)
            return 0;

        int M = maxSum * 2 + 1;

        int res = 0;

        vector<vector<int>> dp(N, vector<int>(M, -1));
        dp[0][maxSum + values[0]] = values[0];
        dp[0][maxSum - values[0]] = 0;
        dp[0][maxSum] = 0;
        for (int i = 1; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (dp[i - 1][j] < 0)
                    continue;

                dp[i][j] = max(dp[i][j], dp[i - 1][j]);
                dp[i][j + values[i]] = max(dp[i][j + values[i]], dp[i - 1][j] + values[i]);
                dp[i][j - values[i]] = max(dp[i][j - values[i]], dp[i - 1][j]);
            }
            res = max(res, dp[i][maxSum]);
        }

        return res;
    }

    // O(N*maxSum), return (a max value, set 1 of the max value, set 2 of the max value)
    static tuple<int, int, int> solveDP(vector<int>& values, int maxSum) {
        int N = int(values.size());
        if (N < 2)
            return make_tuple(0, 0, 0);

        int M = maxSum * 2 + 1;

        tuple<int, int, int> res;

        vector<vector<tuple<int, int, int>>> dp(N, vector<tuple<int, int, int>>(M, make_tuple(-1, 0, 0)));
        dp[0][maxSum + values[0]] = make_tuple(values[0], 1, 0);
        dp[0][maxSum - values[0]] = make_tuple(0, 0, 1);
        dp[0][maxSum] = make_tuple(0, 0, 0);
        for (int i = 1; i < N; i++) {
            for (int j = 0; j < M; j++) {
                int x, set1, set2;
                tie(x, set1, set2) = dp[i - 1][j];

                if (x < 0)
                    continue;

                if (get<0>(dp[i][j]) < x)
                    dp[i][j] = dp[i - 1][j];
                if (get<0>(dp[i][j + values[i]]) < x + values[i])
                    dp[i][j + values[i]] = make_tuple(x + values[i], set1 + (1 << i), set2);
                if (get<0>(dp[i][j - values[i]]) < x)
                    dp[i][j - values[i]] = make_tuple(x, set1, set2 + (1 << i));
            }
            if (get<0>(res) < get<0>(dp[i][maxSum]))
                res = dp[i][maxSum];
        }

        return res;
    }

    // O(3^N)
    static int solveBitDP1(vector<int>& values) {
        int N = int(values.size());
        if (N < 2)
            return 0;

        int res = 0;

        int size = 1 << N;
        vector<int> dp(size);

        int j = -1, mask = 0;
        for (int i = 1; i < size; i++) {
            if ((i & (i - 1)) == 0)
                mask = (1 << ++j) - 1;
            dp[i] = dp[i & mask] + values[j];

            int msk = ~i & mask;
            if (dp[msk] < dp[i] || dp[i] <= res)
                continue;

            for (int s = msk; s; s = (s - 1) & msk) {
                if (dp[i] == dp[s]) {
                    res = max(res, dp[i]);
                    break;
                }
            }
        }

        return res;
    }

    // O(3^N)
    static int solveBitDP2(vector<int>& values) {
        int N = int(values.size());
        if (N < 2)
            return 0;

        int res = 0;

        int size = 1 << N;
        vector<int> dp(size);

        int j = -1, mask = 0;
        for (int i = 0; i < N; i++) {
            mask = 1 << i;
            for (int j = (mask << 1) - 1; j >= mask; j--) {
                dp[j] = dp[j & ~mask] + values[i];

                int msk = ~j & (mask - 1);
                if (dp[msk] < dp[j] || dp[j] <= res)
                    continue;

                for (int s = msk; s; s = (s - 1) & msk) {
                    if (dp[j] == dp[s]) {
                        res = max(res, dp[j]);
                        break;
                    }
                }
            }
        }

        return res;
    }
};
