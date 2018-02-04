// O(n)
class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = (int)grid.size();

        static int offset[][2] = {
            { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 }
        };

        vector<vector<int>> dp(n, vector<int>(n, -1));
        priority_queue<tuple<int, int, int>> Q;
        Q.emplace(-grid[0][0], 0, 0);
        dp[0][0] = grid[0][0];

        while (!Q.empty()) {
            int cost = -get<0>(Q.top());
            int row = get<1>(Q.top());
            int col = get<2>(Q.top());
            Q.pop();

            for (int i = 0; i < 4; i++) {
                int r = row + offset[i][0];
                int c = col + offset[i][1];
                if (r < 0 || r >= n || c < 0 || c >= n || dp[r][c] >= 0)
                    continue;
                dp[r][c] = max(cost, grid[r][c]);
                Q.emplace(-dp[r][c], r, c);
            }
        }

        return dp[n - 1][n - 1];
    }
};