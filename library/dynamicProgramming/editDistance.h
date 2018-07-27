#pragma once

template <typename T>
int minEditDistance(const vector<T>& v1, const vector<T>& v2) {
    int n = int(v1.size());
    int m = int(v2.size());
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    for (int i = 1; i <= m; i++)
        dp[0][i] = i;
    for (int i = 1; i <= n; i++)
        dp[i][0] = i;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (v1[i - 1] == v2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1])) + 1;
        }
    }

    return dp[n][m];
}

int minEditDistance(const string& v1, const string& v2) {
    int n = int(v1.size());
    int m = int(v2.size());
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));

    for (int i = 1; i <= m; i++)
        dp[0][i] = i;
    for (int i = 1; i <= n; i++)
        dp[i][0] = i;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (v1[i - 1] == v2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1])) + 1;
        }
    }

    return dp[n][m];
}
