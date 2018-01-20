class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty())
            return 0;

        int H = (int)grid.size();
        int W = (int)grid[0].size();
        vector<vector<int>> dp(H, vector<int>(W));

        dp[0][0] = grid[0][0];
        for (int i = 1; i < W; i++)
            dp[0][i] = dp[0][i - 1] + grid[0][i];
        for (int i = 1; i < H; i++)
            dp[i][0] = dp[i - 1][0] + grid[i][0];
        for (int i = 1; i < H; i++) {
            for (int j = 1; j < W; j++)
                dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }

        return dp[H - 1][W - 1];
    }
};