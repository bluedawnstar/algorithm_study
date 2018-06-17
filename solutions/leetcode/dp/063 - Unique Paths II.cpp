// O(n*m)
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        if (obstacleGrid.empty() || obstacleGrid[0].empty())
            return 0;
        int n = (int)obstacleGrid.size();
        int m = (int)obstacleGrid[0].size();

        vector<vector<int>> dp(n, vector<int>(m));
        dp[0][0] = (obstacleGrid[0][0] == 0);
        for (int i = 1; i < m; i++) {
            if (!obstacleGrid[0][i])
                dp[0][i] = dp[0][i - 1];
        }
        for (int i = 1; i < n; i++) {
            if (!obstacleGrid[i][0])
                dp[i][0] = dp[i - 1][0];
        }
        for (int i = 1; i < n; i++) {
            for (int j = 1; j < m; j++) {
                if (!obstacleGrid[i][j])
                    dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }

        return dp[n - 1][m - 1];
    }
};
