#pragma once
#pragma warning(disable: 4334)

// O(3^N * N)
inline int minLengthSteinerTree(vector<vector<int>> G, vector<int> verticesToConnect) {
    int N = (int)G.size();
    int M = (int)verticesToConnect.size();
    if (M <= 1)
        return 0;

    for (int k = 0; k < N; k++)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                G[i][j] = min(G[i][j], G[i][k] + G[k][j]);

    vector<vector<int>> dp(1 << M, vector<int>(N));

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            dp[1 << i][j] = G[verticesToConnect[i]][j];

    for (int i = 1; i < 1 << M; i++) {
        if (((i - 1) & i) != 0) {
            for (int j = 0; j < N; j++) {
                dp[i][j] = INT_MAX / 2;
                for (int k = (i - 1) & i; k > 0; k = (k - 1) & i) {
                    dp[i][j] = min(dp[i][j], dp[k][j] + dp[i ^ k][j]);
                }
            }
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    dp[i][j] = min(dp[i][j], dp[i][k] + G[k][j]);
                }
            }
        }
    }

    return dp[(1 << M) - 1][verticesToConnect[0]];
}
