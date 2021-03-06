#pragma once

// the number of paths from (0, 0) to (width - 1, height - 1) in a grid with obstacles
// - It's allowed to move any number of steps towards the right in a single move
// - It's allowed to move any number of steps downwards in a single move

template <typename T, int mod = 1'000'000'007>
struct GridMultistepPathCounterWithObstaclesMod {
    static T normalize(T x) {
        if (x >= mod)
            x -= mod;
        else if (x < 0)
            x += mod;
        return x;
    }

    // grid[r][c] = obstacles?
    // N = the number of rows
    // M = the number of cols
    // O(N*M)
    static T count(const vector<vector<bool>>& grid, int N, int M) {
        vector<vector<T>> dp(N + 1, vector<T>(M + 1));
        vector<vector<T>> dpH(N + 1, vector<T>(M + 1));
        vector<vector<T>> dpV(N + 1, vector<T>(M + 1));

        dp[0][0] = 1;
        dpH[0][0] = 1;
        dpV[0][0] = 1;
        for (int i = 1; i < M && !grid[0][i]; i++) {
            dp[0][i] = dpH[0][i - 1];
            dpH[0][i] = normalize(dpH[0][i - 1] * 2);
            dpV[0][i] = dp[0][i];
        }
        for (int i = 1; i < N && !grid[i][0]; i++) {
            dp[i][0] = dpV[i - 1][0];
            dpV[i][0] = normalize(dpV[i - 1][0] * 2);
            dpH[i][0] = dp[i][0];
        }
        for (int i = 1; i < N; i++) {
            for (int j = 1; j < M; j++) {
                if (grid[i][j])
                    continue;
                dp[i][j] = normalize(dpV[i - 1][j] + dpH[i][j - 1]);
                dpV[i][j] = normalize(dpV[i - 1][j] + dp[i][j]);
                dpH[i][j] = normalize(dpH[i][j - 1] + dp[i][j]);
            }
        }
        return dp[N - 1][M - 1];
    }
};

// <Related problems>
// https://www.hackerearth.com/practice/algorithms/dynamic-programming/2-dimensional/practice-problems/algorithm/rook-path-142e55ee
