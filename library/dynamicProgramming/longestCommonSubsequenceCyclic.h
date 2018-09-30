#pragma once

// Solving Cyclic Longest Common Subsequence in Quadratic Time
// https://arxiv.org/pdf/1208.0396v3.pdf
// https://github.com/zimpha/algorithmic-library/blob/master/StringUtility/cyclic-longest-common-string.cc

struct CyclicLCS {
    int N, M;
    vector<vector<int>> dp;
    vector<vector<int>> from;

    // O(N*M)
    int solve(const string& s, const string& t) {
        N = int(s.length());
        M = int(t.length());

        dp = vector<vector<int>>(N * 2 + 1, vector<int>(M + 1));
        from = vector<vector<int>>(N * 2 + 1, vector<int>(M + 1));

        dp[0][0] = from[0][0] = 0;
        for (int i = 0; i <= N * 2; i++) {
            for (int j = 0; j <= M; j++) {
                dp[i][j] = 0;
                if (j && dp[i][j - 1] > dp[i][j]) {
                    dp[i][j] = dp[i][j - 1];
                    from[i][j] = 0;
                }
                if (i && j && s[(i - 1) % N] == t[(j - 1) % M] && dp[i - 1][j - 1] + 1 > dp[i][j]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    from[i][j] = 1;
                }
                if (i && dp[i - 1][j] > dp[i][j]) {
                    dp[i][j] = dp[i - 1][j];
                    from[i][j] = 2;
                }
            }
        }

        int res = 0;
        for (int i = 0; i < N; i++) {
            res = max(res, dp[i + N][M]);

            // re-root
            int x = i + 1, y = 0;
            while (y <= M && from[x][y] == 0)
                y++;

            for (; y <= M && x <= N * 2; x++) {
                from[x][y] = 0;
                --dp[x][M];

                if (x == N * 2)
                    break;

                for (; y <= M; y++) {
                    if (from[x + 1][y] == 2)
                        break;

                    if (y + 1 <= M && from[x + 1][y + 1] == 1) {
                        ++y;
                        break;
                    }
                }
            }
        }

        return res;
    }
};
